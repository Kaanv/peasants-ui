#pragma once

#include <vector>

struct Score
{
    int mastersScore;
    int positiveScore;
    unsigned int playerNumber;

    bool operator==(const Score& score)
    {
        return (this->mastersScore == score.mastersScore and
                this->positiveScore == score.positiveScore);
    }
    bool operator>(const Score& score)
    {
        return (this->mastersScore > score.mastersScore or
                (this->mastersScore == score.mastersScore and
                this->positiveScore > score.positiveScore));
    }
    bool operator<(const Score& score)
    {
        return (this->mastersScore < score.mastersScore or
                (this->mastersScore == score.mastersScore and
                this->positiveScore < score.positiveScore));
    }
};

typedef std::vector<Score> Scores;
