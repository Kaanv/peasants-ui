#include "net_create_menu.hpp"

NetCreateMenu::NetCreateMenu(NetworkServer& _netServer) : netServer(_netServer)
{

}

PollingPlaceId NetCreateMenu::startEventPoll()
{
    netServer.startServer();
    PollingPlaceId result = SettingsMenu::startEventPoll();

    if (result != PollingPlaceId_Settings) return result;
    return PollingPlaceId_NetworkGameCreating;
}
