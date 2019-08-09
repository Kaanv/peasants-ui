#include "gtest/gtest.h"
#include "Table.hpp"

namespace
{

class TableTest : public ::testing::Test {};

TEST_F(TableTest, EmptyVectorOfCardsShouldBeReturnedWhenNoCardsThrown)
{
    Table table;
    EXPECT_EQ(Cards(), table.getCardsFromTop());
}

TEST_F(TableTest, CardsThatWereThrownShouldBeGettableFromTable)
{
    Table table;
    Cards cards = {Card(seven, hearts), Card(seven, spades)};
    table.throwCards(cards);
    EXPECT_EQ(cards, table.getCardsFromTop());
}

TEST_F(TableTest, OnlyLastCardsThatWereThrownShouldBeOnTop)
{
    Table table;
    Cards cards = {Card(seven, hearts), Card(seven, spades)};
    Cards cardsThrownLater = {Card(eight, hearts), Card(eight, spades)};
    table.throwCards(cards);
    table.throwCards(cardsThrownLater);
    EXPECT_EQ(cardsThrownLater, table.getCardsFromTop());
}

TEST_F(TableTest, CardsOnTableShouldNotBeSelected)
{
    Table table;
    Cards unselectedCards = {Card(seven, hearts), Card(seven, spades)};
    Cards selectedCards = unselectedCards;
    selectedCards[0].select();
    selectedCards[1].select();

    table.throwCards(selectedCards);

    Cards cardsOnTop = table.getCardsFromTop();
    EXPECT_FALSE(cardsOnTop[0].selected);
    EXPECT_FALSE(cardsOnTop[1].selected);
}

TEST_F(TableTest, EmptyVectorOfCardsShouldBeReturnedWhenTableWasCleared)
{
    Table table;
    Cards cards = {Card(seven, hearts), Card(seven, spades)};
    table.throwCards(cards);
    table.clearTable();
    EXPECT_EQ(Cards(), table.getCardsFromTop());
}

}
