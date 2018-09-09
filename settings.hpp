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
    Settings();
    int numberOfPlayers;
    std::array<PlayerType, 6> playerTypes;
};