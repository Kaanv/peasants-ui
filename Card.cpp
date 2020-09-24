#include "Card.hpp"

Card::Card() {}

Card::Card(Value value, Color color)
{
    this->color = color;
    this->value = value;
    selected = false;
}

char convertValueToChar(Value value)
{
    if (value == three) return '3';
    else if (value == four) return '4';
    else if (value == five) return '5';
    else if (value == six) return '6';
    else if (value == seven) return '7';
    else if (value == eight) return '8';
    else if (value == nine) return '9';
    else if (value == ten) return '0';
    else if (value == jack) return 'j';
    else if (value == queen) return 'q';
    else if (value == king) return 'k';
    else if (value == ace) return 'a';
    return 'x';
}

Value convertCharToValue(const char value)
{
    if (value == '3') return three;
    else if (value == '4') return four;
    else if (value == '5') return five;
    else if (value == '6') return six;
    else if (value == '7') return seven;
    else if (value == '8') return eight;
    else if (value == '9') return nine;
    else if (value == '0') return ten;
    else if (value == 'j') return jack;
    else if (value == 'q') return queen;
    else if (value == 'k') return king;
    else if (value == 'a') return ace;
    return three;
}

char convertColorToChar(Color color)
{
    if (color == hearts) return 'h';
    else if (color == diamonds) return 'd';
    else if (color == clubs) return 'c';
    else if (color == spades) return 's';
    return 'x';
}

Color convertCharToColor(const char color)
{
    if (color == 'h') return hearts;
    else if (color == 'd') return diamonds;
    else if (color == 'c') return clubs;
    else if (color == 's') return spades;
    return spades;
}

std::string convertCardsToString(const Cards& cards)
{
    std::string result = "";

    for (const auto& card : cards)
    {
        result += convertValueToChar(card.value);
        result += convertColorToChar(card.color);
    }

    return result;
}

Card convertCharsToCard(const char value, const char color)
{
    return Card(convertCharToValue(value), convertCharToColor(color));
}
