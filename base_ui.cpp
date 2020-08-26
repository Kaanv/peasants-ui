#include "game_ui.hpp"
#include "constants.hpp"
#include "textures.hpp"
#include "text.hpp"
#include <iostream>
#include <algorithm>

namespace
{

std::map<Color, std::string> colorMap
{
    {hearts, "hearts"},
    {diamonds, "diamonds"},
    {clubs, "clubs"},
    {spades, "spades"}
};

std::map<Value, std::string> valueMap
{
    {three, "three"},
    {four, "four"},
    {five, "five"},
    {six, "six"},
    {seven, "seven"},
    {eight, "eight"},
    {nine, "nine"},
    {ten, "ten"},
    {jack, "jack"},
    {queen, "queen"},
    {king, "king"},
    {ace, "ace"}
};

}

BaseUI::BaseUI()
{
    std::string filePath;
    for (int color = hearts; color <= spades; color++)
    {
        for (int value = three; value <= ace; value++)
        {
            filePath = "Images//cards//"
                     + valueMap[static_cast<Value>(value)]
                     + colorMap[static_cast<Color>(color)]
                     + ".png";
            textureMap[std::make_pair<Color, Value>(
                           static_cast<Color>(color),
                           static_cast<Value>(value))] =
                getTexture(filePath);
        }
    }
    cardTopTexture = getTexture("Images//cards//cardtop.png");
}

void BaseUI::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.0, 0.5, 0.0);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

void BaseUI::drawButtonPanel()
{
    double width = 0.5;

    Dimensions fullVertical{width, 2.0};
    Position position{1.0 - width, 1.0};

    glColor3d(0.1, 0.1, 0.5);
    drawRectangle(fullVertical,
                  position);
}

void BaseUI::drawCard(Card card, Position position, double width, double height)
{
    GLuint texture = textureMap[std::make_pair<Color, Value>(
                                    static_cast<Color>(card.color),
                                    static_cast<Value>(card.value))];
    turnOnTextureMode(texture);

    drawTexturedRectangle(
        {width, height},
        {position.x, card.selected ? position.y : position.y - CARD_SELECT_HEIGHT});

    turnOffTextureMode();
}

void BaseUI::drawCardTop(Position position)
{
    turnOnTextureMode(cardTopTexture);

    drawTexturedRectangle(
        {CARD_WIDTH, CARD_HEIGHT},
        {position.x, position.y});

    turnOffTextureMode();
}

void BaseUI::drawCardTopHorizontal(Position position)
{
    turnOnTextureMode(cardTopTexture);

    drawTexturedRectangle(
        {CARD_WIDTH, CARD_HEIGHT},
        {position.x, position.y});

    turnOffTextureMode();
}

void BaseUI::drawCards()
{
    drawCurrentPlayerCards();
    drawAnotherPlayerCards();
    drawTableCards();
}

void BaseUI::drawAnotherPlayersCards(const std::vector<unsigned int>& numberOfCards)
{
    if (numberOfCards.size() == 3) drawAnotherPlayersCardForFourPlayers(numberOfCards);
    else if (numberOfCards.size() == 4) drawAnotherPlayersCardForFivePlayers(numberOfCards);
    else if (numberOfCards.size() == 5) drawAnotherPlayersCardForSixPlayers(numberOfCards);
}

void BaseUI::drawAnotherPlayersCardForFourPlayers(const std::vector<unsigned int>& numberOfCards)
{
    for (unsigned int j = 0; j < numberOfCards[0]; j++)
    {
        drawCardTopHorizontal(Position{-0.9, 0.0 + static_cast<double>(j) * CARD_SPACE});
    }

    for (unsigned int j = 0; j < numberOfCards[1]; j++)
    {
        drawCardTop(Position{-0.5 + static_cast<double>(j) * CARD_SPACE, 0.93});
    }

    for (unsigned int j = 0; j < numberOfCards[2]; j++)
    {
        drawCardTopHorizontal(Position{0.3, 0.0 + static_cast<double>(j) * CARD_SPACE});
    }
}

void BaseUI::drawAnotherPlayersCardForFivePlayers(const std::vector<unsigned int>& numberOfCards)
{
    for (unsigned int j = 0; j < numberOfCards[0]; j++)
    {
        drawCardTopHorizontal(Position{-0.9, -0.4 + static_cast<double>(j) * CARD_SPACE});
    }

    for (unsigned int j = 0; j < numberOfCards[1]; j++)
    {
        drawCardTopHorizontal(Position{-0.9, 0.4 + static_cast<double>(j) * CARD_SPACE});
    }

    for (unsigned int j = 0; j < numberOfCards[2]; j++)
    {
        drawCardTop(Position{-0.5 + static_cast<double>(j) * CARD_SPACE, 0.93});
    }

    for (unsigned int j = 0; j < numberOfCards[3]; j++)
    {
        drawCardTopHorizontal(Position{0.3, 0.0 + static_cast<double>(j) * CARD_SPACE});
    }
}

void BaseUI::drawAnotherPlayersCardForSixPlayers(const std::vector<unsigned int>& numberOfCards)
{
    for (unsigned int j = 0; j < numberOfCards[0]; j++)
    {
        drawCardTopHorizontal(Position{-0.9, -0.4 + static_cast<double>(j) * CARD_SPACE});
    }

    for (unsigned int j = 0; j < numberOfCards[1]; j++)
    {
        drawCardTopHorizontal(Position{-0.9, 0.4 + static_cast<double>(j) * CARD_SPACE});
    }

    for (unsigned int j = 0; j < numberOfCards[2]; j++)
    {
        drawCardTop(Position{-0.5 + static_cast<double>(j) * CARD_SPACE, 0.93});
    }

    for (unsigned int j = 0; j < numberOfCards[3]; j++)
    {
        drawCardTopHorizontal(Position{0.3, 0.4 + static_cast<double>(j) * CARD_SPACE});
    }

    for (unsigned int j = 0; j < numberOfCards[4]; j++)
    {
        drawCardTopHorizontal(Position{0.3, -0.4 + static_cast<double>(j) * CARD_SPACE});
    }
}

void BaseUI::updateSelectedCardSelection(Position glPosition,
                                         Cards& cards,
                                         unsigned int cardIndex)
{
    double width = cardIndex == cards.size() - 1 ? CARD_WIDTH : CARD_SPACE;
    double cardX = -0.5 + static_cast<double>(cardIndex) * CARD_SPACE;
    double cardY = -0.6;

    if (glPosition.x >= cardX and
        glPosition.x <= cardX + width and
        glPosition.y <= cardY and
        glPosition.y >= cardY - CARD_HEIGHT)
    {
        cards[cardIndex].selected = false;
        forceDrawingEverything();
    }
}

void BaseUI::updateNotSelectedCardSelection(Position glPosition,
                                            Cards& cards,
                                            unsigned int cardIndex)
{
    double width = cardIndex == cards.size() - 1 ? CARD_WIDTH : CARD_SPACE;
    double cardX = -0.5 + static_cast<double>(cardIndex) * CARD_SPACE;
    double cardY = -0.6;

    if (glPosition.x >= cardX and
        glPosition.x <= cardX + width and
        glPosition.y <= cardY and
        glPosition.y >= cardY - CARD_HEIGHT)
    {
        cards[cardIndex].selected = true;
        forceDrawingEverything();
    }
}
