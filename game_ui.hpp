#ifndef GAME_UI_HPP
#define GAME_UI_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "settings.hpp"
#include "Game.hpp"
#include <vector>
#include <map>

const double CARD_WIDTH = 0.17;
const double CARD_HEIGHT = 0.3;
const double CARD_SPACE = 0.06;
const double CARD_SELECT_HEIGHT = 0.05;

class GameUI : public PollingPlace
{
public:
    GameUI();
    void setSettings(Settings settings);
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
    void takeCardsFromPeasants();
    void turnOnCardsExchange();
    void turnOffCardsExchange();
    void giveCardsToPeasants();
    void exchangePlayersCards();
    void enteringAction() override;
    bool isCurrentPlayerAI();
    void calculateIsAIOnlyGame();
    void calculateIsGameOneHumanOnly();
    void presentAIGameResults();
    int getCurrentPlayerId();
    Player& getCurrentPlayer();
    bool isHumanAMaster();

    Settings settings;
    int numberOfPlayers;
    Game game;
    std::map<std::pair<Color, Value>, GLuint> textureMap;
    GLuint cardTopTexture;
    int lastTicks;
    bool backgroundNeedsDrawing;
    bool isPopupActive;
    bool isGameAIOnly;
    bool isGameOneHumanOnly;
    int humanPlayer;
    bool cardsExchangeActive = false;
    bool drawExchangePopup = true;
    std::vector<int> exchangePlayersIds;
};

#endif // GAME_UI_HPP
