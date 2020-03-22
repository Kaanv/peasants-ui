#pragma once

#include "network_common.hpp"
#include "SDL/SDL_thread.h"

class NetworkServer
{
public:
    NetworkServer();
    void startServer();

private:
    void resolveHost();
    void openServerSocket();

    IPaddress ip;
    Uint16 port;
    std::string host;
};
