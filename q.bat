@echo off

:: Name: Quick utility
:: Desc: Provides quick packaging, copying, and launching functions
:: Auth: Terra "calani" Rain
:: Date: July 2016
:: Note: This was originally intended to do simple launch commands with options,
::       but quickly expanded into what you see below.  Due to batch's terrible
::       syntax and functionality, the code is kind of awful; I've done my best
::       to keep it as clean and easy-to-read as possible.  Enjoy!


:begin
  if "%1" == ""  goto :help
  set _package=

  set _clean=
  set _launch=

  set _help=
  set _steam=
  set _attach=
  set _detach=
  set _development=
  set _debugging=
  set _arch=64

  set _dest=

  :: set _steam_path=f:\Steam\SteamApps\common\StarMade
  set _vm_path=C:\Users\Terra\Desktop\shared vm
  set _winrar_path=C:\Program Files\WinRAR\winrar.exe
  set _starmade_cache_path=C:/Users/Terra/AppData/Roaming/StarMade/Launcher
goto :parse


:help
  echo Quick launch utility
  echo.
  echo Usage: q [option [option [...]]] [dest:(steam^|vm)]
  echo    p ^| package       Packages the launcher for the current platform
  echo   pa ^| package-all   Packages the launcher (all platforms)
  echo      ^|
  echo    c ^| clean         Clears the launcher cache to simulate a fresh install
  echo      ^|
  echo    l ^| launch        Runs the launcher with any passed flags
  echo      ^|                 - dest:steam modifies the launch directory
  echo   32 ^| ia32          Use the 32bit launcher for all actions
  echo      ^|
  echo    h ^| help          Passes --help
  echo    s ^| steam         Passes --steam        (Implies    --attach)
  echo    a ^| attach        Passes --attach
  echo    d ^| detach        Passes --detach       (Overwrites --attach)
  echo      ^|
  echo    v ^| development   Passes --development  (with passphrase)
  echo   vv ^| debugging     Passes --development --debugging
  echo  vvv ^| verbose       Passes --development --debugging --verbose
  echo      ^|
  echo    t ^| test          Runs the launcher with --development --debugging
  echo   tt ^| test-verbose  Runs the launcher with --development --debugging --verbose
  echo.
  echo Destinations
  echo   dest:vm       Copies the Linux packages to the VM shared folder
  echo   dest:steam    Overwrites previous Steam install with current build
  echo                   - This option will preserve the StarMade game folder
  echo                   - Also modifies `launch` to run within the Steam folder
goto :eof


:parse
  if "%1"==""  goto :run
  if "%1"=="p"             set _package=win64
  if "%1"=="package"       set _package=win64

  if "%1"=="pa"            set _package=all
  if "%1"=="package-all"   set _package=all

  if "%1"=="c"             set _clean=1
  if "%1"=="clean"         set _clean=1

  if "%1"=="l"             set _launch=1
  if "%1"=="launch"        set _launch=1

  if "%1"=="32"            set _arch=32
  if "%1"=="ia32"          set _arch=32

  if "%1"=="h"             set _help=--help
  if "%1"=="help"          set _help=--help

  if "%1"=="s"             set _steam=--steam
  if "%1"=="steam"         set _steam=--steam

  if "%1"=="a"             set _attach=--attach
  if "%1"=="attach"        set _attach=--attach
  
  if "%1"=="d"             set _detach=--detach
  if "%1"=="detach"        set _detach=--detach

  if "%1"=="v"             set _development=--development="here be dragons"
  if "%1"=="development"   set _development=--development="here be dragons"

  set _or=0
    if "%1"=="vv"            set _or=1
    if "%1"=="debugging"     set _or=1
    if "%_or%"=="1" (
      set _development=--development="here be dragons"
      set _debugging=--debugging
    )

  set _or=0
    if "%1"=="vvv"           set _or=1
    if "%1"=="verbose"       set _or=1
    if "%_or%"=="1" (
      set _development=--development="here be dragons"
      set _debugging=--debugging --verbose
    )

  set _or=0
    if "%1"=="t"             set _or=1
    if "%1"=="test"          set _or=1
    if "%_or%"=="1" (
      set _launch=1
      set _development=--development="here be dragons"
      set _debugging=--debugging
    )

  set _or=0
    if "%1"=="tt"            set _or=1
    if "%1"=="test-verbose"  set _or=1
    if "%_or%"=="1" (
      set _launch=1
      set _development=--development="here be dragons"
      set _debugging=--debugging --verbose
    )


  if "%1"=="dest:steam"    set _dest=steam
  if "%1"=="dest:vm"       set _dest=vm

  shift
