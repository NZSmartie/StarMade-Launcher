#include "mainwindow.h"

#include <iostream>
#include <fstream>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif // _WIN32
#include <thread>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <curl/curl.h>
#include <GLFW/glfw3.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "ogl.h"
#include "widgetpane.h"
#include "widgetbutton.h"
#include "fontrenderer.h"
#include "widgettextarea.h"
#include "oauthcontroller.h"
#include "connectionutil.h"
#include "messagebox.h"
#include "platformutil.h"

std::shared_ptr<MainWindow> MainWindow::m_instance;

void MainWindow::newInstance(int borderSize)
{
    if (m_instance == nullptr)
    {
        m_instance = std::shared_ptr<MainWindow>(new MainWindow(borderSize));
    }
}

std::shared_ptr<MainWindow> MainWindow::getInstance()
{
    return m_instance;
}

MainWindow::MainWindow(int borderSize)
    : m_size(0, 0),
    m_mousePosition(0, 0),
    m_mousePositionLast(0, 0),
    m_mouseLastClickedPosition(0, 0),
    m_closeRequested(false),
    m_minimizeRequested(false),
    m_windowMoveRequest(0, 0),
    m_borderSize(borderSize),
    m_windowGrabbed(false)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
    glClearColor(0.1, 0.1, 0.1, 1.0F);
    glDisable(GL_DEPTH_TEST);

    WidgetPane* mainWidget = new WidgetPane();
    mainWidget->setPosition(Vector2I(0, 0));
    mainWidget->setSize(m_size);
    mainWidget->setColor(0, 0, 0);
    m_mainWidget = std::shared_ptr<WidgetPane>(mainWidget);

    WidgetPane* topBar = new WidgetPane(m_mainWidget.get());
    topBar->setPosition(Vector2I(0, 0));
    topBar->setSize(Vector2I(width(), 74));
    topBar->setColor(28, 107, 127);
    Border topBarBorder({ 5, Vector3I(11, 56, 72), BorderMode::BOTTOM });
    topBar->setBorder(topBarBorder);

    /**
    WidgetPane* schineLogo = new WidgetPane(m_mainWidget.get());
    schineLogo->setPosition(Vector2I(27, 16));
    schineLogo->setSize(Vector2I(42, 42));
    schineLogo->setTexture(std::string("data/textures/schine_small.png"));
    */

    WidgetPane* rightBar = new WidgetPane(m_mainWidget.get());
    rightBar->setPosition(Vector2I(906, 74));
    rightBar->setSize(Vector2I(294, 676));
    rightBar->setColor(34, 34, 40);

    WidgetButton* launchButton = new WidgetButton("LAUNCH", BUTTON_LAUNCH,
                                                  FontListEntry::BLENDER_PRO_BOLD_32,
                                                  nullptr, rightBar);
    launchButton->setPosition(Vector2I(929, 649));
    launchButton->setSize(Vector2I(256, 87));
    launchButton->setColor(245, 245, 245);
    launchButton->setTexture(std::string("data/textures/launch_button.png"));
    launchButton->setHoverColor(Vector3I(255, 255, 255));

    WidgetButton* skinSelection = new WidgetButton("Dedicated Server", BUTTON_DEDICATED_SERVER,
                                                    FontListEntry::BLENDER_PRO_12,
                                                    nullptr, rightBar);
    skinSelection->setPosition(Vector2I(929, 597));
    skinSelection->setSize(Vector2I(256, 38));
    skinSelection->setColor(255, 255, 255);
    skinSelection->setTexture(std::string("data/textures/button_small.png"));

    WidgetButton* closeButton = new WidgetButton("", BUTTON_CLOSE, FontListEntry::BLENDER_PRO_16, this, topBar);
    closeButton->setPosition(Vector2I(1162, 22));
    closeButton->setSize(Vector2I(22, 24));
    closeButton->setColor(255, 255, 255);
    closeButton->setTexture(std::string("data/textures/close_button.png"));
    closeButton->setTextureCoordinates({ Vector2F(0.0F, 0.0F), Vector2F(0.6875F, 0.75F) });
    closeButton->setClickableInBackground(true);

    WidgetButton* minimizeButton = new WidgetButton("", BUTTON_MINIMIZE, FontListEntry::BLENDER_PRO_16, this, topBar);
    minimizeButton->setPosition(Vector2I(1118, 22));
    minimizeButton->setSize(Vector2I(22, 24));
    minimizeButton->setColor(255, 255, 255);
    minimizeButton->setTexture(std::string("data/textures/close_button.png"));
    minimizeButton->setTextureCoordinates({ Vector2F(0.0F, 0.75F), Vector2F(0.90625F, 0.1875F) });
    minimizeButton->setDrawOffset({ Vector2F(0.0F, 17.0F), Vector2F(0.0F, -18.0F) });
    minimizeButton->setClickableInBackground(true);

    const int TOP_BAR_BUTTON_WIDTH = 1000;
    for (int i = 0; i < 6; ++i)
    {
        std::string text;

        switch  (i + 2)
        {
        case BUTTON_NEWS:
            text = "News";
            break;
        case BUTTON_UPDATE:
            text = "Update";
            break;
        case BUTTON_OPTIONS:
            text = "Options";
            break;
        case BUTTON_TOOLS:
            text = "Tools";
            break;
        case BUTTON_COMMUNITY:
            text = "Community";
            break;
        case BUTTON_HELP:
            text = "HELP";
            break;
        }

        WidgetButton* topBarButton = new WidgetButton(text, i + 2,
                                                        FontListEntry::BLENDER_PRO_BOLD_24,
                                                        this, topBar);
        topBarButton->setPosition(Vector2I((TOP_BAR_BUTTON_WIDTH / 6) * i, 0));
        topBarButton->setSize(Vector2I(TOP_BAR_BUTTON_WIDTH / 6, 69));
        topBarButton->setColor(28, 107, 127);
        topBarButton->setHoverColor(Vector3I(41, 116, 135));
    }

    WidgetTextArea* textArea = new WidgetTextArea(m_mainWidget.get());
    textArea->setPosition(Vector2I(5, 79));
    textArea->setSize(Vector2I(871, 601));
    textArea->setColor(0, 0, 0);
    Border textAreaBorder({ 1, Vector3I(42, 42, 52) });
    textArea->setBorder(textAreaBorder);
    textArea->setScrollBar(7, 14, Vector3I(20, 20, 27), Vector3I(61, 61, 71));

    std::string username("Player1");
    FontListEntry usernameFont = FontListEntry::BLENDER_PRO_BOLD_24;
    Vector2F textSize = FontRenderer::getTextSize(usernameFont, username);
    if (textSize.x() > 240.0F)
    {
        usernameFont = FontListEntry::BLENDER_PRO_BOLD_16;
        textSize = FontRenderer::getTextSize(usernameFont, username);
        if (textSize.x() > 240.0F)
        {
            usernameFont = FontListEntry::BLENDER_PRO_BOLD_12;
            textSize = FontRenderer::getTextSize(usernameFont, username);
        }
    }
    m_usernameButton = new WidgetButton(username, BUTTON_USERNAME,
                                                usernameFont,
                                                this, rightBar);
    m_usernameButton->setPosition(Vector2I(919, 100));
    m_usernameButton->setSize(Vector2I(static_cast<int>(std::min(textSize.x() + 8, 240.0F)), textSize.y() + 16));
    m_usernameButton->setColor(34, 34, 40);
    m_usernameButton->setHoverColor(Vector3I(68, 68, 80));

    m_accountActivatedIndicator = new WidgetButton("", BUTTON_USERNAME, FontListEntry::BLENDER_PRO_16, this, rightBar);
    m_accountActivatedIndicator->setPosition(Vector2I(m_usernameButton->getPosition().x() + m_usernameButton->getSize().x() + 6,
                                             m_usernameButton->getPosition().y() + m_usernameButton->getSize().y() / 2 - 8));
    m_accountActivatedIndicator->setSize(Vector2I(18, 18));
    m_accountActivatedIndicator->setColor(255, 255, 255);
    m_accountActivatedIndicator->setTexture(std::string("data/textures/activated_indicator.png"));
    m_accountActivatedIndicator->setTextureCoordinates({ Vector2F(0.0F, 0.0F), Vector2F(0.28125F, 0.28125F) });

    m_oauthController = std::shared_ptr<OAuthController>(new OAuthController());

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl;
    CURLcode res;
    ConnectionUtil::BufferStruct output;

    curl = curl_easy_init();
    if (curl)
    {
        ConnectionUtil::setWriteOptions(curl, output);
        curl_easy_setopt(curl, CURLOPT_URL, "http://star-made.org/news.rss");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform failed " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);

        if (output.buffer != nullptr)
        {
            std::stringstream ss(output.buffer);
            std::string item;
            using namespace std;
            vector<string> splitLines;
            bool inDescription = false;
            while (std::getline(ss, item, '\n'))
            {
                std::string line = item;
                int lastFoundPos = -1;
                int lastFoundPos0 = -1;
                size_t findPos = 0;
                replaceAllInLine(line, "&lt;", "<");
                replaceAllInLine(line, "lt;", "<");
                replaceAllInLine(line, "-&amp;gt;", "->");

                replaceAllInLine(line, "&gt;", ">");
                replaceAllInLine(line, "gt;", ">");

                replaceAllInLine(line, "&amp;#39;", "'");
                replaceAllInLine(line, "&amp;rsquo;", "'");
                replaceAllInLine(line, "&rsquo;", "'");

                replaceAllInLine(line, "&amp;quot;", "'");
                replaceAllInLine(line, "&quot;", "'");

                replaceAllInLine(line, "&ldquo;", "\"");
                replaceAllInLine(line, "&amp;ldquo;", "\"");
                replaceAllInLine(line, "ldquo;", "\"");
                replaceAllInLine(line, "&rdquo;", "\"");
                replaceAllInLine(line, "&amp;rdquo;", "\"");
                replaceAllInLine(line, "rdquo;", "\"");

                //ldquo;

                if (line.find("<pubDate>") != std::string::npos ||
                    line.find("<link>") != std::string::npos ||
                    line.find("<guid>") != std::string::npos)
                {
                    continue;
                }

                lastFoundPos = -1;

                // Remove some HTML:
                while ((findPos = line.find("&amp;nbsp;")) != std::string::npos)
                {
                    line.erase(findPos, 10);
                }
                findPos = line.find("<title>");
                if (findPos != std::string::npos)
                {
                    size_t findPos0 = line.find("</title>");
                    if (findPos0 != std::string::npos)
                    {
                        std::string title(line.substr(findPos + 7, findPos0 - findPos - 7));
                        if (title.compare("starmade") != 0)
                        {
                            splitLines.push_back("<linet>" + title);
                            splitLines.push_back("<linet>");
                        }
                    }
                }
                else
                {
                    findPos = line.find("<description>");
                    if (!inDescription && findPos != std::string::npos)
                    {
                        inDescription = true;
                    }
                    if (inDescription)
                    {
                        while ((findPos = line.find("<", (lastFoundPos >= 0 ? lastFoundPos + 1 : 0))) != std::string::npos)
                        {
                            size_t findPos0 = line.find(">", (lastFoundPos0 >= 0 ? lastFoundPos0 + 1 : 0));
                            if (findPos0 != std::string::npos)
                            {
                                std::string toRemove(line.substr(findPos, findPos0 + 1 - findPos));
                                if (toRemove.find("<br />") != std::string::npos)
                                {
                                    lastFoundPos0 = findPos0;
                                    lastFoundPos = findPos;
                                }
                                else if (toRemove.find("<a ") != std::string::npos)
                                {
                                    lastFoundPos0 = findPos0;
                                    lastFoundPos = findPos;
                                    size_t findPos1 = toRemove.find("href=");
                                    size_t findPos2 = toRemove.find_first_of("\"", findPos1 + 6);
                                    std::string link(toRemove.substr(findPos1 + 5, findPos2 - 4 - findPos1));
                                    std::string replaceWith("<link " + link + ">");
                                    line.replace(findPos, findPos0 + 1 - findPos, replaceWith);
                                    lastFoundPos0 = findPos + replaceWith.size() - 1;
                                    lastFoundPos = findPos + replaceWith.size() - 1;
                                }
                                else
                                {
                                    line.replace(findPos, findPos0 + 1 - findPos, "");
                                }
                            }
                        }

                        findPos = line.find("</description>");
                        if (findPos != std::string::npos)
                        {
                            inDescription = false;
                        }
                        else
                        {
                            replaceAllInLine(line, "</description>", "");
                            replaceAllInLine(line, "<description>", "");
                            bool forceLineEnd = line.find("<br />") != std::string::npos;

                            while (forceLineEnd)
                            {
                                std::string preLine = line.substr(0, line.find("<br />"));
                                line = line.substr(line.find("<br />"), line.size() - line.find("<br />") - 7);
                                splitLines.push_back("<line>" + preLine);
                                forceLineEnd = line.find("<br />") != std::string::npos;
                            }

                            splitLines.push_back("<line>" + line);
                        }
                    }
                }
            }
            textArea->initWithText(splitLines);

            if (output.buffer)
            {
                free(output.buffer);
                output.buffer = NULL;
                output.size = 0;
            }
        }
    }
}

