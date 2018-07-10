#include "settings.hpp"

Settings::Settings()
{
    for (int i = 0; i < 6; i++)
    {
        playerTypes[i] = PlayerType_None;
    }
}
