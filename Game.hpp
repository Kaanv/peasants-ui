#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Card.hpp"
#include "Deck.hpp"
#include "Table.hpp"
#include "CardsValidator.hpp"
#include <vector>
#include <string>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

struct  HistoryElement
{
    int playerId;
    std::string action;
    Cards cards;
};

typedef std::vector<int> LevelsHistory;

class History
{
public:
    void saveThrowCards(Cards cards, int playerId);
    void savePassedTurn(int playerId);
    std::vector<HistoryElement> getHistory();
    void clear();
private:
    std::vector<HistoryElement> history;
};

class Game
{
public:
    Game(int numberOfPlayers);

    void distributeCardsFromDeck();
    int findStartingPlayer();
    bool hasRoundEnded();
    Player& getCurrentPlayer();
    void nextPlayer();
    void throwCards(Cards cards);
    const Cards& getCardsFromTableTop() const;
    void passCurrentPlayerTurn();
    void nextRound();
    void checkIfPlayerHasEnded();
    Player& getPlayer(unsigned int id);
    unsigned int findOppositePlayerId(int peasantLevel);
    void setStartingPlayer();
    void saveThrowCardsInHistory(const Cards& cards);
    void savePassedTurnInHistory();
    void performAITurn();
    void performAITurnLua();
    unsigned int getNumberOfEndedRounds();
    const std::vector<LevelsHistory>& getLevelsHistory();
    void giveCardsToPeasantAsAI(int playerId);
    void giveCardsToPeasantAsHuman(int playerId);

private:
    void resetRound();
    void setPeasantsLevels();
    void AIselectAllStartingValues();
    void registerCardClassInLua();
    void registerCardsClassInLua();
    void registerHistoryClassInLua();
    void addPeasantsLevelsToLevelsHistory();

    Players players;
    Deck deck;
    CardsValidator cardsValidator;
    History history;
    int currentPlayerId;
    unsigned int passedTurns;
    Table table;
    std::vector<int> playersThatEnded;
    std::vector<lua_State*> aiStates;
    int numberOfPlayers;
    unsigned int numberOfEndedRounds = 0;
    std::vector<LevelsHistory> levelsHistory;
};

#endif
