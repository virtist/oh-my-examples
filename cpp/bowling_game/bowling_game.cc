#include "bowling_game.h"

void BowlingGame::Roll(int pins) { rolls_[current_roll_++] = pins; }

int BowlingGame::Score()
{
    int score = 0;
    int frame_index = 0;
    for (int frame = 0; frame < 10; frame++)
    {
        if (IsStrike(frame_index))
        {
            score += 10 + StrikeBonus(frame_index);
            frame_index++;
        }
        else if (IsSpare(frame_index))
        {
            score += 10 + SpareBonus(frame_index);
            frame_index += 2;
        }
        else
        {
            score += SumOfBallsInFrame(frame_index);
            frame_index += 2;
        }
    }
    return score;
}

bool BowlingGame::IsStrike(int frame_index) { return rolls_[frame_index] == 10; }

bool BowlingGame::IsSpare(int frame_index)
{
    return rolls_[frame_index] + rolls_[frame_index + 1] == 10;
}

int BowlingGame::SumOfBallsInFrame(int frame_index)
{
    return rolls_[frame_index] + rolls_[frame_index + 1];
}

int BowlingGame::SpareBonus(int frame_index) { return rolls_[frame_index + 2]; }

int BowlingGame::StrikeBonus(int frame_index)
{
    return rolls_[frame_index + 1] + rolls_[frame_index + 2];
}