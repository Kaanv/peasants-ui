#include "polling_place.hpp"
#include "constants.hpp"

PollingPlaceId PollingPlace::enter()
{
    updateScreen();
    PollingPlaceId currentPlace = ownId;
    while (currentPlace == ownId)
    {
        currentPlace = startEventPoll();
    }
    return currentPlace;
}

void PollingPlace::updateButtonsOnMotion(int x, int y)
{
    for (auto& button : buttons)
    {
        button.updateMotion({static_cast<float>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                             static_cast<float>(-y) * 2.0 / SCREEN_HEIGHT + 1.0});
    }
    updateScreen();
}

void PollingPlace::updateButtonsClickStatus()
{
    for (auto& button : buttons)
    {
        button.updateUnderClick();
    }
}
