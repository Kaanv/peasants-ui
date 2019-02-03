#include "game_ui.hpp"
#include "constants.hpp"
#include "textures.hpp"
#include "text.hpp"
#include <iostream>
#include <algorithm>

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
                   backgroundNeedsDrawing(true),
                   isPopupActive(false)
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
    buttons.push_back(Button(defaultButtonDimensions, {-0.4875, 0.1},
                             "OK", ButtonId_PopupOk));

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
    cardTopTexture = getTexture("Images//cards//cardtop.png");
}

void GameUI::calculateIsAIOnlyGame()
{
    isGameAIOnly = true;
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (settings.playerTypes[i] == PlayerType_Human)
        {
            isGameAIOnly = false;
            break;
        }
    }
}

void GameUI::setSettings(Settings settings)
{
    this->settings = settings;
    numberOfPlayers = settings.numberOfPlayers;
    game = Game(numberOfPlayers);
}

PollingPlaceId GameUI::startEventPoll()
{
    if (isGameAIOnly)
    {
        game.performAITurnLua();
        if (game.hasRoundEnded())
        {
            game.nextRound();
            exchangePlayersCards();
            game.setStartingPlayer();
            std::cout << "END OF ROUND" << std::endl;
            if (game.getNumberOfEndedRounds() >= 20)
            {
                std::string text = "Game ended";
                drawPopup(text.c_str());
                presentAIGameResults();
                return PollingPlaceId_MainMenu;
            }
        }
    }

    while (SDL_PollEvent(&event))
    {
        if (game.hasRoundEnded())
        {
            std::string text =
                "End of round";
            drawPopup(text.c_str());
            game.nextRound();
            exchangePlayersCards();
            game.setStartingPlayer();
            drawCurrentPlayerPopup();
        }
        if (isCurrentPlayerAI() and not isPopupActive and not isGameAIOnly)
        {
            game.performAITurnLua();
            forceDrawingEverything();
            drawCurrentPlayerPopup();
        }

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
                    try
                    {
                        switch (button.getButtonId())
                        {
                            case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                            case ButtonId_ExitGame: return PollingPlaceId_Exit;
                        }
                        if (not isPopupActive)
                        {
                            switch (button.getButtonId())
                            {
                                case ButtonId_PassTurn:
                                {
                                    game.passCurrentPlayerTurn();
                                    game.getCurrentPlayer().unselectAllCards();
                                    game.nextPlayer();
                                    forceDrawingEverything();
                                    drawCurrentPlayerPopup();
                                    break;
                                }
                                case ButtonId_ThrowCards:
                                {
                                    game.throwCards(game.getCurrentPlayer().getSelectedCards());
                                    game.getCurrentPlayer().removeSelectedCards();
                                    game.checkIfPlayerHasEnded();
                                    game.nextPlayer();
                                    forceDrawingEverything();
                                    drawCurrentPlayerPopup();
                                }
                            }
                        }
                        else
                        {
                            switch (button.getButtonId())
                            {
                                case ButtonId_PopupOk:
                                {
                                    isPopupActive = false;
                                    forceDrawingEverything();
                                    break;
                                }
                            }
                        }
                    }
                    catch (const std::runtime_error & e)
                    {
                        drawPopup(e.what());
                    }
                }
            }
        }
    }
    return PollingPlaceId_Game;
}

void GameUI::forceDrawButtons()
{
    for (auto& button : buttons)
    {
        button.forceDraw();
        button.draw();
    }
}

