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
    void drawCard(Card card, Position position);
    void drawCardTop(Position position);
    void drawCardTopHorizontal(Position position);
    void drawCards();
    void drawCurrentPlayerCards();
    void drawAnotherPlayerCards();
    void drawTableCards();
    void drawPeasantsInfo();
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
    void exchangePlayersCards();

    Settings settings;
    int numberOfPlayers;
    Game game;
    std::map<std::pair<Color, Value>, GLuint> textureMap;
    GLuint cardTopTexture;
    int lastTicks;
    bool backgroundNeedsDrawing;
    bool isPopupActive;
};

#endif // GAME_UI_HPP
