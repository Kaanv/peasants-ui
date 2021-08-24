#include "client_ui.hpp"
#include "constants.hpp"
#include "textures.hpp"
#include "text.hpp"
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

ClientUI::ClientUI(NetworkClient& _netClient) : netClient(_netClient)
{
    bool notVisible = false;
    ownId = PollingPlaceId_ClientGame;

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
}

PollingPlaceId ClientUI::startEventPoll()
{
    std::string lastMessage = netClient.getLastMessageFromServer();
    /**
     * Structure of game info message:
     * 0. GAME_INFO
     * 1. number of players
     * 2. player cards
     * 3. player id
     * 4. table cards
     * 5. numbers of players cards
     * 6. peasants info
     * 7. turn history
     */

    if (lastMessage.size() > 0)
    {
        std::vector<std::string> results;
        boost::split(results, lastMessage, [](char c){return c == ';';});

        if (results[0] == "GAME_INFO")
        {
            std::cout << "RECEIVED GAME INFO!" << std::endl;
            numbersOfPlayersCards.clear();
            numberOfPlayers = static_cast<unsigned int>(std::stoi(results[1]));
            updateCards(results[2]);
            updateTableCards(results[4]);
            std::vector<std::string> numbersOfPlayersCardsString;
            boost::split(numbersOfPlayersCardsString, results[5], [](char c){return c == ',';});
            for (unsigned int i = 0; i < numbersOfPlayersCardsString.size(); i++)
            {
                numbersOfPlayersCards.push_back(static_cast<unsigned int>(std::stoi(numbersOfPlayersCardsString[i])));
            }

            peasantsInfo.clear();
            std::vector<std::string> peasantsInfoString;
            boost::split(peasantsInfoString, results[6], [](char c){return c == ',';});
            for (unsigned int i = 0; i < peasantsInfoString.size(); i++)
            {
                peasantsInfo.push_back(static_cast<int>(std::stoi(peasantsInfoString[i])));
            }

            turnsInfo = results[7];

            forceDrawingEverything();
            updateScreen();
        }
        else if (results[0] == "CARDS_EXCHANGE")
        {
            drawExchangePopup = true;
            cardsExchangeActive = true;
        }
    }

    if (cardsExchangeActive and drawExchangePopup)
    {
        std::string text =
            "Give cards to your peasant";
        drawPopup(text.c_str());
        drawExchangePopup = false;
    }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlaceId_Exit;
        else if (event.type == SDL_WINDOWEVENT_ENTER or
                 event.type == SDL_WINDOWEVENT_SHOWN or
                 event.type == SDL_WINDOWEVENT_EXPOSED or
                 event.type == SDL_WINDOW_INPUT_FOCUS)
        {
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
                        case ButtonId_CancelGame: return PollingPlaceId_NetworkGameJoining;
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                        case ButtonId_PassTurn:
                        {
                            netClient.sendMessage("PASS_TURN");
                            forceDrawingEverything();
                            break;
                        }
                        case ButtonId_ThrowCards:
                        {
                            std::string cardIndexes = "";

                            for (unsigned int i = 0; i < clientCards.size(); i++)
                            {
                                if (clientCards[i].selected)
                                {
                                    cardIndexes += std::to_string(i);
                                    if (i < (clientCards.size() - 1)) cardIndexes += ";";
                                }
                            }

                            netClient.sendMessage("THROW_CARDS;" + cardIndexes);
                            forceDrawingEverything();
                            break;
                        }
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
        drawPeasantsInfo();
        drawPastTurnsInfo();
    }

    for (auto& button : buttons)
    {
        if (button.getButtonId() != ButtonId_PopupOk) button.draw();
    }

    SDL_GL_SwapWindow(getScreen());
    lastTicks = SDL_GetTicks();
    backgroundNeedsDrawing = false;
}

