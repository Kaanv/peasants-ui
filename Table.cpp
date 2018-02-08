#include "Table.hpp"

Cards Table::getCardsFromTop()
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
