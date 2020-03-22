#ifndef POLLING_PLACE_HPP
#define POLLING_PLACE_HPP

#include "button.hpp"
#include "polling_place_id.hpp"
#include <vector>

class PollingPlace
{
public:
    PollingPlace();
    virtual ~PollingPlace() = default;
    PollingPlaceId enter();
protected:
    virtual PollingPlaceId startEventPoll() = 0;
    virtual void updateScreen() = 0;
    void updateButtonsOnMotion(int x, int y);
    void updateButtonsClickStatus();
    virtual void drawBackground() = 0;
    virtual void forceDrawingEverything();
    virtual void enteringAction();

    std::vector<Button> buttons;
    SDL_Event event;
    PollingPlaceId ownId;
    bool backgroundNeedsDrawing;
    int lastTicks;
};

#endif // POLLING_PLACE_HPP
