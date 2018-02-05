#include "menu.hpp"
#include "sdl_gl_wrapper.hpp"
#include "constants.hpp"

MainMenu::MainMenu(int x, int y)
{
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    this->x = x;
    this->y = y;
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.425},
                             "New game", ButtonId_NewGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.225},
                             "Exit game", ButtonId_ExitGame));
}

PollingPlace MainMenu::enter()
{
    PollingPlace currentPlace = PollingPlace_MainMenu;
    while (currentPlace == PollingPlace_MainMenu)
    {
        currentPlace = startEventPoll();
    }
    return currentPlace;
}

PollingPlace MainMenu::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlace_Exit;
        else if (event.type == SDL_ACTIVEEVENT &&
                 event.active.state & SDL_APPACTIVE &&
                 event.active.gain != 0) updateScreen();
        else if (event.type == SDL_VIDEOEXPOSE) updateScreen();
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
                        case ButtonId_NewGame: return PollingPlace_Game;
                        case ButtonId_ExitGame: return PollingPlace_Exit;
                    }
                }
            }
        }
    }
    return PollingPlace_MainMenu;
}

void MainMenu::updateScreen()
{
    for (auto& button : buttons)
    {
        button.draw();
    }

    SDL_GL_SwapBuffers();
}

void MainMenu::updateButtonsOnMotion(int x, int y)
{
    for (auto& button : buttons)
    {
        button.updateMotion({static_cast<float>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                             static_cast<float>(-y) * 2.0 / SCREEN_HEIGHT + 1.0});
    }
    updateScreen();
}

void MainMenu::updateButtonsClickStatus()
{
    for (auto& button : buttons)
    {
        button.updateUnderClick();
    }
}
