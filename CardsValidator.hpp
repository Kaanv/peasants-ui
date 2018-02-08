#ifndef CARDS_VALIDATOR_HPP
#define CARDS_VALIDATOR_HPP

#include "Card.hpp"

class CardsValidator
{
public:
    CardsValidator(Card startingCard);
    void checkIfCardsCouldBeThrown(Cards cards, Cards cardsOnTableTop);
    void checkIfCardsHaveSameValue(Cards cards);

private:
    void validateGameStartingCards(Cards cards);

    Card startingCard;
};

#endif
