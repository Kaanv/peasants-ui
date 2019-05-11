#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Card.hpp"
#include "Deck.hpp"
#include "Table.hpp"
#include "CardsValidator.hpp"
#include <vector>
#include <string>
#include "settings.hpp"

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
    Game(Settings settings);

    bool hasRoundEnded();
    Player& getCurrentPlayer();
    void nextPlayer();
    void throwCards(Cards cards);
    const Cards& getCardsFromTableTop() const;
    void passCurrentPlayerTurn();
    void nextRound();
    Player& getPlayer(unsigned int id);
    void performAITurnLua();
    unsigned int getNumberOfEndedRounds();
    const std::vector<LevelsHistory>& getLevelsHistory();
    void giveCardsToPeasantAsHuman(int playerId);
    History& getHistory();
    void handleIllegalAITurn();

private:
    void resetRound();
    void setPeasantsLevels();
    void setStartingPlayer();
    void registerCardClassInLua();
    void registerCardsClassInLua();
    void registerHistoryClassInLua();
    void addPeasantsLevelsToLevelsHistory();
    void distributeCardsFromDeck();
    void checkIfPlayerHasEnded();
    int findStartingPlayer();
    void takeCardsFromPeasants();
    void giveCardsToPeasants();
    void exchangePlayersCards();
    unsigned int findOppositePlayerId(int peasantLevel);
    void saveThrowCardsInHistory(const Cards& cards);
    void savePassedTurnInHistory();
    void giveCardsToPeasantAsAI(int playerId);
    void validateNumberOfCardsToGiveAway(int playerId);
    void indicatePeasantLevel(int playerIndex);
    void throwStartingCards();

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
    Settings settings;
};

#endif
