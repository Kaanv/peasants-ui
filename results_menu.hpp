#pragma once

#include "polling_place.hpp"
#include "score.hpp"

class ResultsMenu : public PollingPlace
{
public:
    ResultsMenu();
    void setScores(Scores scores);
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;

    Scores scores;
};
