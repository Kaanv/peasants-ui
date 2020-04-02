#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Card.hpp"

class Player
{
public:
    Player(unsigned int _playerId);
    Cards getCards() const;
    void insertCard(Card card);
    void selectCard(unsigned int cardId);
    void unselectCard(unsigned int cardId);
    void unselectCards(std::vector<unsigned int> cardIds);
    void unselectAllCards();
    Cards getSelectedCards() const;
    unsigned int getId() const;
    void removeSelectedCards();
    bool hasEnded() const;
    void setPeasantLevel(int peasantLevel);
    int getPeasantLevel() const;
    Card takeBestCard();
    Card takeCard(unsigned int cardId);
    void resetCards();

private:
    int peasantLevel;
    const unsigned int playerId;
    Cards cards;
};

typedef std::vector<Player> Players;

#endif
