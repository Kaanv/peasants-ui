#include "net_create_menu.hpp"
#include <iostream>

NetCreateMenu::NetCreateMenu(Settings& _settings,
                             NetworkServer& _netServer) : SettingsMenu(_settings), netServer(_netServer)
{

}

PollingPlaceId NetCreateMenu::startEventPoll()
{
    netServer.startServer();
    netServer.sendStringToClient("HI, IM SERVER");
    std::cout << "SENT HI MESSAGE!" << std::endl;
    SDL_Delay(100);
    PollingPlaceId result = SettingsMenu::startEventPoll();

    if (result != PollingPlaceId_Settings) return result;
    return PollingPlaceId_NetworkGameCreating;
}
