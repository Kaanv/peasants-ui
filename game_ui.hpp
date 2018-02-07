#ifndef GAME_HPP
#define GAME_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
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
};

#endif // GAME_HPP
