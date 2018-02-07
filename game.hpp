#ifndef GAME_HPP
#define GAME_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "sdl_gl_wrapper.hpp"
#include <vector>

class Game
{
public:
    Game();
    PollingPlaceId enter();
private:
    PollingPlaceId startEventPoll();
    void updateScreen();
    void drawBackground();
    void drawButtonPanel();
    void updateButtonsClickStatus();
    void updateButtonsOnMotion(int x, int y);

    std::vector<Button> buttons;
    SDL_Event event;
};

#endif // GAME_HPP
