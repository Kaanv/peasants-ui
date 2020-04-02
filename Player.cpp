#include "Player.hpp"
#include <iostream>

Player::Player(unsigned int _playerId) : playerId(_playerId)
{
    peasantLevel = 0;
}

Cards Player::getCards() const
{
    return cards;
}

void Player::insertCard(Card card)
{
    cards.push_back(card);
}

void Player::selectCard(unsigned int cardId)
{
    cards[cardId].select();
}

void Player::unselectCard(unsigned int cardId)
{
    cards[cardId].selected = false;
}

void Player::unselectCards(std::vector<unsigned int> cardIds)
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

Cards Player::getSelectedCards() const
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
    for (int i = static_cast<int>(cards.size()) - 1; i >= 0; i--)
    {
        if (cards[static_cast<unsigned int>(i)].selected)
        {
            cards.erase(cards.begin() + i);
        }
    }
}

bool Player::hasEnded() const
{
    return not cards.size();
}

void Player::setPeasantLevel(int peasantLevel)
{
    this->peasantLevel = peasantLevel;
}

int Player::getPeasantLevel() const
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
        cardId = static_cast<unsigned int>(cards.size()) -1;
    }

    Card card = cards[cardId];

    cards.erase(cards.begin() + cardId);

    return card;
}

unsigned int Player::getId() const
{
    return playerId;
}

void Player::resetCards()
{
    cards.clear();
}

