#include "resolution.hpp"
#include "polling_place_id.hpp"
#include "menu.hpp"
#include "game_ui.hpp"
#include "sdl_gl_wrapper.hpp"

int main()
{
    Resolution resolution{800, 600};
    SDL_Surface *screen = init(resolution.x, resolution.y);
    PollingPlaceId currentPlace = PollingPlaceId_MainMenu;
    MainMenu mainMenu(10, 10);
    GameUI game;

    while (currentPlace != PollingPlaceId_Exit)
    {
        switch (currentPlace)
        {
        case PollingPlaceId_Game:
            currentPlace = game.enter();
            break;
        case PollingPlaceId_MainMenu:
        default:
            currentPlace = mainMenu.enter();
        }
    }

    delete screen;
    return 0;
}

