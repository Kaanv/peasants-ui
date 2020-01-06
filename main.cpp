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
#include <ctime>

class NetworkServer
{
public:
    NetworkServer();
    void startServer();

private:
    void resolveHost();
    void openServerSocket();

    IPaddress ip;
    Uint32 ipaddr;
    Uint16 port;
    std::string host;
    TCPsocket server;
};

NetworkServer::NetworkServer()
{
    port = static_cast<Uint16>(22222);
}

void NetworkServer::startServer()
{
    resolveHost();
    host = SDLNet_ResolveIP(&ip);
    openServerSocket();
}

void NetworkServer::resolveHost()
{
    if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
    {
        throw(NetworkException(std::string("SDLNet_ResolveHost: ") +
                               std::string(SDLNet_GetError())));
    }
}

void NetworkServer::openServerSocket()
{
    server = SDLNet_TCP_Open(&ip);
    if (!server)
    {
        throw(NetworkException(std::string("SDLNet_TCP_Open: ") +
                               std::string(SDLNet_GetError())));
        printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
    }
}

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
    NetworkClient netClient;
    NetJoinMenu netJoinMenu(netClient);
    NetCreateMenu netCreateMenu;
    WaitMenu waitMenu;

    NetworkServer netServer;
    netServer.startServer();

    try
    {
        intializeSdlNetwork();
    }
    catch(NetworkException e)
    {
        std::cout << "Network error: " << e.what() << std::endl;
    }

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
        case PollingPlaceId_NetworkGameCreating:
            currentPlace = netCreateMenu.enter();
            break;
        case PollingPlaceId_WaitMenu:
            currentPlace = waitMenu.enter();
            break;
        case PollingPlaceId_MainMenu:
        default:
            currentPlace = mainMenu.enter();
        }
    }

    delete screen;
    return 0;
}

