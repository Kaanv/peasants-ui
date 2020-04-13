#pragma once

#include "network_common.hpp"
#include "SDL/SDL_thread.h"

class NetworkServer
{
public:
    NetworkServer();
    void startServer();
    void sendStringToClient(std::string message) const;

private:
    void resolveHost();
    void openServerSocket();

    IPaddress ip;
    Uint16 port;
    std::string host;
    bool serverStarted = false;
};
