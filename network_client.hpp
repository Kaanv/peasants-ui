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
        if(!socketSet)
        {
            throw(NetworkException(std::string("SDLNet_AllocSocketSet: ") +
                                   std::string(SDLNet_GetError())));
        }
    }

    void connectToHost(std::string ip,
                       std::string port);

private:
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
    {
        throw(NetworkException(std::string("SDLNet_ResolveHost: ") +
                               std::string(SDLNet_GetError())));
    }

    socket = SDLNet_TCP_Open(&ipAddress);
    if (!socket)
    {
        throw(NetworkException(std::string("SDLNet_TCP_Open: ") +
                               std::string(SDLNet_GetError())));
    }

    if (SDLNet_TCP_AddSocket(socketSet, socket)==-1)
    {
        throw(NetworkException(std::string("SDLNet_TCP_AddSocket: ") +
                               std::string(SDLNet_GetError())));
    }
}

void intializeSdlNetwork()
{
    if (SDLNet_Init() == -1)
    {
        throw(NetworkException(std::string("SDLNet_Init: ") +
                               std::string(SDLNet_GetError())));
    }
}

#endif // NETWORKCLIENT_HPP
