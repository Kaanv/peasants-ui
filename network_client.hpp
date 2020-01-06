#ifndef NETWORKCLIENT_HPP
#define NETWORKCLIENT_HPP

#include "SDL/SDL_net.h"
#include <iostream>

void intializeSdlNetwork();

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

#endif // NETWORKCLIENT_HPP
