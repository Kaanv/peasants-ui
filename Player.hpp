#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Card.hpp"

class Player
{
public:
    Player(int _playerId);
    Cards getCards() const;
    void insertCard(Card card);
    void selectCard(int cardId);
    void unselectCard(int cardId);
    void unselectCards(std::vector<int> cardIds);
    void unselectAllCards();
    Cards getSelectedCards() const;
    int getId() const;
    void removeSelectedCards();
    bool hasEnded() const;
    void setPeasantLevel(int peasantLevel);
    int getPeasantLevel() const;
    Card takeBestCard();
    Card takeCard(unsigned int cardId);
    void resetCards();

private:
    int peasantLevel;
    const int playerId;
    Cards cards;
};

typedef std::vector<Player> Players;

#endif
