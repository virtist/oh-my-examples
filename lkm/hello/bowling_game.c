///
/// TDD练习题：保龄球记分程序
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

#include "ktest.h"

//
// Bowling game
//

#define BOWLING_ROLL_MAX 21

struct bowling_game {
	int rolls[BOWLING_ROLL_MAX];
	int current_roll;
};

void init_bowling_game(struct bowling_game *g)
{
	memset(g, 0, sizeof(*g));
}

void bowling_roll(struct bowling_game *g, int pins)
{
	g->rolls[g->current_roll++] = pins;
}

static bool is_strike(struct bowling_game *g, int frame_index)
{
	return g->rolls[frame_index] == 10;
}

static bool is_spare(struct bowling_game *g, int frame_index)
{
	return g->rolls[frame_index] + g->rolls[frame_index + 1] == 10;
}

static int sum_of_balls_in_frame(struct bowling_game *g, int frame_index)
{
	return g->rolls[frame_index] + g->rolls[frame_index + 1];
}

static int spare_bonus(struct bowling_game *g, int frame_index)
{
	return g->rolls[frame_index + 2];
}

static int strike_bonus(struct bowling_game *g, int frame_index)
{
	return g->rolls[frame_index + 1] + g->rolls[frame_index + 2];
}

int bowling_score(struct bowling_game *g)
{
	int score = 0;
	int frame_index = 0;
	int frame = 0;

	for (; frame < 10; frame++) {
		if (is_strike(g, frame_index)) {
			score += 10 + strike_bonus(g, frame_index);
			frame_index++;
		} else if (is_spare(g, frame_index)) {
			score += 10 + spare_bonus(g, frame_index);
			frame_index += 2;
		} else {
			score += sum_of_balls_in_frame(g, frame_index);
			frame_index += 2;
		}
	}

	return score;
}

static void roll_many(struct bowling_game *g, int n, int pins)
{
	int i;
	for (i = 0; i < n; i++) {
		bowling_roll(g, pins);
	}
}

//
// Tests
//

static struct bowling_game game;

static void roll_spare(struct bowling_game *g)
{
	bowling_roll(g, 5);
	bowling_roll(g, 5);
}

static void roll_strike(struct bowling_game *g)
{
	bowling_roll(g, 10);
}

static void test_gutter_game(struct ktest *test)
{
	roll_many(&game, 20, 0);
	KTEST_ASSERT_EQ(test, 0, bowling_score(&game));
}

static void test_all_ones(struct ktest *test)
{
	roll_many(&game, 20, 1);
	KTEST_ASSERT_EQ(test, 20, bowling_score(&game));
}

static void test_one_spare(struct ktest *test)
{
	roll_spare(&game);
	bowling_roll(&game, 3);
	roll_many(&game, 17, 0);
	KTEST_ASSERT_EQ(test, 16, bowling_score(&game));
}

static void test_one_strike(struct ktest *test)
{
	roll_strike(&game);
	bowling_roll(&game, 3);
	bowling_roll(&game, 4);
	roll_many(&game, 16, 0);
	KTEST_ASSERT_EQ(test, 24, bowling_score(&game));
}

static void test_perfect_game(struct ktest *test)
{
	roll_many(&game, 12, 10);
	KTEST_ASSERT_EQ(test, 300, bowling_score(&game));
}

static int bowling_game_case_init(struct ktest *test)
{
	init_bowling_game(&game);
	return 0;
}

struct ktest_case bowling_game_test_cases[] = {
	KTEST_CASE(test_gutter_game),  KTEST_CASE(test_all_ones),
	KTEST_CASE(test_one_spare),    KTEST_CASE(test_one_strike),
	KTEST_CASE(test_perfect_game), {},
};

struct ktest_suite bowling_game_test_suite = {
	.name = "bowling_game",
	.init = bowling_game_case_init, // SetUp
	.test_cases = bowling_game_test_cases,
};