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

const double CARD_WIDTH = 0.17;
const double CARD_HEIGHT = 0.3;
const double CARD_SPACE = 0.06;
const double CARD_SELECT_HEIGHT = 0.05;

class BaseUI : public PollingPlace
{
public:
    BaseUI();
protected:
    void drawBackground() override;
    void drawCard(Card card,
                  Position position,
                  double width=CARD_WIDTH,
                  double height=CARD_HEIGHT);
    void drawCardTop(Position position);
    void drawCardTopHorizontal(Position position);
    void drawButtonPanel();
    void updateSelectedCardSelection(Position glPosition,
                                     Cards& cards,
                                     unsigned int cardIndex);
    void updateNotSelectedCardSelection(Position glPosition,
                                        Cards& cards,
                                        unsigned int cardIndex);

private:
    std::map<std::pair<Color, Value>, GLuint> textureMap;
    GLuint cardTopTexture;
};

class ClientUI : public BaseUI
{
public:
    ClientUI();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void forceDrawingEverything() override;
    void enteringAction() override;

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
    void drawCards();
    void drawCurrentPlayerCards();
    void drawAnotherPlayerCards();
    void drawTableCards();
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
