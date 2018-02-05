#ifndef MENU_HPP
#define MENU_HPP

#include "button.hpp"
#include "polling_place.hpp"
#include "sdl_gl_wrapper.hpp"
#include <vector>

class Menu
{
public:
    virtual PollingPlace enter() = 0;
};

class MainMenu : public Menu
{
public:
    MainMenu(int x, int y);
    PollingPlace enter();
private:
    PollingPlace startEventPoll();
    void updateScreen();
    void updateButtonsOnMotion(int x, int y);
    void updateButtonsClickStatus();

    std::vector<Button> buttons;
    SDL_Event event;
    int x, y;
};

#endif // MENU_HPP
