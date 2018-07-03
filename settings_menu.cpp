#include "settings_menu.hpp"

SettingsMenu::SettingsMenu()
{
    ownId = PollingPlaceId_Settings;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.025},
                             "Start game", ButtonId_StartGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.175},
                             "Main menu", ButtonId_MainMenu));
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
                        case ButtonId_StartGame: return PollingPlaceId_Game;
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
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
