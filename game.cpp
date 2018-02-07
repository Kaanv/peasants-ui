#include "game.hpp"
#include "constants.hpp"

Game::Game()
{
    ownId = PollingPlaceId_Game;
    Dimensions defaultButtonDimensions = {0.475, 0.125};

    buttons.push_back(Button(defaultButtonDimensions, {0.5125, 0.660},
                             "Throw cards", ButtonId_ThrowCards));
    buttons.push_back(Button(defaultButtonDimensions, {0.5125, 0.525},
                             "Pass turn", ButtonId_PassTurn));

    buttons.push_back(Button(defaultButtonDimensions, {0.5125, -0.660},
                             "Main menu", ButtonId_MainMenu));
    buttons.push_back(Button(defaultButtonDimensions, {0.5125, -0.795},
                             "Exit game", ButtonId_ExitGame));
}

PollingPlaceId Game::startEventPoll()
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
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                        case ButtonId_ExitGame: return PollingPlaceId_Exit;
                    }
                }
            }
        }
    }
    return PollingPlaceId_Game;
}

void Game::updateScreen()
{
    drawBackground();
    drawButtonPanel();

    for (auto& button : buttons)
    {
        button.draw();
    }

    SDL_GL_SwapBuffers();
}

void Game::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.0, 0.5, 0.0);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

void Game::drawButtonPanel()
{
    float width = 0.5;

    Dimensions fullVertical{width, 2.0};
    Position position{1.0 - width, 1.0};

    glColor3f(0.1, 0.1, 0.5);
    drawRectangle(fullVertical,
                  position);
}
