#ifndef LKM_INCLUDE_TEST_H_
#define LKM_INCLUDE_TEST_H_

#include <linux/string.h>
#include <linux/module.h>

extern int run_test_after_init;

static inline const char *__path_basename(const char *path)
{
	const char *slash = strrchr(path, '/');
	if (slash)
		return slash + 1;
	return path;
}

#define TEST_BEGIN(TEST_NAME)                         \
	void __init test_##TEST_NAME(void)            \
	{                                             \
		const char *__test_name = #TEST_NAME; \
		if (run_test_after_init)

#define TEST_END }

#define ASSERT(EXPRESSION)                                               \
	do {                                                             \
		if (!(EXPRESSION)) {                                     \
			printk(KERN_ERR                                  \
			       "%s: Test FAILED %s at %s:%d: assert %s", \
			       THIS_MODULE->name, __test_name,           \
			       __path_basename(__FILE__), __LINE__,      \
			       #EXPRESSION);                             \
		}                                                        \
	} while (0)

#define ASSERT_EQ(X, Y) ASSERT((X) == (Y))
#define ASSERT_NE(X, Y) ASSERT((X) != (Y))

#define TEST_CALL(TEST_NAME)                 \
	do {                                 \
		void test_##TEST_NAME(void); \
		test_##TEST_NAME();          \
	} while (0)

#endif // LKM_INCLUDE_TEST_H_
