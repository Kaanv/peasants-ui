#include "gtest/gtest.h"
#include "CardsValidator.hpp"

namespace
{

class CardsValidatorTest : public ::testing::Test {
protected:
    CardsValidatorTest() : sut(Card(seven, hearts)) {}
    virtual ~CardsValidatorTest() {}

    CardsValidator sut;
};

TEST_F(CardsValidatorTest, WhenThereIsNoCardsOnTableAndCardDifferentThanSevenHertsIsThrownThenErrorShouldBeRaised)
{
    EXPECT_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(eight, hearts)}), Cards({})), std::runtime_error);
}

TEST_F(CardsValidatorTest, WhenThereIsNoCardsOnTableSevenHeartsMayBeThrown)
{
    EXPECT_NO_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(seven, hearts)}), Cards({})));
}

TEST_F(CardsValidatorTest, WhenThereIsNoCardsOnTableTwoSevensWithSevenHeartsIncludedMayBeThrown)
{
    EXPECT_NO_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(seven, spades), Card(seven, hearts)}), Cards({})));
}

TEST_F(CardsValidatorTest, WhenThereIsNoCardsOnTableAndCardDifferentThanSevenWasThrownErrorShouldBeRaised)
{
    EXPECT_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(eight, hearts), Card(seven, hearts)}), Cards({})), std::runtime_error);
}

TEST_F(CardsValidatorTest, WhenThereIsCardOnTableCardWithSameValueCantBeThrown)
{
    EXPECT_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(eight, spades)}), Cards({Card(eight, hearts)})), std::runtime_error);
}

TEST_F(CardsValidatorTest, WhenThereIsCardOnTableThenCardWithHigherValueCanBeThrown)
{
    EXPECT_NO_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(nine, spades)}), Cards({Card(eight, hearts)})));
}

TEST_F(CardsValidatorTest, WhenThereIsCardOnTableThenMultipleCardsEvenWithHigherValueCantBeThrown)
{
    EXPECT_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(nine, spades), Card(nine, hearts)}), Cards({Card(eight, hearts)})), std::runtime_error);
}

TEST_F(CardsValidatorTest, WhenThereIsThreeCardOnTableThenLessCardsEvenWithHigherValueCantBeThrown)
{
    EXPECT_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(nine, spades), Card(nine, hearts)}),
                                               Cards({Card(eight, hearts), Card(eight, spades), Card(eight, diamonds)})), std::runtime_error);
}

TEST_F(CardsValidatorTest, WhenThereIsTwoCardOnTableThenTwoCardsWithDifferentValuesEvenWhenHigherValueCantBeThrown)
{
    EXPECT_THROW(sut.checkIfCardsCouldBeThrown(Cards({Card(nine, spades), Card(ten, hearts)}),
                                               Cards({Card(eight, hearts), Card(eight, spades)})), std::runtime_error);
}

}
