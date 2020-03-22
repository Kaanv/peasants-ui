#include "network_server.hpp"

namespace
{
bool serverStarted = false;
TCPsocket server;
}

NetworkServer::NetworkServer()
{
    port = static_cast<Uint16>(22222);
}

#define MAXLEN 1024

TCPsocket client;

int serverLoop(void*)
{
    int i = 0;
    while (true)
    {
        std::cout << i << std::endl;
        i++;
        SDL_Delay(1000);

        if (!client) client = SDLNet_TCP_Accept(server);
        if (!client)
        {
          SDL_Delay(100);
          continue;
        }

        char msg[MAXLEN];
        int len = SDLNet_TCP_Recv(client, msg, MAXLEN-1);
        if (len > 0) std::cout << "RECEIVED " << msg << std::endl;
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
