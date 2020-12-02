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

class ClientUI : public BaseUI
{
public:
    ClientUI(NetworkClient&);
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void forceDrawingEverything() override;
    void enteringAction() override;
    void drawCurrentPlayerCards() override;
    void drawAnotherPlayerCards() override;
    void drawTableCards() override;
    void drawPeasantsInfo() override;
    void drawPastTurnsInfo() override;
    void updateCards(std::string cardsInfo);
    void updateTableCards(std::string cardsInfo);
    void updateCardsSelection(int x, int y);

    void getGameInfoFromServer();

    Cards lastMove;
    unsigned int numberOfPlayers = 0;
    int myPlayerId;
    std::vector<unsigned int> numbersOfPlayersCards;
    std::vector<int> peasantsInfo;
    Scores scores;
    Cards clientCards;
    Cards tableCards;
    NetworkClient& netClient;
    std::string turnsInfo;
};
