#pragma once

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "settings.hpp"
#include "Game.hpp"
#include "score.hpp"
#include "network_server.hpp"
#include <vector>
#include <map>
#include <memory>
#include "base_ui.hpp"

class ClientUI : public BaseUI
{
public:
    ClientUI();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void forceDrawingEverything() override;
    void enteringAction() override;
    void drawCurrentPlayerCards() override;
    void drawAnotherPlayerCards() override;
    void drawTableCards() override;

    Cards lastMove;
    unsigned int numberOfPlayers;
    Scores scores;
    Cards clientCards;
};

class GameUI : public BaseUI
{
public:
    GameUI(NetworkServer&);
    void setSettings(Settings settings);
    Scores getGameResults();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void enteringAction() override;
    void drawCurrentPlayerCards() override;
    void drawAnotherPlayerCards() override;
    void drawTableCards() override;
    void drawPeasantsInfo();
    void drawPastTurnsInfo();
    void updateCardsSelection(int x, int y);
    void forceDrawingEverything() override;
    void drawPopup(std::string text);
    void forceDrawButtons();
    void drawCurrentPlayerPopup();
    void turnOnCardsExchange();
    void turnOffCardsExchange();
    bool isCurrentPlayerAI();
    void calculateIsAIOnlyGame();
    void calculateIsGameOneHumanOnly();
    void createGameResults();
    unsigned int getCurrentPlayerId();
    Player &getCurrentPlayer();
    bool isHumanAMaster();
    void processEndOfRound();
    void handleUIPartOfCardsExchange();
    void sendGameInfoToNetworkPlayer(unsigned int clientId);

    Settings settings;
    unsigned int numberOfPlayers;
    std::unique_ptr<Game> game;
    bool isPopupActive;
    bool isGameAIOnly;
    bool isGameOneHumanOnly;
    unsigned int humanPlayer;
    bool cardsExchangeActive = false;
    bool drawExchangePopup = true;
    std::vector<unsigned int> exchangePlayersIds;
    Scores scores;
    NetworkServer& netServer;
};
