#include "settings_menu.hpp"
#include <boost/filesystem.hpp>
#include <regex>
#include "text.hpp"

SettingsMenu::SettingsMenu(Settings& _settings) : settings(_settings)
{
    ownId = PollingPlaceId_Settings;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.6},
                             "Start game", ButtonId_StartGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.75},
                             "Main menu", ButtonId_MainMenu));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.25},
                             "20", ButtonId_Rounds));

    Dimensions playerTypeButtonDimensions = {0.95, 0.125};
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.8},
                             "Human", ButtonId_PlayerType1));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.65},
                             "Human", ButtonId_PlayerType2));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.5},
                             "Human", ButtonId_PlayerType3));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.35},
                             "Human", ButtonId_PlayerType4));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.2},
                             "None", ButtonId_PlayerType5));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.05},
                             "None", ButtonId_PlayerType6));


    Dimensions playerNameButtonDimensions = {0.5, 0.125};
    buttons.push_back(Button(playerNameButtonDimensions, {0.35, 0.8},
                             "Player 1", ButtonId_PlayerName1));
    buttons.push_back(Button(playerNameButtonDimensions, {0.35, 0.65},
                             "Player 2", ButtonId_PlayerName2));
    buttons.push_back(Button(playerNameButtonDimensions, {0.35, 0.5},
                             "Player 3", ButtonId_PlayerName3));
    buttons.push_back(Button(playerNameButtonDimensions, {0.35, 0.35},
                             "Player 4", ButtonId_PlayerName4));
    buttons.push_back(Button(playerNameButtonDimensions, {0.35, 0.2},
                             "Player 5", ButtonId_PlayerName5));
    buttons.push_back(Button(playerNameButtonDimensions, {0.35, 0.05},
                             "Player 6", ButtonId_PlayerName6));
}

PollingPlaceId SettingsMenu::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlaceId_Exit;
        else if (event.type == SDL_ACTIVEEVENT &&
                 event.active.state & SDL_APPACTIVE &&
                 event.active.gain != 0) updateScreen();
        else if (event.type == SDL_VIDEOEXPOSE)
        {
            for (auto& button : buttons)
            {
                button.forceDraw();
            }
            updateScreen();
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            updateButtonsOnMotion(event.motion.x, event.motion.y);
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            updateButtonsClickStatus();
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            for (auto& button : buttons)
            {
                if (button.isClicked())
                {
                    switch (button.getButtonId())
                    {
                        case ButtonId_StartGame:
                            setSettingsAccordingToButtons();
                            return PollingPlaceId_Game;
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                        case ButtonId_PlayerType1:
                        case ButtonId_PlayerType2:
                        case ButtonId_PlayerType3:
                        case ButtonId_PlayerType4:
                        case ButtonId_PlayerType5:
                        case ButtonId_PlayerType6:
                            button.setCaption(nextCaption(button.getCaption()));
                            button.immediatelyDraw();
                            break;
                        case ButtonId_Rounds:
                            button.setCaption(nextRoundsCaption(button.getCaption()));
                            button.immediatelyDraw();
                            break;
                    }
                }
            }
        }
    }
    return PollingPlaceId_Settings;
}

namespace
{

void drawMenuTitles()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 50));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Player type:",
                      font,
                      textColor,
                      -0.275,
                      0.9,
                      0.1);

    SDL_GL_RenderText("Player number:",
                      font,
                      textColor,
                      0.6,
                      0.9,
                      0.1);

    SDL_GL_RenderText("Rounds number:",
                      font,
                      textColor,
                      0.0,
                      -0.15,
                      0.1);
}

}

void SettingsMenu::updateScreen()
{
    if(SDL_GetTicks() - lastTicks > 20)
    {
        drawBackground();
        for (auto& button : buttons)
        {
            button.forceDraw();
        }
        for (auto& button : buttons)
        {
            button.draw();
        }

        drawMenuTitles();

        SDL_GL_SwapBuffers();
        lastTicks = SDL_GetTicks();
    }
}

void SettingsMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3d(0.1, 0.1, 0.5);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

std::vector<std::string> getCurDirFileNames()
{
    std::vector<std::string> names;
    boost::filesystem::path current_path(".");
    std::vector<boost::filesystem::directory_entry> v;
    if (boost::filesystem::is_directory(current_path))
    {
        copy(boost::filesystem::directory_iterator(current_path),
             boost::filesystem::directory_iterator(), back_inserter(v));

        for (auto it = v.begin(); it != v.end();  ++it)
        {
            names.push_back((*it).path().string());
        }
    }
    return names;
}

void SettingsMenu::addMenuSpecificCaptions(std::vector<std::string>& captions)
{
    captions.push_back("Human");
}

std::vector<std::string> SettingsMenu::createCaptions()
{
    std::vector<std::string> captions;
    addMenuSpecificCaptions(captions);

    std::vector<std::string> fileNames = getCurDirFileNames();

    std::regex luaFileRegex(".*\\.lua");

    for (const auto &e : fileNames)
    {
        if (std::regex_search(e, luaFileRegex))
        {
            captions.push_back(e);
        }
    }
    captions.push_back("None");

    return captions;
}

std::string SettingsMenu::nextCaption(std::string caption)
{
    std::vector<std::string> captions = createCaptions();

    for (unsigned int i = 0; i < captions.size(); i++)
    {
        if (captions[i] == caption)
        {
            if (i != captions.size() - 1)
            {
                return captions[i + 1];
            }
            else
            {
                return captions[0];
            }
        }
    }
    return "Human";
}

std::string SettingsMenu::nextRoundsCaption(std::string caption)
{
    std::vector<std::string> captions{"20", "50", "1000"};

    for (unsigned int i = 0; i < captions.size(); i++)
    {
        if (captions[i] == caption)
        {
            if (i != captions.size() - 1)
            {
                return captions[i + 1];
            }
            else
            {
                return captions[0];
            }
        }
    }
    return "ERROR";
}

void SettingsMenu::setSettingsAccordingToButtons()
{
    for (auto& button : buttons)
    {
        switch (button.getButtonId())
        {
            case ButtonId_PlayerType1:
            case ButtonId_PlayerType2:
            case ButtonId_PlayerType3:
            case ButtonId_PlayerType4:
            case ButtonId_PlayerType5:
            case ButtonId_PlayerType6:
                settings.playerTypes[button.getButtonId() - ButtonId_PlayerType1] =
                    convertCaptionToPlayerType(button.getCaption());
                settings.aiTypes[button.getButtonId() - ButtonId_PlayerType1] =
                    button.getCaption();
                break;
            case ButtonId_Rounds:
                settings.numberOfRounds = std::stoi(button.getCaption());
                break;
        }
    }

    settings.numberOfPlayers = 0;
    for (int i = 0; i < 6; i++)
    {
        if (settings.playerTypes[i] != PlayerType_None) settings.numberOfPlayers++;
    }
}

PlayerType SettingsMenu::convertCaptionToPlayerType(std::string caption)
{
    if (caption == "Human") return PlayerType_Human;
    else if (caption == "Local Human") return PlayerType_Human;
    else if (caption == "Open") return PlayerType_Open;
    else if (caption == "Network Player") return PlayerType_Network;
    else if (caption == "None") return PlayerType_None;
    return PlayerType_AI;
}

