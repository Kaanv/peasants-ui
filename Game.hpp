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
    const std::vector<HistoryElement> getHistory() const;
    void clear();
private:
    std::vector<HistoryElement> history;
};

class Game
{
public:
    Game(Settings settings);

    bool hasRoundEnded() const;
    const Player& getCurrentPlayer() const;
    void nextPlayer();
    void throwSelectedCards();
    const Cards& getCardsFromTableTop() const;
    void passCurrentPlayerTurn();
    void nextRound();
    Player &getPlayer(unsigned int id);
    void performAITurnLua();
    unsigned int getNumberOfEndedRounds() const;
    const std::vector<LevelsHistory>& getLevelsHistory() const;
    void giveCardsToPeasantAsHuman(int playerId);
    const History& getHistory() const;
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
    void addPlayerToPlayersThatEndedListIfPossible();
    int findStartingPlayer() const;
    void takeCardsFromPeasants();
    void giveCardsToPeasants();
    void exchangePlayersCards();
    unsigned int findOppositePlayerId(int peasantLevel) const;
    void saveThrowCardsInHistory(const Cards& cards);
    void savePassedTurnInHistory();
    void giveCardsToPeasantAsAI(int playerId);
    void validateNumberOfCardsToGiveAway(int playerId) const;
    void indicatePeasantLevel(int playerIndex);
    void throwStartingCards();

    Players players;
    Deck deck;
    const CardsValidator cardsValidator;
    History history;
    int currentPlayerId;
    unsigned int passedTurns;
    Table table;
    std::vector<int> playersThatEnded;
    std::vector<lua_State*> aiStates;
    unsigned int numberOfPlayers;
    unsigned int numberOfEndedRounds = 0;
    std::vector<LevelsHistory> levelsHistory;
    Settings settings;
};

#endif
