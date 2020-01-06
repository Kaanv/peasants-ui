#ifndef NET_CREATE_MENU
#define NET_CREATE_MENU

#include "settings_menu.hpp"

class NetCreateMenu : public SettingsMenu
{
    PollingPlaceId startEventPoll() override;
};

#endif // NET_CREATE_MENU
