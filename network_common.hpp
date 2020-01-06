#ifndef NETWORKCOMMON_HPP
#define NETWORKCOMMON_HPP

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

#endif // NETWORKCOMMON_HPP
