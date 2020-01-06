#include "network_client.hpp"

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
}

void NetworkClient::sendMessage(std::string message) const
{
    if (not message.size()) throw(NetworkException("Empty message"));

    sendLengthOfTheString(message.size() + 1);
    sendString(message);
}

void NetworkClient::sendLengthOfTheString(Uint32 len) const
{
    len = SDL_SwapBE32(len);

    Uint32 result = SDLNet_TCP_Send(socket,
                                    &len,
                                    sizeof(len));
    if (result < sizeof(len))
        throw(NetworkException(std::string("SDLNet_TCP_Send: ") +
              std::string(SDLNet_GetError())));

    len = SDL_SwapBE32(len);
}

void NetworkClient::sendString(std::string message) const
{
    Uint32 result = SDLNet_TCP_Send(socket,
                                    message.c_str(),
                                    message.size() + 1);
    if (result < message.size() + 1)
        throw(NetworkException(std::string("SDLNet_TCP_Send: ") +
              std::string(SDLNet_GetError())));
}

void intializeSdlNetwork()
{
    if (SDLNet_Init() == -1)
        throw(NetworkException(std::string("SDLNet_Init: ") +
                               std::string(SDLNet_GetError())));
}
