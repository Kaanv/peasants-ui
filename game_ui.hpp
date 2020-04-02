#pragma once

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "settings.hpp"
#include "Game.hpp"
#include "score.hpp"
#include <vector>
#include <map>
#include <memory>

const double CARD_WIDTH = 0.17;
const double CARD_HEIGHT = 0.3;
const double CARD_SPACE = 0.06;
const double CARD_SELECT_HEIGHT = 0.05;

class GameUI : public PollingPlace
{
public:
    GameUI();
    void setSettings(Settings settings);
    Scores getGameResults();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawButtonPanel();
    void drawCard(Card card, Position position,
                  double width=CARD_WIDTH, double height=CARD_HEIGHT);
    void drawCardTop(Position position);
    void drawCardTopHorizontal(Position position);
    void drawCards();
    void drawCurrentPlayerCards();
    void drawAnotherPlayerCards();
    void drawTableCards();
    void drawPeasantsInfo();
    void drawPastTurnsInfo();
    void updateCardsSelection(int x, int y);
    void forceDrawingEverything() override;
    void updateSelectedCardSelection(Position glPosition,
                                     const Cards& cards,
                                     unsigned int cardIndex);
    void updateNotSelectedCardSelection(Position glPosition,
                                        const Cards& cards,
                                        unsigned int cardIndex);
    void drawPopup(std::string text);
    void forceDrawButtons();
    void drawCurrentPlayerPopup();
    void turnOnCardsExchange();
    void turnOffCardsExchange();
    void enteringAction() override;
    bool isCurrentPlayerAI();
    void calculateIsAIOnlyGame();
    void calculateIsGameOneHumanOnly();
    void createGameResults();
    int getCurrentPlayerId();
    Player &getCurrentPlayer();
    bool isHumanAMaster();
    void processEndOfRound();
    void handleUIPartOfCardsExchange();

    Settings settings;
    unsigned int numberOfPlayers;
    std::unique_ptr<Game> game;
    std::map<std::pair<Color, Value>, GLuint> textureMap;
    GLuint cardTopTexture;
    bool isPopupActive;
    bool isGameAIOnly;
    bool isGameOneHumanOnly;
    unsigned int humanPlayer;
    bool cardsExchangeActive = false;
    bool drawExchangePopup = true;
    std::vector<unsigned int> exchangePlayersIds;
    Scores scores;
};
