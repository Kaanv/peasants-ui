#include "network_server.hpp"

namespace
{
bool serverStarted = false;
}

NetworkServer::NetworkServer()
{
    port = static_cast<Uint16>(22222);
}

int serverLoop(void*)
{
    static int i = 0;
    while (true)
    {
        std::cout << i << std::endl;
        i++;
        SDL_Delay(1000);
    }

    return 0;
}

void NetworkServer::startServer()
{
    if (serverStarted) return;
    serverStarted = true;
    resolveHost();
    host = SDLNet_ResolveIP(&ip);
    openServerSocket();

    SDL_CreateThread(serverLoop, nullptr);
}

void NetworkServer::resolveHost()
{
    if (SDLNet_ResolveHost(&ip, nullptr, port) == -1)
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
    }
}
