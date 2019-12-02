#ifndef NETWORKCLIENT_HPP
#define NETWORKCLIENT_HPP

#include "SDL/SDL_net.h"
#include <iostream>

class NetworkException : public std::exception
{
public:
    NetworkException(std::string e_) : e(e_) {}

    const char* what() const throw()
    {
        return e.c_str();
    }
private:
    std::string e;
};

class NetworkClient
{
public:
    NetworkClient()
    {
        socketSet = SDLNet_AllocSocketSet(1);
        if (!socketSet)
            throw(NetworkException(std::string("SDLNet_AllocSocketSet: ") +
                                   std::string(SDLNet_GetError())));
    }

    void connectToHost(std::string ip,
                       std::string port);
    void sendMessage(std::string message) const;

private:
    void sendLengthOfTheString(Uint32 len) const;
    void sendString(std::string message) const;

    IPaddress ipAddress;
    TCPsocket socket;
    SDLNet_SocketSet socketSet;
};

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

#endif // NETWORKCLIENT_HPP