void MainWindow::replaceAllInLine(std::string& lineToChange, const std::string& toReplace, const std::string& replaceWith)
{
    int lastFoundPos = -1;
    size_t findPos;
    while ((findPos = lineToChange.find(toReplace, (lastFoundPos >= 0 ? lastFoundPos + replaceWith.size() : 0))) != std::string::npos)
    {
        lineToChange.replace(findPos, toReplace.size(), replaceWith);
    }
}

void MainWindow::update(double deltaTime)
{
    m_mainWidget->update(deltaTime);

    if (shouldDisplayMessageBox())
    {
        std::shared_ptr<LauncherMessageBox>& messageBox = m_messageBoxes[0];
        messageBox->update(deltaTime);
    }
}

void MainWindow::render()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->m_mainWidget->draw();
    if (shouldDisplayMessageBox())
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_TEXTURE_2D);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);

        std::shared_ptr<LauncherMessageBox>& messageBox = m_messageBoxes[0];
        glBegin(GL_QUADS);
        glColor4f(0.0F, 0.0F, 0.0F, (float)std::min(0.5, (glfwGetTime() - messageBox->getTimeOpened()) / 3.0));
        glVertex2f(0, 0);
        glVertex2f(m_size.x(), 0);
        glVertex2f(m_size.x(), m_size.y());
        glVertex2f(0, m_size.y());
        glEnd();

        messageBox->render();

        glPopAttrib();
    }
}

