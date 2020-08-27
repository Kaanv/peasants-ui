#include "network_server.hpp"
#include <vector>
#include <array>
#include <string>

namespace
{
TCPsocket server;
}

NetworkServer::NetworkServer()
{
    port = static_cast<Uint16>(22222);
}

#define MAXLEN 1024

static std::vector<TCPsocket> clients;
static SDLNet_SocketSet clientSockets = SDLNet_AllocSocketSet(5);
static std::array<std::vector<std::string>, 5> messages;

int serverLoop(void*)
{
    int i = 0;
    while (true)
    {
        std::cout << "Server " << i << std::endl;
        i++;
        SDL_Delay(1000);

        if (clients.size() < 5)
        {
            TCPsocket client = SDLNet_TCP_Accept(server);
            if (client)
            {
                clients.push_back(client);
                SDLNet_TCP_AddSocket(clientSockets, clients[clients.size() - 1]);
                std::cout << "New client joined, id: " << clients.size() - 1 << std::endl;
            }
        }
        if (clients.size() == 0)
        {
            SDL_Delay(100);
            continue;
        }

        if (SDLNet_CheckSockets(clientSockets, 0))
        {
            for (unsigned int i = 0; i < clients.size(); i++)
            {
                if (SDLNet_SocketReady(clients[i]))
                {
                    char msg[MAXLEN];
                    int len = SDLNet_TCP_Recv(clients[i], msg, MAXLEN-1);
                    if (len > 0) std::cout << "RECEIVED FROM CLIENT " << i << ": " << msg << std::endl;
                    messages[i].push_back(msg);
                }
            }
        }
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

void NetworkServer::sendStringToClient(std::string message, unsigned int clientIndex) const
{
    if (clients.size() > clientIndex)
    {
        int result = SDLNet_TCP_Send(clients[clientIndex],
                                     message.c_str(),
                                     static_cast<int>(message.size()) + 1);
        if (result < static_cast<int>(message.size()) + 1)
            throw(NetworkException(std::string("SDLNet_TCP_Send: ") +
                  std::string(SDLNet_GetError())));
    }
    else
    {
        std::cout << "NO CLIENT" << std::endl;
    }
}

std::string NetworkServer::getLastMessageFromClient(unsigned int clientIndex)
{
    if (clientIndex < clients.size() and messages[clientIndex].size() > 0)
    {
        std::string result = messages[clientIndex][messages[clientIndex].size() - 1];
        messages[clientIndex].pop_back();
        return result;
    }
    return "";
}

void NetworkServer::removeLastMessageFromClient(unsigned int clientIndex)
{
    if (clientIndex < clients.size() and messages[clientIndex].size() > 0)
    {
        messages[clientIndex].pop_back();
    }
}
