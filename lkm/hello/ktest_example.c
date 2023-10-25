#include "ktest.h"

static void example_should_test_priv_working(struct ktest *test)
{
	KTEST_ASSERT_EQ(test, 1234, (unsigned long)test->priv);
}

static void example_test_asserts(struct ktest *test)
{
	KTEST_ASSERT_EQ(test, 1234, (unsigned long)test->priv);
	KTEST_ASSERT(test, 1 == 1);
	KTEST_ASSERT_FALSE(test, 1 == 0);
	KTEST_ASSERT_EQ(test, 100, 100);
	KTEST_ASSERT_EQ(test, -100, -100);
	KTEST_ASSERT_NE(test, 1, 2);
	KTEST_ASSERT_NE(test, -100, -222);
	KTEST_ASSERT(test, 1 < 2);

	KTEST_ASSERT_HEX_EQ(test, 0x1234, 0x1234);
	KTEST_ASSERT_HEX_NE(test, 0x1234, 0x12345);

	KTEST_ASSERT_STR_EQ(test, "Hello", "Hello");
}

static void DISABLED_can_disable_test_case(struct ktest *test)
{
}

struct ktest_case example_test_cases[] = {
	KTEST_CASE(example_should_test_priv_working),
	KTEST_CASE(example_test_asserts),
	KTEST_CASE(DISABLED_can_disable_test_case),
	{},
};

static int example_suite_init(struct ktest_suite *suite)
{
	ktest_info(suite, "suite init");
	return 0;
}

static void example_suite_exit(struct ktest_suite *suite)
{
	ktest_info(suite, "suite exit");
}

static int example_case_init(struct ktest *test)
{
	ktest_info(test, "case init\n");
	test->priv = (void *)1234;
	return 0;
}

static void example_case_exit(struct ktest *test)
{
	ktest_info(test, "case exit\n");
}

struct ktest_suite example_test_suite = {
	.name = "example",
	.suite_init = example_suite_init,
	.suite_exit = example_suite_exit,
	.init = example_case_init,
	.exit = example_case_exit,
	.test_cases = example_test_cases,
};