goto :parse


:run
  if "%_package%"=="win64" (
    echo Packaging
    echo.
    cmd /C gulp package
    if "%errorlevel%"=="1"  goto :package_failed
    echo.
  )
  if "%_package%"=="all" (
    echo Packaging all
    echo.
    cmd /C gulp package --platform all --arch all
    if "%errorlevel%"=="1"  goto :package_failed
    echo.
  )

  if "%_dest%"=="steam" (
    echo Removing previous steam install
    rem This preserves the StarMade game dir and starmade-starter.exe

    rem It isn't possible to replace the paths here with %%_steam_path%. gg Windows.
    for /d %%d in (f:\Steam\SteamApps\common\StarMade\*)   do ( if /i  "%%d" NEQ "f:\Steam\SteamApps\common\StarMade\StarMade"           rd  /S /Q "%%d" )
    for    %%f in (f:\Steam\SteamApps\common\StarMade\*.*) do ( if not "%%f"=="f:\Steam\SteamApps\common\StarMade\starmade-starter.exe"  del /q    "%%f" )
    if "%_arch%"=="64" (
      echo Copying new x64 build to Steam
      xcopy .\dist\starmade-launcher-win32-x64\* f:\Steam\SteamApps\common\StarMade\ /E /V /H /Q
    ) else (
      echo Copying new ia32 build to Steam
      xcopy .\dist\starmade-launcher-win32-ia32\* f:\Steam\SteamApps\common\StarMade\ /E /V /H /Q
    )
    rem /L for xcopy simulation
  )

  if "%_dest%"=="vm" (
    echo Removing previous VM builds
    del /F /Q "%_vm_path%\*.zip"  2> nul

    echo Removing previous zips
    del /F /Q ".\dist\*.zip"  2> nul

    echo Creating zips
    cd dist
    "%_winrar_path%" a -afzip .\starmade-launcher-linux-ia32.zip .\starmade-launcher-linux-ia32
    "%_winrar_path%" a -afzip .\starmade-launcher-linux-x64.zip  .\starmade-launcher-linux-x64
    cd ..

    echo Copying linux builds
    xcopy .\dist\*.zip "%_vm_path%" /V /Q
  )


  if "%_clean%"=="1" (
    echo Cleaning
    rm -rf "%_starmade_cache_path%"
  )


  if "%_launch%"=="1" (
    if "%_dest%"=="steam" (
      echo Launching Steam build
      echo f:\Steam\SteamApps\common\StarMade\starmade-launcher.exe    %_help% %_attach% %_detach% %_steam% %_development% %_debugging%
      echo.
      f:\Steam\SteamApps\common\StarMade\starmade-launcher.exe         %_help% %_attach% %_detach% %_steam% %_development% %_debugging%
    ) else (
      if "%_arch%"=="64" (
        echo Launching ^(x64^)
        echo .\dist\starmade-launcher-win32-x64\starmade-launcher.exe  %_help% %_attach% %_detach% %_steam% %_development% %_debugging%
        echo.
        .\dist\starmade-launcher-win32-x64\starmade-launcher.exe       %_help% %_attach% %_detach% %_steam% %_development% %_debugging%
      ) else (
        echo Launching ^(ia32^)
        echo .\dist\starmade-launcher-win32-ia32\starmade-launcher.exe %_help% %_attach% %_detach% %_steam% %_development% %_debugging%
        echo.
        .\dist\starmade-launcher-win32-ia32\starmade-launcher.exe      %_help% %_attach% %_detach% %_steam% %_development% %_debugging%
      )
    )
  )
goto :cleanup


:package_failed
  echo Packaging failed.
goto :cleanup


:cleanup
  set _package=

  set _clean=
  set _launch=

  set _steam=
  set _attach=
  set _detach=
  set _development=
  set _debugging=

  set _dest=

  set _steam_path=
  set _vm_path=
  set _winrar_path=
  set _starmade_cache_path=

  set _or=

  :: consider cleaning up %appdata%\..\Local\Temp\electron-packager

echo.

:eof