#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GFL");
MODULE_AUTHOR("Bob");
MODULE_DESCRIPTION("A simple Hello world LKM!");
MODULE_VERSION("0.1");

void foo(void);
int run_all_test(void);

static int __init hello_start(void)
{
	printk(KERN_INFO "Loading hello module...\n");
	printk(KERN_INFO "Hello world\n");

	foo();

	return run_all_test(); // call at last
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
