#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "ktest.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bob");
MODULE_DESCRIPTION("A simple Hello world LKM!");
MODULE_VERSION("0.1");

extern struct ktest_suite example_test_suite;
extern struct ktest_suite bowling_game_test_suite;

static int __init hello_module_init(void)
{
	printk(KERN_INFO "Hello world\n");

	ktest_add_suite(&example_test_suite);
	ktest_add_suite(&bowling_game_test_suite);

	return ktest_run();
}

static void __exit hello_moudle_exit(void)
{
	printk(KERN_INFO "Goodbye Sir.\n");
}

module_init(hello_module_init);
module_exit(hello_moudle_exit);
