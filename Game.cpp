#include "Game.hpp"
#include <stdexcept>

GameUI::GameUI(int numberOfPlayers) : deck(numberOfPlayers),
                                  cardsValidator(deck.getStartingCard())
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        players.push_back(Player(i));
    }

    resetRound();
}

void GameUI::distributeCardsFromDeck()
{
    for (unsigned int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            players[i].insertCard(deck.takeCard());
        }
    }
}

int GameUI::findStartingPlayer()
{
    Card startingCard = deck.getStartingCard();

    for (unsigned int i = 0; i < players.size(); i++)
    {
        Cards playerCards = players[i].getCards();

        for (unsigned int j = 0; j < playerCards.size(); j++)
        {
            if (playerCards[j] == startingCard)
            {
                return i;
            }
        }
    }

    return 0;
}

bool GameUI::hasRoundEnded()
{
    return playersThatEnded.size() >= players.size() - 1;
}

Player& GameUI::getCurrentPlayer()
{
    return players[currentPlayerId];
}

void GameUI::nextPlayer()
{
    currentPlayerId = (currentPlayerId + 1) % players.size();

    if (players[currentPlayerId].hasEnded())
    {
        nextPlayer();
    }
}

void GameUI::throwCards(Cards cards)
{
    if (passedTurns >= players.size() - 1 - playersThatEnded.size())
    {
        cardsValidator.checkIfCardsHaveSameValue(cards);
    }
    else
    {
        cardsValidator.checkIfCardsCouldBeThrown(cards, table.getCardsFromTop());
    }

    table.throwCards(cards);
    passedTurns = 0;
}

Cards GameUI::getCardsFromTableTop()
{
    return table.getCardsFromTop();
}

void GameUI::passCurrentPlayerTurn()
{
    if (getCardsFromTableTop().size())
    {
        passedTurns++;
    }
    else
    {
        throw std::runtime_error("Cannot pass starting turn");
    }
}

void GameUI::nextRound()
{
    setPeasantsLevels();
    resetRound();
}

void GameUI::checkIfPlayerHasEnded()
{
    if (players[currentPlayerId].hasEnded())
    {
        playersThatEnded.push_back(currentPlayerId);
    }
}

Player &GameUI::getPlayer(unsigned int id)
{
    return players[id];
}

unsigned int GameUI::findOppositePlayerId(int peasantLevel)
{
    for (unsigned i = 0; i < players.size(); i++)
    {
        if (players[i].getPeasantLevel() == -peasantLevel)
        {
            return i;
        }
    }

    return 0;
}

void GameUI::setStartingPlayer()
{
    currentPlayerId = findStartingPlayer();
}

void GameUI::resetRound()
{
    deck.resetDeck(players.size());
    distributeCardsFromDeck();
    currentPlayerId = findStartingPlayer();
    passedTurns = 0;
    playersThatEnded.clear();
    table.clearTable();
}

void GameUI::setPeasantsLevels()
{
    for (unsigned int i = 0; i < players.size(); i++)
    {
        if (not players[i].hasEnded())
        {
            playersThatEnded.push_back(i);
        }
    }

    int peasantLevel;

    if (players.size() == 4 or players.size() == 5)
    {
        peasantLevel = 2;
    }
    else
    {
        peasantLevel = 3;
    }

    for (unsigned i = 0; i < playersThatEnded.size(); i++)
    {
        players[playersThatEnded[i]].setPeasantLevel(peasantLevel);
        peasantLevel--;

        if (peasantLevel == 0 and players.size() != 5) peasantLevel--;
    }
}