void ClientUI::drawPeasantsInfo()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 40));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Peasant levels:",
                      font,
                      textColor,
                      0.75,
                      0.68,
                      0.1);

    for (unsigned int i = 0; i < peasantsInfo.size(); i++)
    {
        std::string text =
            "Player " + std::to_string(i + 1) + ": " +
            std::to_string(peasantsInfo[i]);
        SDL_GL_RenderText(text.c_str(),
                          font,
                          textColor,
                          0.75,
                          0.61 - i * 0.06,
                          0.1);
    }
}

void ClientUI::forceDrawingEverything()
{
    backgroundNeedsDrawing = true;
    for (auto& button : buttons)
    {
        button.forceDraw();
    }
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
    if (numbersOfPlayersCards.size()) drawAnotherPlayersCards(numbersOfPlayersCards);
}

void ClientUI::drawTableCards()
{
    for (unsigned int i = 0; i < tableCards.size(); i++)
    {
        drawCard(tableCards[i], Position{-0.4 + static_cast<double>(i) * CARD_SPACE, 0.2});
    }
}

void ClientUI::getGameInfoFromServer()
{
    netClient.sendMessage("GET_GAME_INFO");
}

void ClientUI::updateCards(std::string cardsInfo)
{
    Cards newClientCards = convertStringToCards(cardsInfo);

    if (newClientCards.size() == clientCards.size())
    {
        for (unsigned int i = 0; i < clientCards.size(); i++)
        {
            if (clientCards[i] != newClientCards[i])
            {
                clientCards.clear();
                clientCards = std::move(newClientCards);
                return;
            }
        }
        return;
    }

    clientCards.clear();
    clientCards = std::move(newClientCards);
}

void ClientUI::updateTableCards(std::string cardsInfo)
{
    tableCards.clear();
    for (unsigned int i = 1; i < cardsInfo.size(); i += 2)
    {
        Card card = convertCharsToCard(cardsInfo[i - 1], cardsInfo[i]);
        tableCards.push_back(card);
    }
}

void ClientUI::updateCardsSelection(int x, int y)
{
    Position glPosition = {static_cast<double>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                           static_cast<double>(-y) * 2.0 / SCREEN_HEIGHT + 1.0};

    for (unsigned int i = 0; i < clientCards.size(); i++)
    {
        if (clientCards[i].selected) updateSelectedCardSelection(glPosition,
                                                                 clientCards,
                                                                 i);
        else updateNotSelectedCardSelection(glPosition,
                                            clientCards,
                                            i);
    }
}

void ClientUI::drawPastTurnsInfo()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 40));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Past turns:",
                      font,
                      textColor,
                      0.75,
                      0.2,
                      0.1);

    std::vector<std::string> playerTurnsStrings;
    boost::split(playerTurnsStrings, turnsInfo, [](char c){return c == ',';});

    if (playerTurnsStrings.size() > 1)
    for (unsigned int i = 0; i < playerTurnsStrings.size(); i += 2)
    {
        std::string text =
            "Player " + std::to_string(std::stoi(playerTurnsStrings[i]) + 1) + ": ";

        double smallFactor = 2.31;
        if (playerTurnsStrings[i + 1] == "PASS_TURN")
        {
            SDL_GL_RenderText("PASS",
                              font,
                              textColor,
                              0.85,
                              0.09 - static_cast<int>(i/2) * (CARD_HEIGHT/smallFactor),
                              0.1);
        }
        else
        {
            Cards cards = convertStringToCards(playerTurnsStrings[i + 1]);

            for (unsigned int j = 0; j < cards.size(); j++)
            {
                drawCard(cards[j],
                         Position{0.78 + static_cast<double>(j) * CARD_SPACE/smallFactor, 0.13 - static_cast<int>(i/2) * (CARD_HEIGHT/smallFactor) + 0.05},
                         CARD_WIDTH/smallFactor, CARD_HEIGHT/smallFactor);
            }
        }

        SDL_GL_RenderText(text.c_str(),
                          font,
                          textColor,
                          0.65,
                          0.09 - static_cast<int>(i/2) * (CARD_HEIGHT/smallFactor),
                          0.1);
    }
}
