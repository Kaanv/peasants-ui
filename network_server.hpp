#pragma once

#include "network_common.hpp"
#include "SDL/SDL_thread.h"

class NetworkServer
{
public:
    NetworkServer();
    void startServer();
    void sendStringToClient(std::string message, unsigned int clientIndex = 0) const;
    std::string getLastMessageFromClient(unsigned int clientIndex);
    void removeLastMessageFromClient(unsigned int clientIndex);

private:
    void resolveHost();
    void openServerSocket();

    IPaddress ip;
    Uint16 port;
    std::string host;
    bool serverStarted = false;
};
