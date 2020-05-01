#include "game_ui.hpp"
#include "constants.hpp"
#include "textures.hpp"
#include "text.hpp"
#include <iostream>
#include <algorithm>

namespace
{

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

GameUI::GameUI(NetworkServer& _netServer) : numberOfPlayers(4),
                                            isPopupActive(false),
                                            netServer(_netServer)
{
    bool notVisible = false;
    ownId = PollingPlaceId_Game;
    Dimensions defaultButtonDimensions = {0.475, 0.125};

    buttons.push_back(Button(defaultButtonDimensions, {0.5125, 0.95},
                             "Throw cards", ButtonId_ThrowCards));
    buttons.push_back(Button(defaultButtonDimensions, {0.5125, 0.84},
                             "Pass turn", ButtonId_PassTurn));
    buttons.push_back(Button(defaultButtonDimensions, {0.5125, 0.95},
                             "Give away", ButtonId_GiveAway, notVisible));

    buttons.push_back(Button(defaultButtonDimensions, {0.5125, -0.675},
                             "Main menu", ButtonId_MainMenu));
    buttons.push_back(Button(defaultButtonDimensions, {0.5125, -0.825},
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
    for (unsigned int i = 0; i < numberOfPlayers; i++)
    {
        if (settings.playerTypes[i] == PlayerType_Human)
        {
            isGameAIOnly = false;
            break;
        }
    }
}

void GameUI::calculateIsGameOneHumanOnly()
{
    int numberOfHumanFound = 0;
    for (unsigned int i = 0; i < numberOfPlayers; i++)
    {
        if (settings.playerTypes[i] == PlayerType_Human)
        {
            numberOfHumanFound++;
            humanPlayer = i;
        }
    }
    isGameOneHumanOnly = (numberOfHumanFound == 1);
}

void GameUI::setSettings(Settings settings)
{
    this->settings = settings;
    numberOfPlayers = settings.numberOfPlayers;
    game = std::make_unique<Game>(settings);
}

PollingPlaceId GameUI::startEventPoll()
{
    if (isGameAIOnly)
    {
        try { game->performAITurnLua(); }
        catch(...) { game->handleIllegalAITurn(); }
        if (game->hasRoundEnded())
        {
            std::cout << "END OF ROUND" << std::endl;
            processEndOfRound();
            if (game->getNumberOfEndedRounds() >= settings.numberOfRounds)
            {
                createGameResults();
                return PollingPlaceId_Results;
            }
        }
    }

    do
    {
        if (game->hasRoundEnded())
        {
            std::string text =
                "End of round";
            drawPopup(text.c_str());
            processEndOfRound();
            if (game->getNumberOfEndedRounds() >= settings.numberOfRounds)
            {
                createGameResults();
                return PollingPlaceId_Results;
            }
            drawCurrentPlayerPopup();
        }
        if (isCurrentPlayerAI() and not isGameAIOnly)
        {
            try { game->performAITurnLua(); }
            catch(...) { game->handleIllegalAITurn(); }
            forceDrawingEverything();
            if (not isGameOneHumanOnly)
            {
                drawCurrentPlayerPopup();
            }
            updateScreen();
        }
        if (cardsExchangeActive and drawExchangePopup)
        {
            std::string text =
                "Player " + std::to_string(exchangePlayersIds[0] + 1) + " give cards to your peasant";
            drawPopup(text.c_str());
            drawExchangePopup = false;
        }
    }
    while (isCurrentPlayerAI() and not isGameAIOnly and isGameOneHumanOnly);

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
                    try
                    {
                        switch (button.getButtonId())
                        {
                            case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                            case ButtonId_ExitGame: return PollingPlaceId_Exit;
                        }
                        if (not isPopupActive)
                        {
                            if (cardsExchangeActive)
                            {
                                switch (button.getButtonId())
                                {
                                    case ButtonId_GiveAway:
                                    {
                                        game->giveCardsToPeasantAsHuman(exchangePlayersIds[0]);
                                        exchangePlayersIds.erase(exchangePlayersIds.begin());
                                        if (exchangePlayersIds.size() == 0)
                                        {
                                            turnOffCardsExchange();
                                        }
                                        drawExchangePopup = true;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                switch (button.getButtonId())
                                {
                                    case ButtonId_PassTurn:
                                    {
                                        game->passCurrentPlayerTurn();
                                        game->nextPlayer();
                                        forceDrawingEverything();
                                        drawCurrentPlayerPopup();
                                        break;
                                    }
                                    case ButtonId_ThrowCards:
                                    {
                                        game->throwSelectedCards();
                                        game->nextPlayer();
                                        forceDrawingEverything();
                                        drawCurrentPlayerPopup();
                                    }
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
    else
    {
        if (backgroundNeedsDrawing)
        {
            drawBackground();
            drawButtonPanel();
            drawCards();
            drawPeasantsInfo();
            drawPastTurnsInfo();
        }

        for (auto& button : buttons)
        {
            if (button.getButtonId() != ButtonId_PopupOk) button.draw();
        }
    }

    SDL_GL_SwapBuffers();
    lastTicks = SDL_GetTicks();
    backgroundNeedsDrawing = false;
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
    double width = 0.5;

    Dimensions fullVertical{width, 2.0};
    Position position{1.0 - width, 1.0};

    glColor3d(0.1, 0.1, 0.5);
    drawRectangle(fullVertical,
                  position);
}

void GameUI::drawCard(Card card, Position position, double width, double height)
{
    GLuint texture = textureMap[std::make_pair<Color, Value>(
                                    static_cast<Color>(card.color),
                                    static_cast<Value>(card.value))];
    turnOnTextureMode(texture);

    drawTexturedRectangle(
        {width, height},
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
    unsigned int playerId = game->getCurrentPlayer().getId();
    if (isGameOneHumanOnly) playerId = humanPlayer;
    if (cardsExchangeActive)
    {
        playerId = exchangePlayersIds[0];
    }
    Cards cards = game->getPlayer(playerId).getCards();
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        drawCard(cards[i], Position{-0.5 + static_cast<double>(i) * CARD_SPACE, -0.6});
    }
}

void GameUI::drawAnotherPlayerCards()
{
    int currentPlayerId = getCurrentPlayerId();
    if (isGameOneHumanOnly) currentPlayerId = humanPlayer;

    int nextPlayerId = (currentPlayerId + 1) % numberOfPlayers;
    unsigned int numberOfCards = game->getPlayer(nextPlayerId).getCards().size();

    for (unsigned int j = 0; j < numberOfCards; j++)
    {
        drawCardTopHorizontal(Position{-0.9, 0.0 + static_cast<double>(j) * CARD_SPACE});
    }

    nextPlayerId = (nextPlayerId + 1) % numberOfPlayers;
    numberOfCards = game->getPlayer(nextPlayerId).getCards().size();

    for (unsigned int j = 0; j < numberOfCards; j++)
    {
        drawCardTop(Position{-0.5 + static_cast<double>(j) * CARD_SPACE, 0.93});
    }

    nextPlayerId = (nextPlayerId + 1) % numberOfPlayers;
    numberOfCards = game->getPlayer(nextPlayerId).getCards().size();

    for (unsigned int j = 0; j < numberOfCards; j++)
    {
        drawCardTopHorizontal(Position{0.3, 0.0 + static_cast<double>(j) * CARD_SPACE});
    }
}

void GameUI::drawTableCards()
{
    const Cards& cards = game->getCardsFromTableTop();
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
                      0.68,
                      0.1);

    for (int i = 0; i < numberOfPlayers; i++)
    {
        std::string text =
            "Player " + std::to_string(i + 1) + ": " +
            std::to_string(game->getPlayer(i).getPeasantLevel());
        SDL_GL_RenderText(text.c_str(),
                          font,
                          textColor,
                          0.75,
                          0.61 - i * 0.06,
                          0.1);
    }
}

void GameUI::drawPastTurnsInfo()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 40));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Past turns:",
                      font,
                      textColor,
                      0.75,
                      0.2,
                      0.1);

    const History& history = game->getHistory();

    for (int i = 0; i < numberOfPlayers and i < static_cast<int>(history.getHistory().size()); i++)
    {
        HistoryElement currentHistory = history.getHistory()[history.getHistory().size() - i - 1];
        std::string text =
            "Player " + std::to_string(currentHistory.playerId + 1) + ": ";

        double smallFactor = 2.31;
        if (currentHistory.action == "PASS TURN")
        {
            SDL_GL_RenderText("PASS",
                              font,
                              textColor,
                              0.85,
                              0.09 - i * (CARD_HEIGHT/smallFactor),
                              0.1);
        }
        else
        {
            for (unsigned int j = 0; j < currentHistory.cards.size(); j++)
            {
                drawCard(currentHistory.cards[j],
                         Position{0.78 + static_cast<double>(j) * CARD_SPACE/smallFactor, 0.13 - i * (CARD_HEIGHT/smallFactor)},
                         CARD_WIDTH/smallFactor, CARD_HEIGHT/smallFactor);
            }
        }

        SDL_GL_RenderText(text.c_str(),
                          font,
                          textColor,
                          0.65,
                          0.09 - i * (CARD_HEIGHT/smallFactor),
                          0.1);
    }
}

void GameUI::updateCardsSelection(int x, int y)
{
    Position glPosition = {static_cast<double>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                           static_cast<double>(-y) * 2.0 / SCREEN_HEIGHT + 1.0};

    Cards cards = getCurrentPlayer().getCards();
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
        getCurrentPlayer().unselectCard(cardIndex);
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
        getCurrentPlayer().selectCard(cardIndex);
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
    drawBackground();
    drawButtonPanel();
    drawPeasantsInfo();
    drawPastTurnsInfo();
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
    std::string text = "Player " + std::to_string(getCurrentPlayerId() + 1) + " turn";;
    if (isCurrentPlayerAI()) text = "AI " + text;
    if (not isGameOneHumanOnly) drawPopup(text.c_str());
}

bool GameUI::isCurrentPlayerAI()
{
    return settings.playerTypes[getCurrentPlayerId()] == PlayerType_AI;
}

void GameUI::turnOnCardsExchange()
{
    cardsExchangeActive = true;
    for (auto& button : buttons)
    {
        switch (button.getButtonId())
        {
            case ButtonId_PassTurn:
            {
                button.isVisible = false;
                break;
            }
            case ButtonId_ThrowCards:
            {
                button.isVisible = false;
                break;
            }
            case ButtonId_GiveAway:
            {
                button.isVisible = true;
                break;
            }
        }
    }
    forceDrawingEverything();
}

void GameUI::turnOffCardsExchange()
{
    cardsExchangeActive = false;
    for (auto& button : buttons)
    {
        switch (button.getButtonId())
        {
            case ButtonId_PassTurn:
            {
                button.isVisible = true;
                break;
            }
            case ButtonId_ThrowCards:
            {
                button.isVisible = true;
                break;
            }
            case ButtonId_GiveAway:
            {
                button.isVisible = false;
                break;
            }
        }
    }
    forceDrawingEverything();
}

void GameUI::handleUIPartOfCardsExchange()
{
    if (not isGameAIOnly and isHumanAMaster())
    {
        turnOnCardsExchange();
    }
    for (int id = 0; id < numberOfPlayers; id++)
    {
        if (settings.playerTypes[id] == PlayerType_Human and
            game->getPlayer(id).getPeasantLevel() > 0)
        {
            exchangePlayersIds.push_back(id);
        }
    }
}

void GameUI::enteringAction()
{
    calculateIsAIOnlyGame();
    calculateIsGameOneHumanOnly();
    for (unsigned int i = 0; i < numberOfPlayers; i++)
    {
        if (settings.playerTypes[i] == PlayerType_Network)
        {
            netServer.sendStringToClient("GAME STARTED", settings.clientId[i]);
            sendGameInfoToNetworkPlayer(settings.clientId[i]);
        }
    }
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

void GameUI::sendGameInfoToNetworkPlayer(unsigned int clientId)
{
    netServer.sendStringToClient("GAME INFO", clientId);
    netServer.sendStringToClient(std::to_string(numberOfPlayers), clientId);
}

namespace
{

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

void GameUI::createGameResults()
{
    const std::vector<LevelsHistory>& levelsHistory = game->getLevelsHistory();
    scores.clear();

    for (int i = 0; i < numberOfPlayers; i++)
    {
        int numberOfMasterTimes = calculateNumberOfMasterTimes(levelsHistory[i]);
        int positiveScore = calculatePositiveScoreAsMaster(levelsHistory[i]);
        scores.push_back(Score{numberOfMasterTimes, positiveScore, i});
    }
    std::sort(scores.begin(), scores.end());
    std::reverse(scores.begin(), scores.end());
}

int GameUI::getCurrentPlayerId()
{
    int playerId = game->getCurrentPlayer().getId();
    if (cardsExchangeActive)
    {
        playerId = exchangePlayersIds[0];
    }
    return playerId;
}

Player& GameUI::getCurrentPlayer()
{
    int playerId = getCurrentPlayerId();
    return game->getPlayer(playerId);
}

bool GameUI::isHumanAMaster()
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (game->getPlayer(i).getPeasantLevel() > 0 and
            settings.playerTypes[i] == PlayerType_Human) return true;
    }
    return false;
}

void GameUI::processEndOfRound()
{
    game->nextRound();
    handleUIPartOfCardsExchange();
}

Scores GameUI::getGameResults()
{
    return scores;
}
