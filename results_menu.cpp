#include "results_menu.hpp"
#include <iostream>
#include "text.hpp"

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
        drawResults();

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

void ResultsMenu::drawResults()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 100));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("END OF GAME RESULTS",
                      font,
                      textColor,
                      0.0,
                      0.9,
                      0.2);

    SDL_GL_RenderText("PLAYER",
                      font,
                      textColor,
                      -0.6,
                      0.5,
                      0.1);

    SDL_GL_RenderText("MASTER TIMES",
                      font,
                      textColor,
                      -0.0,
                      0.5,
                      0.1);

    SDL_GL_RenderText("POSITIVE SCORE",
                      font,
                      textColor,
                      0.6,
                      0.5,
                      0.1);

    for (unsigned int i = 0; i < scores.size(); i++)
    {
        std::string playerText =
            "PLAYER " + std::to_string(scores[i].playerNumber + 1);
        SDL_GL_RenderText(playerText.c_str(),
                          font,
                          textColor,
                          -0.6,
                          0.3 - i * 0.1,
                          0.1);

        std::string masterText =
            std::to_string(scores[i].mastersScore);
        SDL_GL_RenderText(masterText.c_str(),
                          font,
                          textColor,
                          0.0,
                          0.3 - i * 0.1,
                          0.1);

        std::string scoreText =
            std::to_string(scores[i].positiveScore);
        SDL_GL_RenderText(scoreText.c_str(),
                          font,
                          textColor,
                          0.6,
                          0.3 - i * 0.1,
                          0.1);
    }
}

