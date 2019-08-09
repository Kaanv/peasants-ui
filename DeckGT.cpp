#include "gtest/gtest.h"
#include <set>
#include "Deck.hpp"

namespace
{

class DeckTest : public ::testing::Test {
protected:
    DeckTest() {}
    virtual ~DeckTest() {}
};

TEST_F(DeckTest, DeckForFourPlayersShouldHaveStartingCardSevenHearts)
{
    Deck deckForFourPlayers(4);
    Card startingCard = deckForFourPlayers.getStartingCard();

    EXPECT_EQ(hearts, startingCard.color);
    EXPECT_EQ(seven, startingCard.value);
}

TEST_F(DeckTest, DeckForFivePlayersShouldHaveStartingCardFiveHearts)
{
    Deck deckForFivePlayers(5);
    Card startingCard = deckForFivePlayers.getStartingCard();

    EXPECT_EQ(hearts, startingCard.color);
    EXPECT_EQ(five, startingCard.value);
}

TEST_F(DeckTest, DeckForSixPlayersShouldHaveStartingCardThreeHearts)
{
    Deck deckForSixPlayers(6);
    Card startingCard = deckForSixPlayers.getStartingCard();

    EXPECT_EQ(hearts, startingCard.color);
    EXPECT_EQ(three, startingCard.value);
}

TEST_F(DeckTest, DeckForFourPlayersShouldContain32Cards)
{
    Deck deck(4);
    Cards cardsInDeck = deck.getCards();

    EXPECT_EQ(32, cardsInDeck.size());
}

TEST_F(DeckTest, DeckForFivePlayersShouldContain40Cards)
{
    Deck deck(5);
    Cards cardsInDeck = deck.getCards();

    EXPECT_EQ(40, cardsInDeck.size());
}

TEST_F(DeckTest, DeckForSixPlayersShouldContain48Cards)
{
    Deck deck(6);
    Cards cardsInDeck = deck.getCards();

    EXPECT_EQ(48, cardsInDeck.size());
}

TEST_F(DeckTest, DeckForFourPlayersShouldContainUniqueCards)
{
    Deck deck(4);
    Cards cardsInDeck = deck.getCards();
    int sizeOfDeck = cardsInDeck.size();

    std::set<Card> s(cardsInDeck.begin(), cardsInDeck.end());
    cardsInDeck.assign(s.begin(), s.end());

    EXPECT_EQ(sizeOfDeck, cardsInDeck.size());
}

TEST_F(DeckTest, DeckForFivePlayersShouldContainUniqueCards)
{
    Deck deck(5);
    Cards cardsInDeck = deck.getCards();
    int sizeOfDeck = cardsInDeck.size();

    std::set<Card> s(cardsInDeck.begin(), cardsInDeck.end());
    cardsInDeck.assign(s.begin(), s.end());

    EXPECT_EQ(sizeOfDeck, cardsInDeck.size());
}

TEST_F(DeckTest, DeckForSixPlayersShouldContainUniqueCards)
{
    Deck deck(6);
    Cards cardsInDeck = deck.getCards();
    int sizeOfDeck = cardsInDeck.size();

    std::set<Card> s(cardsInDeck.begin(), cardsInDeck.end());
    cardsInDeck.assign(s.begin(), s.end());

    EXPECT_EQ(sizeOfDeck, cardsInDeck.size());
}

}
