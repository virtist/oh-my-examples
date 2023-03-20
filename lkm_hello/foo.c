#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>

void foo(void)
{
	printk(KERN_INFO "%s: call foo()\n", THIS_MODULE->name);
}