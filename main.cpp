#include "resolution.hpp"
#include "polling_place_id.hpp"
#include "menu.hpp"
#include "game_ui.hpp"
#include "sdl_gl_wrapper.hpp"
#include "settings_menu.hpp"
#include "results_menu.hpp"
#include "net_join_menu.hpp"
#include <ctime>

int main()
{
    std::srand(std::time(0));
    Resolution resolution{800, 600};
    SDL_Surface *screen = init(resolution.x, resolution.y);
    PollingPlaceId currentPlace = PollingPlaceId_MainMenu;
    MainMenu mainMenu;
    ResultsMenu resultsMenu;
    GameUI game;
    SettingsMenu settingsMenu;
    NetJoinMenu netJoinMenu;

    while (currentPlace != PollingPlaceId_Exit)
    {
        switch (currentPlace)
        {
        case PollingPlaceId_Game:
            game.setSettings(settingsMenu.getSettings());
            currentPlace = game.enter();
            break;
        case PollingPlaceId_Settings:
            currentPlace = settingsMenu.enter();
            break;
        case PollingPlaceId_Results:
            resultsMenu.setScores(game.getGameResults());
            currentPlace = resultsMenu.enter();
            break;
        case PollingPlaceId_NetworkGameJoining:
            currentPlace = netJoinMenu.enter();
            break;
        case PollingPlaceId_MainMenu:
        default:
            currentPlace = mainMenu.enter();
        }
    }

    delete screen;
    return 0;
}

