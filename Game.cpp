#include "Game.hpp"
#include <stdexcept>
#include <iostream>

Game::Game(int numberOfPlayers) : deck(numberOfPlayers),
                                  cardsValidator(deck.getStartingCard())
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        players.push_back(Player(i));
    }

    resetRound();
}

void Game::distributeCardsFromDeck()
{
    for (unsigned int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            players[i].insertCard(deck.takeCard());
        }
    }
}

int Game::findStartingPlayer()
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

bool Game::hasRoundEnded()
{
    return playersThatEnded.size() >= players.size() - 1;
}

Player& Game::getCurrentPlayer()
{
    return players[currentPlayerId];
}

void Game::nextPlayer()
{
    currentPlayerId = (currentPlayerId + 1) % players.size();

    if (players[currentPlayerId].hasEnded())
    {
        nextPlayer();
    }
}

void Game::throwCards(Cards cards)
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

const Cards& Game::getCardsFromTableTop() const
{
    return table.getCardsFromTop();
}

void Game::passCurrentPlayerTurn()
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

void Game::nextRound()
{
    setPeasantsLevels();
    resetRound();
}

void Game::checkIfPlayerHasEnded()
{
    if (players[currentPlayerId].hasEnded())
    {
        playersThatEnded.push_back(currentPlayerId);
    }
}

Player &Game::getPlayer(unsigned int id)
{
    return players[id];
}

unsigned int Game::findOppositePlayerId(int peasantLevel)
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

void Game::setStartingPlayer()
{
    currentPlayerId = findStartingPlayer();
}

void Game::resetRound()
{
    deck.resetDeck(players.size());
    distributeCardsFromDeck();
    currentPlayerId = findStartingPlayer();
    passedTurns = 0;
    playersThatEnded.clear();
    table.clearTable();
}

void Game::setPeasantsLevels()
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

void Game::performAITurn()
{
    bool selected = false;
    Cards cards= getCurrentPlayer().getCards();
    std::cout << "AI TURN " << getCurrentPlayer().getId() << std::endl;
    std::cout << "AI CARDS:" <<std::endl;
    for (unsigned int i = 0; i < cards.size(); i++)
    {
        if (cards[i]== deck.getStartingCard())
        {
            getCurrentPlayer().selectCard(i);
            selected = true;
        }
        std::cout << cards[i].value << " " << cards[i].color << std::endl;
    }
    Cards tableCards = getCardsFromTableTop();
    std::cout << "TABLE CARDS:" <<std::endl;
    for (unsigned int i = 0; i < tableCards.size(); i++)
    {
        std::cout << tableCards[i].value << " " << tableCards[i].color << std::endl;
    }

    if (selected)
    {
        throwCards(getCurrentPlayer().getSelectedCards());
        getCurrentPlayer().removeSelectedCards();
        checkIfPlayerHasEnded();
        std::cout << "Thrown starting card" << std::endl;
    }
    else
    {
        passCurrentPlayerTurn();
        std::cout << "Passed turn" << std::endl;
    }
    std::cout << std::flush;
    nextPlayer();
}
