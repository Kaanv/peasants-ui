#ifndef GAME_UI_HPP
#define GAME_UI_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "Game.hpp"
#include <vector>
#include <map>

class GameUI : public PollingPlace
{
public:
    GameUI();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawButtonPanel();
    void drawCard(Card card, Position position);
    void drawCards();
    void drawCurrentPlayerCards();
    void drawTableCards();
    void drawPeasantsInfo();
    void updateCardsSelection(int x, int y);
    void forceDrawingEverything();

    int numberOfPlayers;
    Game game;
    std::map<std::pair<Color, Value>, GLuint> textureMap;
    int lastTicks;
    bool backgroundNeedsDrawing;
};

#endif // GAME_UI_HPP
