#pragma once

#include "button.hpp"
#include "polling_place.hpp"
#include "polling_place_id.hpp"
#include "sdl_gl_wrapper.hpp"
#include "settings.hpp"
#include "Game.hpp"
#include "score.hpp"
#include "network_server.hpp"
#include <vector>
#include <map>
#include <memory>

const double CARD_WIDTH = 0.17;
const double CARD_HEIGHT = 0.3;
const double CARD_SPACE = 0.06;
const double CARD_SELECT_HEIGHT = 0.05;

class BaseUI : public PollingPlace
{
public:
    BaseUI();
protected:
    void drawBackground() override;
    void drawCard(Card card,
                  Position position,
                  double width=CARD_WIDTH,
                  double height=CARD_HEIGHT);
    void drawCardTop(Position position);
    void drawCardTopHorizontal(Position position);
    void drawButtonPanel();
    void updateSelectedCardSelection(Position glPosition,
                                     Cards& cards,
                                     unsigned int cardIndex);
    void updateNotSelectedCardSelection(Position glPosition,
                                        Cards& cards,
                                        unsigned int cardIndex);
    void drawCards();
    virtual void drawCurrentPlayerCards() = 0;
    virtual void drawAnotherPlayerCards() = 0;
    virtual void drawTableCards() = 0;
    void drawAnotherPlayersCardForFourPlayers(const std::vector<unsigned int>& numberOfCards);

private:
    std::map<std::pair<Color, Value>, GLuint> textureMap;
    GLuint cardTopTexture;
};
