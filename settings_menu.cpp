#include "settings_menu.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <regex>

SettingsMenu::SettingsMenu()
{
    ownId = PollingPlaceId_Settings;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.5},
                             "Start game", ButtonId_StartGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.7},
                             "Main menu", ButtonId_MainMenu));

    Dimensions playerTypeButtonDimensions = {0.5, 0.125};
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.7},
                             "Human", ButtonId_PlayerType1));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.55},
                             "Human", ButtonId_PlayerType2));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.4},
                             "Human", ButtonId_PlayerType3));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.25},
                             "Human", ButtonId_PlayerType4));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, 0.1},
                             "None", ButtonId_PlayerType5));
    buttons.push_back(Button(playerTypeButtonDimensions, {-0.75, -0.05},
                             "None", ButtonId_PlayerType6));


    Dimensions playerNameButtonDimensions = {0.95, 0.125};
    buttons.push_back(Button(playerNameButtonDimensions, {-0.15, 0.7},
                             "Player 1", ButtonId_PlayerName1));
    buttons.push_back(Button(playerNameButtonDimensions, {-0.15, 0.55},
                             "Player 2", ButtonId_PlayerName2));
    buttons.push_back(Button(playerNameButtonDimensions, {-0.15, 0.4},
                             "Player 3", ButtonId_PlayerName3));
    buttons.push_back(Button(playerNameButtonDimensions, {-0.15, 0.25},
                             "Player 4", ButtonId_PlayerName4));
    buttons.push_back(Button(playerNameButtonDimensions, {-0.15, 0.1},
                             "Player 5", ButtonId_PlayerName5));
    buttons.push_back(Button(playerNameButtonDimensions, {-0.15, -0.05},
                             "Player 6", ButtonId_PlayerName6));
}

Settings SettingsMenu::getSettings()
{
    return settings;
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
            backgroundNeedsDrawing = true;
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
                    }
                }
            }
        }
    }
    return PollingPlaceId_Settings;
}

void SettingsMenu::updateScreen()
{
    if(SDL_GetTicks() - lastTicks > 20)
    {
        if (backgroundNeedsDrawing) drawBackground();
        for (auto& button : buttons)
        {
            button.draw();
        }

        SDL_GL_SwapBuffers();
        lastTicks = SDL_GetTicks();
        backgroundNeedsDrawing = false;
    }
}

void SettingsMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.1, 0.1, 0.5);
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

std::vector<std::string> createCaptions()
{
    std::vector<std::string> captions;
    captions.push_back("Human");

    std::vector<std::string> fileNames = getCurDirFileNames();

    std::regex luaFileRegex(".*\\.lua");

    for (const auto &e : fileNames)
    {
        if (std::regex_search(e, luaFileRegex))
        {
            captions.push_back(e);
        }
    }

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
    else return PlayerType_AI;
}

