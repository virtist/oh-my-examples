///
/// http://butunclebob.com/ArticleS.UncleBob.TheBowlingGameKata
/// https://kata-log.rocks/bowling-game-kata
///
/// ## 规则
/// 1. 一局10轮
/// 2. 每轮两次机会击倒全部10个球
///    - 如果第一次就全中（strike），得分 = 10 + 后两次击倒的瓶数
///    - 如果未全中，则再打一球（spare），得分 = 10 + 后一次击倒的瓶数
///    - 如果两次都未击倒所有瓶（miss），得分 = 两次击倒的的球瓶之和
/// 3. 第10轮有三次击球机会
///    - 如果第一次全中，还可继续投完剩余两球
///    - 如果补中，则继续投完剩余一球
///    - 如果两次都未击倒全部球，则比赛结束
/// 4. 分数最高的胜利
///

#ifndef KATA_BOWLING_GAME_H_
#define KATA_BOWLING_GAME_H_

#include <array>

class BowlingGame {
 public:
  void Roll(int pins);
  int Score();

 private:
  bool IsStrike(int frame_index);
  bool IsSpare(int frame_index);
  int SumOfBallsInFrame(int frame_index);
  int SpareBonus(int frame_index);
  int StrikeBonus(int frame_index);

  std::array<int, 21> rolls_ = {};
  int current_roll_ = 0;
};

#endif  // KATA_BOWLING_GAME_H_
