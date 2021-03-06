#include "polling_place.hpp"
#include "constants.hpp"

PollingPlace::PollingPlace() : backgroundNeedsDrawing(true),
                               lastTicks(0)
{}

PollingPlaceId PollingPlace::enter()
{
    enteringAction();
    forceDrawingEverything();
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
        button.updateMotion({static_cast<double>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                             static_cast<double>(-y) * 2.0 / SCREEN_HEIGHT + 1.0});
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

void PollingPlace::forceDrawingEverything()
{
    backgroundNeedsDrawing = true;
    for (auto& button : buttons)
    {
        button.forceDraw();
    }
}

void PollingPlace::enteringAction()
{
}
