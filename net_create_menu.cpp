#include "net_create_menu.hpp"

NetCreateMenu::NetCreateMenu()
{

}

PollingPlaceId NetCreateMenu::startEventPoll()
{
    PollingPlaceId result = SettingsMenu::startEventPoll();

    if (result != PollingPlaceId_Settings) return result;
    return PollingPlaceId_NetworkGameCreating;
}
