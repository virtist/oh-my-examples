#include "ktest.h"

static void nothing(struct ktest *test)
{
}

static int bowling_game_case_init(struct ktest *test)
{
	ktest_info(test, "case init\n");
	test->priv = (void *)1234;
	return 0;
}

static void bowling_game_case_exit(struct ktest *test)
{
	ktest_info(test, "case exit\n");
}

struct ktest_case bowling_game_test_cases[] = {
	KTEST_CASE(nothing),
	{},
};

struct ktest_suite bowling_game_test_suite = {
	.name = "bowling_game",
	.init = bowling_game_case_init, // SetUp
	.exit = bowling_game_case_exit, // TearDown
	.test_cases = bowling_game_test_cases,
};