#include "game_ui.hpp"
#include "constants.hpp"
#include <iostream>
#include "textures.hpp"

namespace
{

const double CARD_WIDTH = 0.17;
const double CARD_HEIGHT = 0.3;
const double CARD_SPACE = 0.06;
const double CARD_SELECT_HEIGHT = 0.05;

std::map<Color, std::string> colorMap
{
    {hearts, "hearts"},
    {diamonds, "diamonds"},
    {clubs, "clubs"},
    {spades, "spades"}
};

std::map<Value, std::string> valueMap
{
    {three, "three"},
    {four, "four"},
    {five, "five"},
    {six, "six"},
    {seven, "seven"},
    {eight, "eight"},
    {nine, "nine"},
    {ten, "ten"},
    {jack, "jack"},
    {queen, "queen"},
    {king, "king"},
    {ace, "ace"}
};

}

GameUI::GameUI() : numberOfPlayers(4),
                   game(numberOfPlayers),
                   lastTicks(0),
                   backgroundNeedsDrawing(true)
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

    std::string filePath;
    for (int color = hearts; color <= spades; color++)
    {
        for (int value = three; value <= ace; value++)
        {
            filePath = "Images//cards//"
                     + valueMap[static_cast<Value>(value)]
                     + colorMap[static_cast<Color>(color)]
                     + ".png";
            textureMap[std::make_pair<Color, Value>(
                           static_cast<Color>(color),
                           static_cast<Value>(value))] =
                getTexture(filePath);
        }
    }
}

PollingPlaceId GameUI::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlaceId_Exit;
        else if (event.type == SDL_ACTIVEEVENT &&
                 event.active.state & SDL_APPACTIVE &&
                 event.active.gain != 0) updateScreen();
        else if (event.type == SDL_VIDEOEXPOSE)
        {
            forceDrawingEverything();
            updateScreen();
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            updateButtonsOnMotion(event.motion.x, event.motion.y);
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            updateButtonsClickStatus();
            updateCardsSelection(event.motion.x, event.motion.y);
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
                        case ButtonId_PassTurn:
                        {
                            game.passCurrentPlayerTurn();
                            game.getCurrentPlayer().unselectAllCards();
                            game.nextPlayer();
                            forceDrawingEverything();
                            break;
                        }
                        case ButtonId_ThrowCards:
                        {
                            game.throwCards(game.getCurrentPlayer().getSelectedCards());
                            game.getCurrentPlayer().removeSelectedCards();
                            game.checkIfPlayerHasEnded();
                            game.nextPlayer();
                            forceDrawingEverything();
                        }
                    }
                }
            }
        }
    }
    return PollingPlaceId_Game;
}

void GameUI::updateScreen()
{
    if(SDL_GetTicks() - lastTicks > 20)
    {
        if (backgroundNeedsDrawing)
        {
            drawBackground();
            drawButtonPanel();
            drawCards();
        }

        for (auto& button : buttons)
        {
            button.draw();
        }

        SDL_GL_SwapBuffers();
        lastTicks = SDL_GetTicks();
        backgroundNeedsDrawing = false;
    }
}

void GameUI::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.0, 0.5, 0.0);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

void GameUI::drawButtonPanel()
{
    float width = 0.5;

    Dimensions fullVertical{width, 2.0};
    Position position{1.0 - width, 1.0};

    glColor3f(0.1, 0.1, 0.5);
    drawRectangle(fullVertical,
                  position);
}

void GameUI::drawCard(Card card, Position position)
{
    GLuint texture = textureMap[std::make_pair<Color, Value>(
                                    static_cast<Color>(card.color),
                                    static_cast<Value>(card.value))];
    turnOnTextureMode(texture);

    drawTexturedRectangle(
        {CARD_WIDTH, CARD_HEIGHT},
        {position.x, card.selected ? position.y : position.y - CARD_SELECT_HEIGHT});

    turnOffTextureMode();
}

void GameUI::drawCards()
{
    drawCurrentPlayerCards();
    drawTableCards();
}

void GameUI::drawCurrentPlayerCards()
{
    Cards cards = game.getCurrentPlayer().getCards();
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        drawCard(cards[i], Position{-0.5 + static_cast<double>(i) * CARD_SPACE, -0.6});
    }
}

void GameUI::drawTableCards()
{
    const Cards& cards = game.getCardsFromTableTop();
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        drawCard(cards[i], Position{-0.4 + static_cast<double>(i) * CARD_SPACE, 0.2});
    }
}

void GameUI::updateCardsSelection(int x, int y)
{
    float glX = static_cast<float>(x) * 2.0 / SCREEN_WIDTH - 1.0;
    float glY = static_cast<float>(-y) * 2.0 / SCREEN_HEIGHT + 1.0;

    Cards cards = game.getCurrentPlayer().getCards();
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        float width = i == cards.size() - 1 ? CARD_WIDTH : CARD_SPACE;
        float cardX = -0.5 + static_cast<float>(i) * CARD_SPACE;
        float cardY = -0.6;

        if (glX >= cardX and
            glX <= cardX + width and
            glY <= cardY and
            glY >= cardY - CARD_HEIGHT)
        {
            game.getCurrentPlayer().selectCard(i);
            forceDrawingEverything();
        }
    }
}

void GameUI::forceDrawingEverything()
{
    backgroundNeedsDrawing = true;
    for (auto& button : buttons)
    {
        button.forceDraw();
    }
}
