#ifndef NETWORKCLIENT_HPP
#define NETWORKCLIENT_HPP

#include "SDL/SDL_net.h"
#include "network_common.hpp"
#include <iostream>

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

#endif // NETWORKCLIENT_HPP
