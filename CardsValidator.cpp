#include "CardsValidator.hpp"
#include <stdexcept>

CardsValidator::CardsValidator(Card startingCard)
{
    this->startingCard = startingCard;
}

void CardsValidator::checkIfCardsCouldBeThrown(Cards cards, Cards cardsOnTableTop)
{
    if (not cardsOnTableTop.size())
    {
        validateGameStartingCards(cards);
    }
    else
    {
        if (cards.size() != cardsOnTableTop.size())
        {
            throw std::runtime_error("You must throw same number of cards as previous player");
        }

        checkIfCardsHaveSameValue(cards);

        if (not (cards[0].value > cardsOnTableTop[0].value))
        {
            throw std::runtime_error("Cards must be higher than previous");
        }
    }
}

void CardsValidator::checkIfCardsHaveSameValue(Cards cards)
{
    for (unsigned int i = 1; i < cards.size(); i++)
    {
        if (cards[i].value != cards[i - 1].value)
        {
            throw std::runtime_error("Cards must be have same value");
        }
    }
}

void CardsValidator::validateGameStartingCards(Cards cards)
{
    bool startingCardFound = false;

    for (unsigned int i = 0; i < cards.size(); i++)
    {
        if (cards[i] == startingCard)
        {
            startingCardFound = true;
            break;
        }

        if (cards[i].value != startingCard.value)
        {
            throw std::runtime_error("You have to start with starting card");
        }
    }

    if (not startingCardFound)
    {
        throw std::runtime_error("You have to start with starting card");
    }
}

