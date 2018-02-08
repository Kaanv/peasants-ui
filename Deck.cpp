#include "Deck.hpp"

#include <algorithm>

Deck::Deck() {}

Deck::Deck(int numberOfPlayers)
{
    resetDeck(numberOfPlayers);
}

void Deck::shuffleCards()
{
    std::random_shuffle(cards.begin(), cards.end());
}

Card Deck::takeCard()
{
    Card lastCard = cards.back();
    cards.pop_back();
    return lastCard;
}

Cards Deck::getCards()
{
    return cards;
}

void Deck::resetDeck(int numberOfPlayers)
{
    cards.clear();

    startingCard = createStartingCard(numberOfPlayers);

    for (int color = 0; color < 4; color++)
    {
        for (int value = startingCard.value; value <= ace; value++)
        {
            cards.push_back(Card(Value(value), Color(color)));
        }
    }

    shuffleCards();
}

Card Deck::getStartingCard()
{
    return startingCard;
}

Card Deck::createStartingCard(int numberOfPlayers)
{
    Value smallestValue;

    switch (numberOfPlayers)
    {
        case 4:
            smallestValue = seven;
            break;
        case 5:
            smallestValue = five;
            break;
        case 6:
            smallestValue = three;
            break;
        default:
            smallestValue = three;
    }

    return Card(smallestValue, hearts);
}
