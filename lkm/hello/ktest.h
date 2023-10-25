#ifndef KPI_KTEST_H_
#define KPI_KTEST_H_

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>

/*
 * TAP specifies subtest stream indentation of 4 spaces, 8 spaces for a
 * sub-subtest.  See the "Subtests" section in
 * https://node-tap.org/tap-protocol/
 */
#define KTEST_SUBTEST_INDENT "    "
#define KTEST_SUBSUBTEST_INDENT "        "

/**
 * enum ktest_status - Type of result for a test or test suite
 * @KTEST_SUCCESS: Denotes the test suite has not failed nor been skipped
 * @KTEST_FAILURE: Denotes the test has failed.
 * @KTEST_SKIPPED: Denotes the test has been skipped.
 */
enum ktest_status {
	KTEST_SUCCESS,
	KTEST_FAILURE,
	KTEST_SKIPPED,
};

struct ktest {
	void *priv;

	/* private: internal use only. */
	const char *name; /* Read only after initialization! */

	// spinlock_t lock; /* Guards all mutable test state. */
	enum ktest_status status; /* Read only after test_case finishes! */
};

//
// TEST_CASE
//

struct ktest_case {
	void (*run_case)(struct ktest *test);
	const char *name;
};

#define KTEST_CASE(TEST_NAME)                             \
	{                                                 \
		.run_case = TEST_NAME, .name = #TEST_NAME \
	}

//
// TEST_SUITE
//

struct ktest_suite {
	const char name[256];
	int (*suite_init)(struct ktest_suite *suite);
	void (*suite_exit)(struct ktest_suite *suite);
	int (*init)(struct ktest *test);
	void (*exit)(struct ktest *test);
	struct ktest_case *test_cases;

	/* private: internal use only. */
	struct list_head list;
};

static inline const char *__ktest_basename(const char *path)
{
	const char *slash = strrchr(path, '/');
	if (slash)
		return slash + 1;
	return path;
}

#define KTEST_FAIL(TEST, FMT, ...)                                           \
	do {                                                                 \
		TEST->status = KTEST_FAILURE;                                \
		printk(KERN_ERR "[ktest] [     FAIL ] - %s:%d: " FMT "\n",   \
		       __ktest_basename(__FILE__), __LINE__, ##__VA_ARGS__); \
		return;                                                      \
	} while (0)

#define KTEST_SKIP(TEST, FMT, ...)                                           \
	do {                                                                 \
		TEST->status = KTEST_SKIPPED;                                \
		printk(KERN_ERR "[ktest] [     SKIP ] - %s:%d: " FMT "\n",   \
		       __ktest_basename(__FILE__), __LINE__, ##__VA_ARGS__); \
		return;                                                      \
	} while (0)

//
// ASSERTS
//

#define KTEST_ASSERT(TEST, EXPRESSION)               \
	if (!(EXPRESSION)) {                         \
		KTEST_FAIL(TEST, "%s", #EXPRESSION); \
	}

#define KTEST_ASSERT_FALSE(TEST, EXPRESSION) KTEST_ASSERT(TEST, !(EXPRESSION))

#define KTEST_ASSERT_BINARY(TEST, EXPRESSION, X, Y)                         \
	do {                                                                \
		typeof(X) __X__ = (X);                                      \
		typeof(Y) __Y__ = (Y);                                      \
		if (!(__X__ EXPRESSION __Y__)) {                            \
			if ((long long)__X__ < 0 || (long long)__Y__ < 0) { \
				KTEST_FAIL(TEST,                            \
					   "expect '%lld', actual '%lld' ", \
					   (long long)__X__,                \
					   (long long)__Y__);               \
			} else {                                            \
				KTEST_FAIL(TEST,                            \
					   "expect '%llu', actual '%llu' ", \
					   (unsigned long long)__X__,       \
					   (unsigned long long)__Y__);      \
			}                                                   \
			return;                                             \
		}                                                           \
	} while (0)

#define KTEST_ASSERT_NE(TEST, X, Y) KTEST_ASSERT_BINARY(TEST, !=, (X), (Y))
#define KTEST_ASSERT_EQ(TEST, X, Y) KTEST_ASSERT_BINARY(TEST, ==, (X), (Y))

#define KTEST_ASSERT_HEX_BINARY(TEST, EXPRESSION, X, Y)                       \
	do {                                                                  \
		typeof(X) __X__ = (X);                                        \
		typeof(Y) __Y__ = (Y);                                        \
		if (!(__X__ EXPRESSION __Y__)) {                              \
			KTEST_FAIL(TEST, "expect '0x%llx', actual '0x%llx' ", \
				   (unsigned long long)__X__,                 \
				   (unsigned long long)__Y__);                \
			return;                                               \
		}                                                             \
	} while (0)

#define KTEST_ASSERT_HEX_EQ(TEST, X, Y) \
	KTEST_ASSERT_HEX_BINARY(TEST, ==, (X), (Y))
#define KTEST_ASSERT_HEX_NE(TEST, X, Y) \
	KTEST_ASSERT_HEX_BINARY(TEST, !=, (X), (Y))

#define KTEST_ASSERT_STR_EQ(TEST, X, Y)                                      \
	do {                                                                 \
		const char *__X__ = ((const char *)(X));                     \
		const char *__Y__ = ((const char *)(Y));                     \
		if (strcmp(__X__ ? __X__ : "", __Y__ ? __Y__ : "") != 0) {   \
			KTEST_FAIL(TEST, "expect '%s', actual '%s' ", __X__, \
				   __Y__);                                   \
			return;                                              \
		}                                                            \
	} while (0)

#define KTEST_EXPECT(TEST, EXPRESSION)                                     \
	if (!(EXPRESSION)) {                                               \
		TEST->status = KTEST_FAILURE;                              \
		printk(KERN_ERR "[ktest] [     FAIL ] - %s:%d: %s\n",      \
		       __ktest_basename(__FILE__), __LINE__, #EXPRESSION); \
	}

#define KTEST_EXPECT_TRUE(TEST, EXPRESSION) KTEST_EXPECT(TEST, EXPRESSION)
#define KTEST_EXPECT_FALSE(TEST, EXPRESSION) KTEST_EXPECT(TEST, !(EXPRESSION))
#define KTEST_EXPECT_EQ(TEST, X, Y) KTEST_EXPECT(TEST, (X) == (Y))
#define KTEST_EXPECT_NE(TEST, X, Y) KTEST_EXPECT(TEST, (X) != (Y))

#define ktest_printk(LVL, TEST, FMT, ...)                                \
	printk(LVL KTEST_SUBTEST_INDENT "# %s: " FMT "\n", (TEST)->name, \
	       ##__VA_ARGS__)

#define ktest_info(TEST, FMT, ...) \
	ktest_printk(KERN_INFO, TEST, FMT, ##__VA_ARGS__)

bool is_ktest_mode(void);
void ktest_add_suite(struct ktest_suite *suite);
int ktest_run(void);

#endif // KPI_KTEST_H_
