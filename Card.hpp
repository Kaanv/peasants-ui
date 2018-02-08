#ifndef CARD_HPP
#define CARD_HPP

#include <vector>

enum Color
{
    hearts,
    diamonds,
    clubs,
    spades
};

enum Value
{
    three = 3,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
};

struct Card
{
    Card();

    Card(Value value, Color color);

    void select()
    {
        selected = true;
    }

    friend bool operator<(const Card& l, const Card& r)
    {
        if (l.value == r.value)
        {
            return l.color < r.color;
        }

        return l.value < r.value;
    }

    friend bool operator>(const Card& l, const Card& r)
    {
        return not (l < r);
    }

    friend bool operator==(const Card& l, const Card& r)
    {
        return (l.value == r.value and l.color == r.color);
    }

    friend bool operator!=(const Card& l, const Card& r)
    {
        return (l.value != r.value or l.color != r.color);
    }

    Color color;
    Value value;
    bool selected;
};

typedef std::vector<Card> Cards;

#endif
