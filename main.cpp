#include "resolution.hpp"
#include "polling_place.hpp"
#include "menu.hpp"
#include "sdl_gl_wrapper.hpp"

int main()
{
    Resolution resolution{800, 600};
    SDL_Surface *screen = init(resolution.x, resolution.y);
    PollingPlace currentPlace = PollingPlace_MainMenu;
    MainMenu mainMenu(10, 10);

    while (currentPlace != PollingPlace_Exit)
    {
        currentPlace = mainMenu.enter();
    }

    delete screen;
    return 0;
}
