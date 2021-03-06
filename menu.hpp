#ifndef MENU_HPP
#define MENU_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include <vector>

class MainMenu : public PollingPlace
{
public:
    MainMenu();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawTitle();
};

#endif // MENU_HPP
