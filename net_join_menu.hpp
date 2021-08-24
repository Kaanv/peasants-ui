#ifndef NET_JOIN_MENU
#define NET_JOIN_MENU

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "network_client.hpp"
#include <vector>

class NetJoinMenu : public PollingPlace
{
public:
    NetJoinMenu(NetworkClient& netClient_);
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    void drawTitle();

    NetworkClient& netClient;
    bool enteringIp = false;
};

#endif // NET_JOIN_MENU
