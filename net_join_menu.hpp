#ifndef NET_JOIN_MENU
#define NET_JOIN_MENU

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include <vector>

class NetJoinMenu : public PollingPlace
{
public:
    NetJoinMenu();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawTitle();
};

#endif // NET_JOIN_MENU
