#include "Player.hpp"
#include <iostream>

Player::Player(int playerId)
{
    this->playerId = playerId;
    peasantLevel = 0;
}

Cards Player::getCards()
{
    return cards;
}

void Player::insertCard(Card card)
{
    cards.push_back(card);
}

void Player::selectCard(int cardId)
{
    cards[cardId].select();
}

void Player::unselectCard(int cardId)
{
    cards[cardId].selected = false;
}

void Player::unselectCards(std::vector<int> cardIds)
{
    for (unsigned int i = 0; i < cardIds.size(); i++)
    {
        unselectCard(cardIds[i]);
    }
}

void Player::unselectAllCards()
{
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        unselectCard(i);
    }
}

Cards Player::getSelectedCards()
{
    Cards selectedCards;

    for (unsigned int i = 0; i < cards.size(); i++)
    {
        if (cards[i].selected)
        {
            selectedCards.push_back(cards[i]);
        }
    }

    return selectedCards;
}

void Player::removeSelectedCards()
{
    for (int i = cards.size() - 1; i >= 0; i--)
    {
        if (cards[i].selected)
        {
            cards.erase(cards.begin() + i);
        }
    }
}

bool Player::hasEnded()
{
    return not cards.size();
}

void Player::setPeasantLevel(int peasantLevel)
{
    this->peasantLevel = peasantLevel;
}

int Player::getPeasantLevel()
{
    return peasantLevel;
}

Card Player::takeBestCard()
{
    unsigned int highestCardId = 0;

    for (unsigned int i = 1; i < cards.size(); i++)
    {
        if (cards[i] > cards[highestCardId])
        {
            highestCardId = i;
        }
    }

    Card highestCard = cards[highestCardId];

    cards.erase(cards.begin() + highestCardId);

    return highestCard;
}

Card Player::takeCard(unsigned int cardId)
{
    if (cardId >= cards.size())
    {
        cardId = cards.size() -1;
    }

    Card card = cards[cardId];

    cards.erase(cards.begin() + cardId);

    return card;
}

int Player::getId()
{
    return playerId;
}