void GameUI::updateScreen()
{
    if (isPopupActive)
    {
        forceDrawButtons();
        SDL_GL_SwapBuffers();
    }
    else if(SDL_GetTicks() - lastTicks > 20)
    {
        if (backgroundNeedsDrawing)
        {
            drawBackground();
            drawButtonPanel();
            drawCards();
            drawPeasantsInfo();
        }

        for (auto& button : buttons)
        {
            if (button.getButtonId() != ButtonId_PopupOk) button.draw();
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

void GameUI::drawCardTop(Position position)
{
    turnOnTextureMode(cardTopTexture);

    drawTexturedRectangle(
        {CARD_WIDTH, CARD_HEIGHT},
        {position.x, position.y});

    turnOffTextureMode();
}

void GameUI::drawCardTopHorizontal(Position position)
{
    turnOnTextureMode(cardTopTexture);

    drawTexturedRectangle(
        {CARD_WIDTH, CARD_HEIGHT},
        {position.x, position.y});

    turnOffTextureMode();
}

void GameUI::drawCards()
{
    drawCurrentPlayerCards();
    drawAnotherPlayerCards();
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

void GameUI::drawAnotherPlayerCards()
{
    int currentPlayerId = game.getCurrentPlayer().getId();

    int nextPlayerId = (currentPlayerId + 1) % numberOfPlayers;
    unsigned int numberOfCards = game.getPlayer(nextPlayerId).getCards().size();

    for (unsigned int j = 0; j < numberOfCards; j++)
    {
        drawCardTopHorizontal(Position{-0.9, 0.0 + static_cast<double>(j) * CARD_SPACE});
    }

    nextPlayerId = (nextPlayerId + 1) % numberOfPlayers;
    numberOfCards = game.getPlayer(nextPlayerId).getCards().size();

    for (unsigned int j = 0; j < numberOfCards; j++)
    {
        drawCardTop(Position{-0.5 + static_cast<double>(j) * CARD_SPACE, 0.93});
    }

    nextPlayerId = (nextPlayerId + 1) % numberOfPlayers;
    numberOfCards = game.getPlayer(nextPlayerId).getCards().size();

    for (unsigned int j = 0; j < numberOfCards; j++)
    {
        drawCardTopHorizontal(Position{0.3, 0.0 + static_cast<double>(j) * CARD_SPACE});
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

void GameUI::drawPeasantsInfo()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 40));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Peasant levels:",
                      font,
                      textColor,
                      0.75,
                      0.2,
                      0.1);

    for (int i = 0; i < numberOfPlayers; i++)
    {
        std::string text =
            "Player " + std::to_string(i + 1) + ": " +
            std::to_string(game.getPlayer(i).getPeasantLevel());
        SDL_GL_RenderText(text.c_str(),
                          font,
                          textColor,
                          0.75,
                          0.13 - i * 0.06,
                          0.1);
    }
}

void GameUI::updateCardsSelection(int x, int y)
{
    Position glPosition = {static_cast<double>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                           static_cast<double>(-y) * 2.0 / SCREEN_HEIGHT + 1.0};

    Cards cards = game.getCurrentPlayer().getCards();
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        if (cards[i].selected) updateSelectedCardSelection(glPosition,
                                                           cards,
                                                           i);
        else updateNotSelectedCardSelection(glPosition,
                                            cards,
                                            i);
    }
}

void GameUI::updateSelectedCardSelection(Position glPosition,
                                         const Cards& cards,
                                         unsigned int cardIndex)
{
    float width = cardIndex == cards.size() - 1 ? CARD_WIDTH : CARD_SPACE;
    float cardX = -0.5 + static_cast<float>(cardIndex) * CARD_SPACE;
    float cardY = -0.6;

    if (glPosition.x >= cardX and
        glPosition.x <= cardX + width and
        glPosition.y <= cardY and
        glPosition.y >= cardY - CARD_HEIGHT)
    {
        game.getCurrentPlayer().unselectCard(cardIndex);
        forceDrawingEverything();
    }
}

void GameUI::updateNotSelectedCardSelection(Position glPosition,
                                            const Cards& cards,
                                            unsigned int cardIndex)
{
    float width = cardIndex == cards.size() - 1 ? CARD_WIDTH : CARD_SPACE;
    float cardX = -0.5 + static_cast<float>(cardIndex) * CARD_SPACE;
    float cardY = -0.6;

    if (glPosition.x >= cardX and
        glPosition.x <= cardX + width and
        glPosition.y <= cardY and
        glPosition.y >= cardY - CARD_HEIGHT)
    {
        game.getCurrentPlayer().selectCard(cardIndex);
        forceDrawingEverything();
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

void GameUI::drawPopup(std::string text)
{
    Dimensions fullScreen{1.5, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.0, 0.0, 0.8);
    drawRectangle(fullScreen,
                  rightLeftCorner);

    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 40));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText(text.c_str(),
                      font,
                      textColor,
                      -0.25,
                      0.2,
                      0.1);
    forceDrawButtons();
    SDL_GL_SwapBuffers();
    isPopupActive = true;
}

void GameUI::drawCurrentPlayerPopup()
{
    std::string text = "Player " + std::to_string(game.getCurrentPlayer().getId() + 1) + " turn";;
    if (isCurrentPlayerAI()) text = "AI " + text;
    drawPopup(text.c_str());
}

bool GameUI::isCurrentPlayerAI()
{
    return settings.playerTypes[game.getCurrentPlayer().getId()] == PlayerType_AI;
}

void GameUI::takeCardsFromPeasants()
{
    for (int id = 0; id < numberOfPlayers; id++)
    {
        if (game.getPlayer(id).getPeasantLevel() < 0)
        {
            Cards cardsToGiveAway;

            for (int j = 0; j > game.getPlayer(id).getPeasantLevel(); j--)
            {
                cardsToGiveAway.push_back(game.getPlayer(id).takeBestCard());
            }

            unsigned int masterId = game.findOppositePlayerId(game.getPlayer(id).getPeasantLevel());

            for (unsigned int j = 0; j < cardsToGiveAway.size(); j++)
            {
                game.getPlayer(masterId).insertCard(cardsToGiveAway[j]);
            }
        }
    }
}

void GameUI::giveCardsToPeasants()
{
    for (int id = 0; id < numberOfPlayers; id++)
    {
        if (game.getPlayer(id).getPeasantLevel() > 0)
        {
            if (settings.playerTypes[id] == PlayerType_AI)
            {
                game.giveCardsToPeasantAsAI(id);
            }
        }
    }
}

void GameUI::exchangePlayersCards()
{
    takeCardsFromPeasants();
    giveCardsToPeasants();
}

void GameUI::enteringAction()
{
    calculateIsAIOnlyGame();
    if (isGameAIOnly)
    {
        std::string text = "Calculating AI game results...";
        drawPopup(text.c_str());
    }
    else
    {
        drawCurrentPlayerPopup();
    }
}

namespace
{

struct Score
{
    int mastersScore;
    int positiveScore;
    int playerNumber;

    bool operator==(const Score& score)
    {
        return (this->mastersScore == score.mastersScore and
                this->positiveScore == score.positiveScore);
    }
    bool operator>(const Score& score)
    {
        return (this->mastersScore > score.mastersScore or
                (this->mastersScore == score.mastersScore and
                this->positiveScore > score.positiveScore));
    }
    bool operator<(const Score& score)
    {
        return (this->mastersScore < score.mastersScore or
                (this->mastersScore == score.mastersScore and
                this->positiveScore < score.positiveScore));
    }
};

int calculateNumberOfMasterTimes(const LevelsHistory& levelsHistory)
{
    int numberOfMasterTimes = 0;

    for (unsigned int i = 0; i < levelsHistory.size(); i++)
    {
        if (levelsHistory[i] > 0)
        {
            numberOfMasterTimes++;
        }
    }
    return numberOfMasterTimes;
}

int calculatePositiveScoreAsMaster(const LevelsHistory& levelsHistory)
{
    int positiveScore = 0;

    for (unsigned int i = 0; i < levelsHistory.size(); i++)
    {
        if (levelsHistory[i] > 0)
        {
            positiveScore += levelsHistory[i];
        }
    }
    return positiveScore;
}

}

void GameUI::presentAIGameResults()
{
    std::cout << "RESULTS" << std::endl;

    const std::vector<LevelsHistory>& levelsHistory = game.getLevelsHistory();
    std::vector<Score> scores;

    for (int i = 0; i < numberOfPlayers; i++)
    {
        int numberOfMasterTimes = calculateNumberOfMasterTimes(levelsHistory[i]);
        int positiveScore = calculatePositiveScoreAsMaster(levelsHistory[i]);
        scores.push_back(Score{numberOfMasterTimes, positiveScore, i});
    }
    std::sort(scores.begin(), scores.end());
    std::reverse(scores.begin(), scores.end());

    for (unsigned int i = 0; i < scores.size(); i++)
    {
        std::cout << "PLAYER " << scores[i].playerNumber << " result:" << std::endl;
        std::cout << "MASTER TIMES: " << scores[i].mastersScore << std::endl;
        std::cout << "POSITIVE SCORE: " << scores[i].positiveScore << std::endl;
    }
}
