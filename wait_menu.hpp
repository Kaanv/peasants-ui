#ifndef WAIT_MENU
#define WAIT_MENU

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include <vector>

class WaitMenu : public PollingPlace
{
public:
    WaitMenu();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawTitle();
};

#endif // WAIT_MENU
