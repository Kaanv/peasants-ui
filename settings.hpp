#pragma once

#include <array>

enum PlayerType
{
    PlayerType_None,
    PlayerType_Human,
    PlayerType_AI
};

struct Settings
{
    unsigned int numberOfPlayers;
    std::array<PlayerType, 6> playerTypes;
    std::array<std::string, 6> aiTypes;
    unsigned int numberOfRounds;
};
