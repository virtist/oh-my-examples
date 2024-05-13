#include "gmock/gmock.h"
#include "bowling_game.h"

namespace {

class BowlingGameTest : public ::testing::Test {
 protected:
  BowlingGame g;

  void RollMany(int n, int pins) {
    for (int i = 0; i < n; i++) g.Roll(pins);
  }

  void RollSpare() {
    g.Roll(5);
    g.Roll(5);
  }

  void RollStrike() { g.Roll(10); }
};

TEST_F(BowlingGameTest, testGutterGame) {
  RollMany(20, 0);
  EXPECT_EQ(0, g.Score());
}

TEST_F(BowlingGameTest, testAllOnes) {
  RollMany(20, 1);
  EXPECT_EQ(20, g.Score());
}

TEST_F(BowlingGameTest, testOneSpare) {
  RollSpare();
  g.Roll(3);
  RollMany(17, 0);
  EXPECT_EQ(16, g.Score());
}

TEST_F(BowlingGameTest, testOneStrike) {
  RollStrike();
  g.Roll(3);
  g.Roll(4);
  RollMany(16, 0);
  EXPECT_EQ(24, g.Score());
}

TEST_F(BowlingGameTest, testPerfectGame) {
  RollMany(12, 10);
  EXPECT_EQ(300, g.Score());
}

}  // namespace
