#ifndef NET_CREATE_MENU
#define NET_CREATE_MENU

#include "settings_menu.hpp"
#include "network_server.hpp"

class NetCreateMenu : public SettingsMenu
{
public:
    NetCreateMenu(NetworkServer&);
private:
    PollingPlaceId startEventPoll() override;
    NetworkServer& netServer;
};

#endif // NET_CREATE_MENU
