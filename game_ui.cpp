#include "game_ui.hpp"
#include "constants.hpp"
#include "textures.hpp"
#include "text.hpp"
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

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
    for (unsigned int clientIndex = 0; clientIndex < 5; clientIndex++)
    {
        std::string lastMessage = netServer.getLastMessageFromClient(clientIndex);
        if (lastMessage == "GET_GAME_INFO")
        {
            sendGameInfoToNetworkPlayer(clientIndex);
        }
    }

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

void GameUI::drawCurrentPlayerCards()
{
    unsigned int playerId = game->getCurrentPlayer().getId();
    if (isGameOneHumanOnly) playerId = humanPlayer;
    if (cardsExchangeActive)
    {
        playerId = exchangePlayersIds[0];
    }
    Cards& cards = game->getPlayer(playerId).getCards();
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        drawCard(cards[i], Position{-0.5 + static_cast<double>(i) * CARD_SPACE, -0.6});
    }
}

void GameUI::drawAnotherPlayerCards()
{
    unsigned int currentPlayerId = getCurrentPlayerId();
    if (isGameOneHumanOnly) currentPlayerId = humanPlayer;

    std::vector<unsigned int> numbersOfPlayersCards;

    for (unsigned int i = 1; i < numberOfPlayers; i++)
    {
        numbersOfPlayersCards.push_back(
            static_cast<unsigned int>(game->getPlayer((currentPlayerId + i) % numberOfPlayers).getCards().size()));
    }

    drawAnotherPlayersCards(numbersOfPlayersCards);
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

    for (unsigned int i = 0; i < numberOfPlayers; i++)
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

    for (unsigned int i = 0; i < numberOfPlayers and i < static_cast<unsigned int>(history.getHistory().size()); i++)
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

    Cards& cards = getCurrentPlayer().getCards();
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

    glColor3d(0.0, 0.0, 0.8);
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
    std::string text = "Player " + std::to_string(getCurrentPlayerId() + 1) + " turn";
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
    for (unsigned int id = 0; id < numberOfPlayers; id++)
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
            netServer.sendStringToClient("GAME_STARTED", settings.clientId[i]);
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

namespace
{

char convertValueToChar(Value value)
{
    if (value == three) return '3';
    else if (value == four) return '4';
    else if (value == five) return '5';
    else if (value == six) return '6';
    else if (value == seven) return '7';
    else if (value == eight) return '8';
    else if (value == nine) return '9';
    else if (value == ten) return '0';
    else if (value == jack) return 'j';
    else if (value == queen) return 'q';
    else if (value == king) return 'k';
    else if (value == ace) return 'a';
    return 'x';
}

Value convertCharToValue(const char value)
{
    if (value == '3') return three;
    else if (value == '4') return four;
    else if (value == '5') return five;
    else if (value == '6') return six;
    else if (value == '7') return seven;
    else if (value == '8') return eight;
    else if (value == '9') return nine;
    else if (value == '0') return ten;
    else if (value == 'j') return jack;
    else if (value == 'q') return queen;
    else if (value == 'k') return king;
    else if (value == 'a') return ace;
    return three;
}

char convertColorToChar(Color color)
{
    if (color == hearts) return 'h';
    else if (color == diamonds) return 'd';
    else if (color == clubs) return 'c';
    else if (color == spades) return 's';
    return 'x';
}

Color convertCharToColor(const char color)
{
    if (color == 'h') return hearts;
    else if (color == 'd') return diamonds;
    else if (color == 'c') return clubs;
    else if (color == 's') return spades;
    return spades;
}

std::string convertCardsToString(const Cards& cards)
{
    std::string result = "";

    for (const auto& card : cards)
    {
        result += convertValueToChar(card.value);
        result += convertColorToChar(card.color);
    }

    return result;
}

Card convertCharsToCard(const char value, const char color)
{
    return Card(convertCharToValue(value), convertCharToColor(color));
}

}

void GameUI::sendGameInfoToNetworkPlayer(unsigned int clientId)
{
    netServer.sendStringToClient("GAME_INFO;" + std::to_string(numberOfPlayers) + ";"
                                              + convertCardsToString(game->getPlayer(0).getCards()) + ";", // TODO: Player index instead of 0
                                 clientId);
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

    for (unsigned int i = 0; i < numberOfPlayers; i++)
    {
        int numberOfMasterTimes = calculateNumberOfMasterTimes(levelsHistory[i]);
        int positiveScore = calculatePositiveScoreAsMaster(levelsHistory[i]);
        scores.push_back(Score{numberOfMasterTimes, positiveScore, i});
    }
    std::sort(scores.begin(), scores.end());
    std::reverse(scores.begin(), scores.end());
}

unsigned int GameUI::getCurrentPlayerId()
{
    unsigned int playerId = game->getCurrentPlayer().getId();
    if (cardsExchangeActive)
    {
        playerId = exchangePlayersIds[0];
    }
    return playerId;
}

Player& GameUI::getCurrentPlayer()
{
    unsigned int playerId = getCurrentPlayerId();
    return game->getPlayer(playerId);
}

bool GameUI::isHumanAMaster()
{
    for (unsigned int i = 0; i < numberOfPlayers; i++)
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

ClientUI::ClientUI(NetworkClient& _netClient) : netClient(_netClient)
{
    ownId = PollingPlaceId_ClientGame;
}

PollingPlaceId ClientUI::startEventPoll()
{
    std::string lastMessage = netClient.getLastMessageFromServer();

    if (lastMessage.size() > 0)
    {
        std::vector<std::string> results;
        boost::split(results, lastMessage, [](char c){return c == ';';});

        if (results[0] == "GAME_INFO")
        {
            std::cout << "RECEIVED GAME INFO!" << std::endl;
            updateCards(results[2]);
        }
    }

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
                        case ButtonId_CancelGame: return PollingPlaceId_NetworkGameJoining;
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                    }
                }
            }
        }
    }
    return PollingPlaceId_ClientGame;
}

void ClientUI::updateScreen()
{
    if (backgroundNeedsDrawing)
    {
        drawBackground();
        drawButtonPanel();
        drawCards();
//        drawPeasantsInfo();
//        drawPastTurnsInfo();
    }

    for (auto& button : buttons)
    {
        if (button.getButtonId() != ButtonId_PopupOk) button.draw();
    }

    SDL_GL_SwapBuffers();
    lastTicks = SDL_GetTicks();
    backgroundNeedsDrawing = false;
}

void ClientUI::forceDrawingEverything()
{

}

void ClientUI::enteringAction()
{
    if (numberOfPlayers == 0)
    {
        getGameInfoFromServer();
    }
}

void ClientUI::drawCurrentPlayerCards()
{
    for (unsigned int i = 0; i < clientCards.size(); i++)
    {
        drawCard(clientCards[i], Position{-0.5 + static_cast<double>(i) * CARD_SPACE, -0.6});
    }
}

void ClientUI::drawAnotherPlayerCards()
{

}

void ClientUI::drawTableCards()
{

}

void ClientUI::getGameInfoFromServer()
{
    netClient.sendMessage("GET_GAME_INFO");
}

void ClientUI::updateCards(std::string cardsInfo)
{
    clientCards.clear();
    for (unsigned int i = 1; i < cardsInfo.size(); i += 2)
    {
        Card card = convertCharsToCard(cardsInfo[i - 1], cardsInfo[i]);
        clientCards.push_back(card);
    }
}
