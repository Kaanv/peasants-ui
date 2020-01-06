#include "network_common.hpp"

void intializeSdlNetwork()
{
    if (SDLNet_Init() == -1)
        throw(NetworkException(std::string("SDLNet_Init: ") +
                               std::string(SDLNet_GetError())));
}
