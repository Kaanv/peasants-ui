#include "net_create_menu.hpp"

PollingPlaceId NetCreateMenu::startEventPoll()
{
    PollingPlaceId result = SettingsMenu::startEventPoll();

    if (result != PollingPlaceId_Settings) return result;
    return PollingPlaceId_NetworkGameCreating;
}
