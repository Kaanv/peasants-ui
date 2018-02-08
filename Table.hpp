#ifndef TABLE_HPP
#define TABLE_HPP

#include "Card.hpp"

class Table
{
public:
    Cards getCardsFromTop();
    void throwCards(Cards cards);
    void clearTable();

private:
    Cards cardsOnTop;
};

#endif
