#include "results_menu.hpp"
#include <iostream>

ResultsMenu::ResultsMenu()
{
    ownId = PollingPlaceId_Results;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.7},
                             "Main menu", ButtonId_MainMenu));
}

void ResultsMenu::setScores(Scores scores)
{
    this->scores = scores;
    std::cout << "RESULTS" << std::endl;
    for (unsigned int i = 0; i < scores.size(); i++)
    {
        std::cout << "PLAYER " << scores[i].playerNumber << " result:" << std::endl;
        std::cout << "MASTER TIMES: " << scores[i].mastersScore << std::endl;
        std::cout << "POSITIVE SCORE: " << scores[i].positiveScore << std::endl;
    }
}

PollingPlaceId ResultsMenu::startEventPoll()
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
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                            break;
                    }
                }
            }
        }
    }
    return PollingPlaceId_Results;
}

void ResultsMenu::updateScreen()
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

void ResultsMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.1, 0.1, 0.5);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

