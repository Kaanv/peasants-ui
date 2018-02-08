#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Card.hpp"

class Player
{
public:
    Player(int playerId);
    Cards getCards();
    void insertCard(Card card);
    void selectCard(int cardId);
    void unselectCard(int cardId);
    void unselectCards(std::vector<int> cardIds);
    void unselectAllCards();
    Cards getSelectedCards();
    int getId();
    void removeSelectedCards();
    bool hasEnded();
    void setPeasantLevel(int peasantLevel);
    int getPeasantLevel();
    Card takeBestCard();
    Card takeCard(unsigned int cardId);

private:
    int peasantLevel;
    int playerId;
    Cards cards;
};

typedef std::vector<Player> Players;

#endif
