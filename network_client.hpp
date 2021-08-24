#pragma once

#include "SDL2/SDL_net.h"
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
    std::string getLastMessageFromServer();

private:
    void sendString(std::string message) const;

    IPaddress ipAddress;
    SDLNet_SocketSet socketSet;
};
