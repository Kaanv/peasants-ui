#pragma once

#include <array>

enum PlayerType
{
    PlayerType_None,
    PlayerType_Human,
    PlayerType_AI,
    PlayerType_Network,
    PlayerType_Open
};

struct Settings
{
    unsigned int numberOfPlayers;
    std::array<PlayerType, 6> playerTypes;
    std::array<unsigned int, 6> clientId;
    std::array<unsigned int, 6> playerIdFromClientId;
    std::array<std::string, 6> aiTypes;
    unsigned int numberOfRounds;
};
