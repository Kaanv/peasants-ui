#include "gtest/gtest.h"
#include "Player.hpp"

namespace
{

class PlayerTest : public ::testing::Test {
protected:
    PlayerTest()
    {
    }

    virtual ~PlayerTest()
    {

    }

    void insertCardsFromThreeToTen(Player & player)
    {
        Card cardToInsert(seven, hearts);

        for (int i = 3; i < 11; i++)
        {
            player.insertCard(Card(Value(i), hearts));
        }
    }
};

TEST_F(PlayerTest, PlayerShouldHaveCardThatWasInserted)
{
    Card cardToInsert(seven, hearts);
    Player player(0);
    player.insertCard(cardToInsert);
    Cards expectedCards = {cardToInsert};

    EXPECT_EQ(expectedCards, player.getCards());
}

TEST_F(PlayerTest, PlayerShouldHaveCardsThatWereInserted)
{
    Card firstCardToInsert(seven, hearts);
    Card secondCardToInsert(king, spades);
    Player player(0);
    player.insertCard(firstCardToInsert);
    player.insertCard(secondCardToInsert);
    Cards expectedCards = {firstCardToInsert, secondCardToInsert};

    EXPECT_EQ(expectedCards, player.getCards());
}

TEST_F(PlayerTest, SelectingCardShouldBePossible)
{
    Player player(0);
    insertCardsFromThreeToTen(player);

    player.selectCard(1);
    Cards expectedCards = {Card(four, hearts)};

    EXPECT_EQ(expectedCards, player.getSelectedCards());
}

TEST_F(PlayerTest, SelectingMultipleCardsShouldBePossible)
{
    Player player(0);
    insertCardsFromThreeToTen(player);

    player.selectCard(1);
    player.selectCard(3);
    player.selectCard(5);
    Cards expectedCards = {Card(four, hearts), Card(six, hearts), Card(eight, hearts)};

    EXPECT_EQ(expectedCards, player.getSelectedCards());
}

TEST_F(PlayerTest, UnselectingAllCardsShouldLeaveNoSelectedCards)
{
    Player player(0);
    insertCardsFromThreeToTen(player);

    player.selectCard(1);
    player.selectCard(3);
    player.selectCard(5);
    player.unselectAllCards();
    Cards expectedCards = {};

    EXPECT_EQ(expectedCards, player.getSelectedCards());
}

TEST_F(PlayerTest, UnselectingCardShouldRemoveGivenCardFromSelected)
{
    Player player(0);
    insertCardsFromThreeToTen(player);

    player.selectCard(1);
    player.selectCard(3);
    player.selectCard(5);
    player.unselectCard(3);
    Cards expectedCards = {Card(four, hearts), Card(eight, hearts)};

    EXPECT_EQ(expectedCards, player.getSelectedCards());
}

TEST_F(PlayerTest, RemovingSelectedCardsShouldRemoveThoseCardsFromPlayerDeck)
{
    Player player(0);
    insertCardsFromThreeToTen(player);

    player.selectCard(1);
    player.selectCard(3);
    player.selectCard(5);
    player.removeSelectedCards();
    Cards expectedCards = {Card(three, hearts), Card(five, hearts), Card(seven, hearts), Card(nine, hearts), Card(ten, hearts)};

    EXPECT_EQ(expectedCards, player.getCards());
}

}
