#ifndef CARDS_VALIDATOR_HPP
#define CARDS_VALIDATOR_HPP

#include "Card.hpp"

class CardsValidator
{
public:
    CardsValidator(Card _startingCard);
    void checkIfCardsCouldBeThrown(Cards cards, Cards cardsOnTableTop) const;
    void checkIfCardsHaveSameValue(Cards cards) const;

private:
    void validateGameStartingCards(Cards cards) const;

    const Card startingCard;
};

#endif
