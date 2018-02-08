#include "Card.hpp"

Card::Card() {}

Card::Card(Value value, Color color)
{
    this->color = color;
    this->value = value;
    selected = false;
}
