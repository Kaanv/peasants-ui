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
#include "network_client.hpp"

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
    std::vector<unsigned int> createNumbersOfPlayersCards();
    void drawAnotherPlayerCards() override;
    void drawTableCards() override;
    void drawPeasantsInfo() override;
    void drawPastTurnsInfo() override;
    void updateCardsSelection(int x, int y);
    void forceDrawingEverything() override;
    void drawCurrentPlayerPopup();
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
    void sendGameInfoToAllNetworkPlayers();
    void sendCardsExchangeAllNetworkPlayers();
    void passCurrentPlayerTurn();
    std::string getTurnHistoryAsString();
    void handleClientsMessages();

    Settings settings;
    unsigned int numberOfPlayers;
    std::unique_ptr<Game> game;
    bool isGameAIOnly;
    bool isGameOneHumanOnly;
    unsigned int humanPlayer;
    std::vector<unsigned int> exchangePlayersIds;
    Scores scores;
    NetworkServer& netServer;
};