void MainWindow::resize(int w, int h)
{
    m_size.setXY(w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, this->width(), this->height(), 0.0f, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::mouseClicked(int button, bool press)
{
    if (shouldDisplayMessageBox())
    {
        std::shared_ptr<LauncherMessageBox>& messageBox = m_messageBoxes[0];
        messageBox->mouseClicked(m_mousePosition, button, press);
    }

    if (press)
    {
        if (m_mousePosition.x() >= this->width() - MINIMIZE_BUTTON_OFFSET_X - CLOSE_BUTTON_SIZE / 2.0F &&
                m_mousePosition.x() <= this->width() - MINIMIZE_BUTTON_OFFSET_X + CLOSE_BUTTON_SIZE / 2.0F &&
                m_mousePosition.y() >= CLOSE_BUTTON_OFFSET - CLOSE_BUTTON_SIZE / 2.0F &&
                m_mousePosition.y() < CLOSE_BUTTON_OFFSET + CLOSE_BUTTON_SIZE / 2.0F)
        {
            m_windowGrabbed = false;
            setMinimizeRequested(true);
        }
        else
        {
            m_mouseLastClickedPosition.setXY(m_mousePosition.x(), m_mousePosition.y());
            m_windowGrabbed = true;
        }
    }
    else
    {
        m_windowGrabbed = false;
    }
    m_mainWidget->mouseClicked(m_mousePosition, button, press, shouldDisplayMessageBox());
}

void MainWindow::mouseMoved(double xPos, double yPos)
{
    m_mousePosition.setXY(xPos, yPos);
    double dX = m_mousePosition.x() - m_mousePositionLast.x();
    double dY = m_mousePosition.y() - m_mousePositionLast.y();
    if (m_windowGrabbed && m_mouseLastClickedPosition.y() > 0 && m_mouseLastClickedPosition.y() < this->height() * 0.1)
    {
        m_windowMoveRequest.setXY(xPos - m_mouseLastClickedPosition.x() + m_borderSize, yPos - m_mouseLastClickedPosition.y() + 30);
    }

    if (shouldDisplayMessageBox())
    {
        std::shared_ptr<LauncherMessageBox>& messageBox = m_messageBoxes[0];
        messageBox->mouseMoved(m_mousePosition, Vector2D(dX, dY));
    }
    else
    {
        m_mainWidget->mouseMoved(m_mousePosition, Vector2D(dX, dY));
    }

    m_mousePositionLast.setXY(xPos, yPos);
}

void MainWindow::mouseWheelScrolled(double xOffset, double yOffset)
{
    if (shouldDisplayMessageBox())
    {

    }
    else
    {
        m_mainWidget->mouseWheelScrolled(xOffset, yOffset);
    }
}

void MainWindow::keyPressed(int key, int action, int mods)
{
    if (shouldDisplayMessageBox())
    {
        std::shared_ptr<LauncherMessageBox>& messageBox = m_messageBoxes[0];
        messageBox->keyPressed(key, action, mods);
    }
}

void MainWindow::charTyped(unsigned int codePoint, int mods)
{
    if (shouldDisplayMessageBox())
    {
        std::shared_ptr<LauncherMessageBox>& messageBox = m_messageBoxes[0];
        messageBox->charTyped(codePoint, mods);
    }
}

void MainWindow::buttonClicked(int callbackIndex)
{
    switch (callbackIndex)
    {
    case BUTTON_CLOSE:
        m_closeRequested = true;
        break;
    case BUTTON_MINIMIZE:
        m_minimizeRequested = true;
        break;
    case BUTTON_USERNAME:
        addMessageBox(std::shared_ptr<LauncherMessageBox>(
             new LauncherMessageBox(Vector2I(700, 150),
                                    { new MessageBoxButton(BUTTON_USERNAME_OK, Vector2I(700 / 2, 30), Vector2I(0, -25), true,  "OK", 2, 0, 40),
                                      new MessageBoxButton(BUTTON_USERNAME_CANCEL, Vector2I(700 / 2, 30), Vector2I(0, -25), true,  "Cancel", 2, 1, 40),
                                      new MessageBoxButton(BUTTON_UPLINK, Vector2I(700, 30), Vector2I(0, -60), true,  "Uplink Acount", 1, 0, 40) },
                                    { new MessageBoxTextWidget(TEXT_BOX_USERNAME, Vector2I(400, 30), Vector2I(0, -45), true, false) },
                                    { new MessageBoxLabelWidget(LABEL_USERNAME, Vector2I(0, 0), Vector2I(0, -63), true, "Username") },
                                    glfwGetTime(),
                                    this)));
        break;
    case BUTTON_UPLINK:
        {
            MainWindow::getInstance()->addMessageBox(std::shared_ptr<LauncherMessageBox>(
                         new LauncherMessageBox(Vector2I(700, 290),
                                                { new MessageBoxButton(BUTTON_UPLINK_NEW_ACCOUNT, Vector2I(700, 30), Vector2I(0, -25 - 60 - 20), true,  "New Account", 1, 0, 40),
                                                  new MessageBoxButton(BUTTON_UPLINK_RESET_CREDENTIALS, Vector2I(700, 30), Vector2I(0, -25 - 30 - 10), true,  "Reset Credentials", 1, 0, 40),
                                                  new MessageBoxButton(BUTTON_UPLINK_OK, Vector2I(700 / 2, 30), Vector2I(0, -25), true,  "OK", 2, 0, 40),
                                                  new MessageBoxButton(BUTTON_UPLINK_CANCEL, Vector2I(700 / 2, 30), Vector2I(0, -25), true,  "Cancel", 2, 1, 40) },
                                                { new MessageBoxTextWidget(TEXT_BOX_UPLINK_USERNAME, Vector2I(400, 30), Vector2I(0, -75), true, false),
                                                  new MessageBoxTextWidget(TEXT_BOX_UPLINK_PASSWORD, Vector2I(400, 30), Vector2I(0, -20), true, true) },
                                                { new MessageBoxLabelWidget(LABEL_UPLINK_STATUS, Vector2I(0, 0), Vector2I(0, -125), true, ""),
                                                  new MessageBoxLabelWidget(LABEL_UPLINK_USERNAME, Vector2I(0, 0), Vector2I(0, -93), true, "Username"),
                                                  new MessageBoxLabelWidget(LABEL_UPLINK_PASSWORD, Vector2I(0, 0), Vector2I(0, -38), true, "Password") },
                                                getCurrentMessageBox()->getTimeOpened(),
                                                this)));
            std::shared_ptr<LauncherMessageBox> messageBox = getCurrentMessageBox();
            MessageBoxTextWidget* usernameBox = messageBox->getTextBoxByID(TEXT_BOX_USERNAME);
            std::string username = usernameBox->text;
            removeCurrentMessageBox();
            messageBox = getCurrentMessageBox();
            usernameBox = messageBox->getTextBoxByID(TEXT_BOX_UPLINK_USERNAME);
            usernameBox->setText(username);
        }
        break;
    case BUTTON_UPLINK_NEW_ACCOUNT:
        PlatformUtil::openWebPage("https://registry.star-made.org/users/password/new");
        break;
    case BUTTON_UPLINK_RESET_CREDENTIALS:
        PlatformUtil::openWebPage("https://registry.star-made.org/users/sign_up");
        break;
    case BUTTON_UPLINK_CANCEL:
    case BUTTON_USERNAME_CANCEL:
        removeCurrentMessageBox();
        break;
    case BUTTON_UPLINK_OK:
        {
            std::shared_ptr<LauncherMessageBox> messageBox = getCurrentMessageBox();
            MessageBoxTextWidget* usernameBox = messageBox->getTextBoxByID(TEXT_BOX_UPLINK_USERNAME);
            MessageBoxTextWidget* passwordBox = messageBox->getTextBoxByID(TEXT_BOX_UPLINK_PASSWORD);
            std::string username = usernameBox->text;
            std::string password = passwordBox->text;
            std::string token = m_oauthController->loginRequest(username, password);
            TokenRequestResult tokenResult = m_oauthController->checkTokenValidity(token);
            if (tokenResult == TokenRequestResult::OK)
            {
                std::cout << token << std::endl;
                updateAccountWidgets(username);
                removeCurrentMessageBox();
            }
            else
            {
                MessageBoxLabelWidget* statusLabel = messageBox->getLabelByID(LABEL_UPLINK_STATUS);
                if (tokenResult == TokenRequestResult::CONNECTION_ERROR)
                {
                    statusLabel->text = "Connection error!";
                }
                else if (tokenResult == TokenRequestResult::INVALID_CREDENTIALS)
                {
                    statusLabel->text = "Invalid Credentials";
                }
                else
                {
                    statusLabel->text = "Unknown error";
                }
            }
        }
        break;
    case BUTTON_USERNAME_OK:
        {
            std::shared_ptr<LauncherMessageBox> cMessageBox = getCurrentMessageBox();
            MessageBoxTextWidget* usernameBox = cMessageBox->getTextBoxByID(TEXT_BOX_USERNAME);
            std::string usernameText = usernameBox->text;
            updateAccountWidgets(usernameText);

            removeCurrentMessageBox();
        }
        break;
    default:
        std::cerr << "Unknown button index: " << callbackIndex << std::endl;
        break;
    }
}

void MainWindow::updateAccountWidgets(const std::string& newUsername)
{
    FontListEntry usernameFont = FontListEntry::BLENDER_PRO_BOLD_24;
    Vector2F textSize = FontRenderer::getTextSize(usernameFont, newUsername);
    if (textSize.x() > 240.0F)
    {
        usernameFont = FontListEntry::BLENDER_PRO_BOLD_16;
        textSize = FontRenderer::getTextSize(usernameFont, newUsername);
        if (textSize.x() > 240.0F)
        {
            usernameFont = FontListEntry::BLENDER_PRO_BOLD_12;
            textSize = FontRenderer::getTextSize(usernameFont, newUsername);
        }
    }
    m_usernameButton->setText(newUsername);
    m_usernameButton->setFont(usernameFont);
    m_usernameButton->setPosition(Vector2I(919, 100));
    m_usernameButton->setSize(Vector2I(static_cast<int>(std::min(textSize.x() + 8, 240.0F)), textSize.y() + 16));
    m_usernameButton->setColor(34, 34, 40);
    m_usernameButton->setHoverColor(Vector3I(68, 68, 80));

    m_accountActivatedIndicator->setPosition(Vector2I(m_usernameButton->getPosition().x() + m_usernameButton->getSize().x() + 6,
                                             m_usernameButton->getPosition().y() + m_usernameButton->getSize().y() / 2 - 8));
    m_accountActivatedIndicator->setSize(Vector2I(18, 18));
    m_accountActivatedIndicator->setColor(255, 255, 255);
    m_accountActivatedIndicator->setTexture(std::string("data/textures/activated_indicator.png"));
    m_accountActivatedIndicator->setTextureCoordinates({ Vector2F(0.0F, 0.0F), Vector2F(0.28125F, 0.28125F) });
}

void MainWindow::textChanged(int callbackIndex, const std::string& newString)
{
    switch (callbackIndex)
    {
    case TEXT_BOX_USERNAME:
        break;
    default:
        std::cerr << "Unknown text box index: " << callbackIndex << std::endl;
        break;
    }
}
