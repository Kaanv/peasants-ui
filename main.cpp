#include "resolution.hpp"
#include "polling_place.hpp"
#include "menu.hpp"
#include "sdl_gl_wrapper.hpp"

class Game
{
public:
    PollingPlace enter();
private:
    PollingPlace startEventPoll();
    void updateScreen();

    SDL_Event event;
};

int main()
{
    Resolution resolution{800, 600};
    SDL_Surface *screen = init(resolution.x, resolution.y);
    PollingPlace currentPlace = PollingPlace_MainMenu;
    MainMenu mainMenu(10, 10);
    Game game;

    while (currentPlace != PollingPlace_Exit)
    {
        switch (currentPlace)
        {
        case PollingPlace_Game:
            currentPlace = game.enter();
            break;
        case PollingPlace_MainMenu:
        default:
            currentPlace = mainMenu.enter();
        }
    }

    delete screen;
    return 0;
}

PollingPlace Game::enter()
{
    PollingPlace currentPlace = PollingPlace_Game;
    while (currentPlace == PollingPlace_Game)
    {
        currentPlace = startEventPoll();
    }
    return currentPlace;
}

PollingPlace Game::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlace_Exit;
        else if (event.type == SDL_ACTIVEEVENT &&
                 event.active.state & SDL_APPACTIVE &&
                 event.active.gain != 0) updateScreen();
        else if (event.type == SDL_VIDEOEXPOSE) updateScreen();
        else if (event.type == SDL_MOUSEMOTION)
        {

        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {

        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {

        }
    }
    return PollingPlace_Game;
}

void Game::updateScreen()
{

}
