#pragma once

#include "settings_menu.hpp"
#include "network_server.hpp"

class NetCreateMenu : public SettingsMenu
{
public:
    NetCreateMenu(Settings&, NetworkServer&);
private:
    PollingPlaceId startEventPoll() override;
    NetworkServer& netServer;
};
