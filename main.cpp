#include "resolution.hpp"
#include "polling_place_id.hpp"
#include "menu.hpp"
#include "game_ui.hpp"
#include "sdl_gl_wrapper.hpp"
#include "settings_menu.hpp"
#include "results_menu.hpp"
#include "net_join_menu.hpp"
#include "network_client.hpp"
#include "wait_menu.hpp"
#include "net_create_menu.hpp"
#include "network_server.hpp"
#include <ctime>
#include "SDL/SDL_thread.h"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Resolution resolution{800, 600};
    SDL_Surface *screen = init(resolution.x, resolution.y);
    NetworkClient netClient;
    NetworkServer netServer;
    PollingPlaceId currentPlace = PollingPlaceId_MainMenu;
    MainMenu mainMenu;
    ResultsMenu resultsMenu;
    GameUI game(netServer);
    ClientUI clientGame;
    Settings settings;
    SettingsMenu settingsMenu(settings);
    NetJoinMenu netJoinMenu(netClient);
    NetCreateMenu netCreateMenu(settings, netServer);
    WaitMenu waitMenu(netClient);

    try
    {
        intializeSdlNetwork();
    }
    catch(const NetworkException& e)
    {
        std::cout << "Network error: " << e.what() << std::endl;
    }

    while (currentPlace != PollingPlaceId_Exit)
    {
        switch (currentPlace)
        {
        case PollingPlaceId_Game:
            game.setSettings(settings);
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
        case PollingPlaceId_NetworkGameCreating:
            currentPlace = netCreateMenu.enter();
            break;
        case PollingPlaceId_WaitMenu:
            currentPlace = waitMenu.enter();
            break;
        case PollingPlaceId_ClientGame:
            currentPlace = clientGame.enter();
            break;
        case PollingPlaceId_MainMenu:
        default:
            currentPlace = mainMenu.enter();
        }
    }

    delete screen;
    return 0;
}

