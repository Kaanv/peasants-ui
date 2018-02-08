#ifndef DECK_HPP
#define DECK_HPP

#include "Card.hpp"

class Deck
{
public:
    Deck();
    Deck(int numberOfPlayers);
    void shuffleCards();
    Card takeCard();
    Cards getCards();
    void resetDeck(int numberOfPlayers);
    Card getStartingCard();

private:
    Card createStartingCard(int numberOfPlayers);

    Cards cards;
    Card startingCard;
};
#endif
