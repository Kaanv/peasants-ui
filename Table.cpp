#include "Table.hpp"

const Cards& Table::getCardsFromTop() const
{
    return cardsOnTop;
}

void Table::throwCards(Cards cards)
{
    cardsOnTop = cards;
    for (unsigned int i = 0; i < cardsOnTop.size(); i++)
    {
        cardsOnTop[i].selected = false;
    }
}

void Table::clearTable()
{
    cardsOnTop.clear();
}
