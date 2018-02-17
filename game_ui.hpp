#ifndef GAME_UI_HPP
#define GAME_UI_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "Game.hpp"
#include <vector>

class GameUI : public PollingPlace
{
public:
    GameUI();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawButtonPanel();
    void drawCards();
    void drawCurrentPlayerCards();

    int numberOfPlayers;
    Game game;
};

#endif // GAME_UI_HPP
