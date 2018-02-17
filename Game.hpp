#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Card.hpp"
#include "Deck.hpp"
#include "Table.hpp"
#include "CardsValidator.hpp"
#include <vector>

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
    Cards getCardsFromTableTop();
    void passCurrentPlayerTurn();
    void nextRound();
    void checkIfPlayerHasEnded();
    Player& getPlayer(unsigned int id);
    unsigned int findOppositePlayerId(int peasantLevel);
    void setStartingPlayer();

private:
    void resetRound();
    void setPeasantsLevels();

    Players players;
    Deck deck;
    CardsValidator cardsValidator;
    int currentPlayerId;
    unsigned int passedTurns;
    Table table;
    std::vector<int> playersThatEnded;
};

#endif
