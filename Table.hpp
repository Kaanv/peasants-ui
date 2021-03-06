#ifndef TABLE_HPP
#define TABLE_HPP

#include "Card.hpp"

class Table
{
public:
    const Cards& getCardsFromTop() const;
    void throwCards(Cards cards);
    void clearTable();

private:
    Cards cardsOnTop;
};

#endif
