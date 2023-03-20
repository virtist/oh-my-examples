#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include "test.h"

int run_test_after_init = 0;
module_param_named(run_test, run_test_after_init, int, 0644);

int __init run_all_test(void)
{
	if (run_test_after_init == 0)
		return 0;

	printk(KERN_NOTICE "%s: ===== run all test =====\n", THIS_MODULE->name);
	TEST_CALL(just_a_example_test);

	return 0;
}

TEST_BEGIN(just_a_example_test)
{
	ASSERT_EQ(1, 1);
	ASSERT_NE(1, 2);
	ASSERT(1 < 2);

	ASSERT_EQ(1, 2);
	ASSERT_NE(1, 1);
	ASSERT(1 == 2);
}
TEST_END
