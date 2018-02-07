#include "menu.hpp"
#include "sdl_gl_wrapper.hpp"
#include "constants.hpp"

MainMenu::MainMenu(int x, int y)
{
    ownId = PollingPlaceId_MainMenu;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    this->x = x;
    this->y = y;
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.425},
                             "New game", ButtonId_NewGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.225},
                             "Exit game", ButtonId_ExitGame));
}

PollingPlaceId MainMenu::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlaceId_Exit;
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
                        case ButtonId_NewGame: return PollingPlaceId_Game;
                        case ButtonId_ExitGame: return PollingPlaceId_Exit;
                    }
                }
            }
        }
    }
    return PollingPlaceId_MainMenu;
}

void MainMenu::updateScreen()
{
    drawBackground();
    for (auto& button : buttons)
    {
        button.draw();
    }

    SDL_GL_SwapBuffers();
}

void MainMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.1, 0.1, 0.5);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}
