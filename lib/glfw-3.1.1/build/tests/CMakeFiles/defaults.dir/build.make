# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.2

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build

# Include any dependencies generated for this target.
include tests/CMakeFiles/defaults.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/defaults.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/defaults.dir/flags.make

tests/CMakeFiles/defaults.dir/defaults.c.obj: tests/CMakeFiles/defaults.dir/flags.make
tests/CMakeFiles/defaults.dir/defaults.c.obj: tests/CMakeFiles/defaults.dir/includes_C.rsp
tests/CMakeFiles/defaults.dir/defaults.c.obj: ../tests/defaults.c
	$(CMAKE_COMMAND) -E cmake_progress_report C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/CMakeFiles/defaults.dir/defaults.c.obj"
	cd /d C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests && C:\MinGW\bin\gcc.exe  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles\defaults.dir\defaults.c.obj   -c C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\tests\defaults.c

tests/CMakeFiles/defaults.dir/defaults.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/defaults.dir/defaults.c.i"
	cd /d C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests && C:\MinGW\bin\gcc.exe  $(C_DEFINES) $(C_FLAGS) -E C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\tests\defaults.c > CMakeFiles\defaults.dir\defaults.c.i

tests/CMakeFiles/defaults.dir/defaults.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/defaults.dir/defaults.c.s"
	cd /d C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests && C:\MinGW\bin\gcc.exe  $(C_DEFINES) $(C_FLAGS) -S C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\tests\defaults.c -o CMakeFiles\defaults.dir\defaults.c.s

tests/CMakeFiles/defaults.dir/defaults.c.obj.requires:
.PHONY : tests/CMakeFiles/defaults.dir/defaults.c.obj.requires

tests/CMakeFiles/defaults.dir/defaults.c.obj.provides: tests/CMakeFiles/defaults.dir/defaults.c.obj.requires
	$(MAKE) -f tests\CMakeFiles\defaults.dir\build.make tests/CMakeFiles/defaults.dir/defaults.c.obj.provides.build
.PHONY : tests/CMakeFiles/defaults.dir/defaults.c.obj.provides

tests/CMakeFiles/defaults.dir/defaults.c.obj.provides.build: tests/CMakeFiles/defaults.dir/defaults.c.obj

# Object files for target defaults
defaults_OBJECTS = \
"CMakeFiles/defaults.dir/defaults.c.obj"

# External object files for target defaults
defaults_EXTERNAL_OBJECTS =

tests/defaults.exe: tests/CMakeFiles/defaults.dir/defaults.c.obj
tests/defaults.exe: tests/CMakeFiles/defaults.dir/build.make
tests/defaults.exe: src/libglfw3.a
tests/defaults.exe: tests/CMakeFiles/defaults.dir/linklibs.rsp
tests/defaults.exe: tests/CMakeFiles/defaults.dir/objects1.rsp
tests/defaults.exe: tests/CMakeFiles/defaults.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable defaults.exe"
	cd /d C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\defaults.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/defaults.dir/build: tests/defaults.exe
.PHONY : tests/CMakeFiles/defaults.dir/build

tests/CMakeFiles/defaults.dir/requires: tests/CMakeFiles/defaults.dir/defaults.c.obj.requires
.PHONY : tests/CMakeFiles/defaults.dir/requires

tests/CMakeFiles/defaults.dir/clean:
	cd /d C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests && $(CMAKE_COMMAND) -P CMakeFiles\defaults.dir\cmake_clean.cmake
.PHONY : tests/CMakeFiles/defaults.dir/clean

tests/CMakeFiles/defaults.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1 C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\tests C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests C:\Users\Mitch\Dropbox\Development\Env\C++\Projects\StarMadeLauncher\StarMadeLauncher\lib\glfw-3.1.1\build\tests\CMakeFiles\defaults.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/defaults.dir/depend

