#include "gtest/gtest.h"
#include "Game.hpp"

namespace
{

class GameTest : public ::testing::Test {
protected:
    Settings settings{4, {}, {}, 20};
    GameTest() : game(settings) {}

    virtual ~GameTest() {}

    Game game;

    void assertThatStartingPlayerHasStartingCard()
    {
        bool startingCardFound = false;
        Cards playerCards = game.getCurrentPlayer().getCards();

        for (unsigned int j = 0; j < playerCards.size(); j++)
        {
            if (playerCards[j] == Card(seven, hearts))
            {
                startingCardFound = true;
            }
        }

        ASSERT_TRUE(startingCardFound);
    }
};

TEST_F(GameTest, StartingPlayerShouldHaveStartingCard)
{
    assertThatStartingPlayerHasStartingCard();
}

}
