#include "network_client.hpp"
#include <vector>

#define MAXLEN 1024

static TCPsocket socket;
static std::vector<std::string> messages;

int clientLoop(void*)
{
    int i = 0;
    while (true)
    {
        std::cout << "Client " << i << std::endl;
        i++;
        SDL_Delay(1000);

        if (!socket)
        {
          SDL_Delay(100);
          continue;
        }

        char msg[MAXLEN];
        int len = SDLNet_TCP_Recv(socket, msg, MAXLEN-1);
        if (len > 0)
        {
            std::cout << "RECEIVED " << msg << std::endl;
            messages.push_back(msg);
        }
    }

    return 0;
}

void NetworkClient::connectToHost(std::string ip,
                                  std::string port)
{
    if (SDLNet_ResolveHost(&ipAddress,
                          ip.c_str(),
                          static_cast<Uint16>(std::stol(port))) == -1)
        throw(NetworkException(std::string("SDLNet_ResolveHost: ") +
                               std::string(SDLNet_GetError())));

    socket = SDLNet_TCP_Open(&ipAddress);
    if (!socket)
        throw(NetworkException(std::string("SDLNet_TCP_Open: ") +
                               std::string(SDLNet_GetError())));

    if (SDLNet_TCP_AddSocket(socketSet, socket)==-1)
        throw(NetworkException(std::string("SDLNet_TCP_AddSocket: ") +
                               std::string(SDLNet_GetError())));

    SDL_CreateThread(clientLoop, nullptr, nullptr);
}

void NetworkClient::sendMessage(std::string message) const
{
    if (not message.size()) throw(NetworkException("Empty message"));
    sendString(message);
}

void NetworkClient::sendString(std::string message) const
{
    int result = SDLNet_TCP_Send(socket,
                                 message.c_str(),
                                 static_cast<int>(message.size()) + 1);
    if (result < static_cast<int>(message.size()) + 1)
        throw(NetworkException(std::string("SDLNet_TCP_Send: ") +
              std::string(SDLNet_GetError())));
}

std::string NetworkClient::getLastMessageFromServer()
{
    if (messages.size() > 0)
    {
        std::string result = messages[messages.size() - 1];
        messages.pop_back();
        return result;
    }
    return "";
}
