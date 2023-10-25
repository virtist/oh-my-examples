#include "ktest.h"
#include <linux/list.h>
#include <linux/ktime.h>

static LIST_HEAD(suite_list);
static int suite_count;

static int success_case_counter;
static int failure_case_counter;
static int skipped_case_counter;
static int disable_suite_counter;
static int disable_case_counter;

static bool run_ktest;
module_param_named(run_ktest, run_ktest, bool, 0600);

#undef PLOG
#define PLOG(STATUS, FMT, ...)                                         \
	do {                                                           \
		printk("[ktest] [ %8s ] " FMT, STATUS, ##__VA_ARGS__); \
	} while (0)

bool is_ktest_mode(void)
{
	return run_ktest;
}

static int ktest_run_case(struct ktest_suite *suite, struct ktest_case *c)
{
	int rc;
	struct ktest t = {
		.priv = NULL,
		.name = c->name,
		//.lock = SPINLOCK_INIT,
		.status = KTEST_SUCCESS,
	};

	s64 delta;
	ktime_t start_time;

	if (strncmp(c->name, "DISABLED_", 9) == 0) {
		disable_case_counter++;
		PLOG("DISABLED", "%s.%s\n", suite->name, c->name);
		return 0;
	}

	PLOG("RUN     ", "%s.%s\n", suite->name, c->name);
	start_time = ktime_get();

	if (suite->init) {
		rc = suite->init(&t);
		if (rc) {
			PLOG("ERROR", "Init failed (%d)\n", rc);
			return 0;
		}
	}
	c->run_case(&t);
	if (suite->exit) {
		suite->exit(&t);
	}
	delta = ktime_to_ms(ktime_sub(ktime_get(), start_time));

	switch (t.status) {
	case KTEST_SUCCESS:
		success_case_counter++;
		PLOG("OK", "%s.%s (%lld ms)\n", suite->name, c->name, delta);
		break;
	case KTEST_FAILURE:
		failure_case_counter++;
		PLOG("FAILED", "%s.%s (%lld ms)\n", suite->name, c->name,
		     delta);
		break;
	case KTEST_SKIPPED:
		skipped_case_counter++;
		PLOG("SKIPPED", "%s.%s (%lld ms)\n", suite->name, c->name,
		     delta);
		break;
	}
	return 0;
}

static int ktest_run_suite(struct ktest_suite *suite)
{
	int rc = 0;
	struct ktest_case *c;

	int tests = 0;
	s64 delta;
	ktime_t start_time;

	if (strncmp(suite->name, "DISABLED_", 9) == 0) {
		disable_suite_counter++;
		printk(KERN_INFO "[ktest] [----------] Suite: %s Disabled\n",
		       suite->name);
		return 0;
	}

	printk(KERN_INFO "[ktest] [----------] Suite: %s\n", suite->name);
	start_time = ktime_get();

	if (suite->suite_init) {
		rc = suite->suite_init(suite);
		if (rc) {
			printk(KERN_ERR
			       "[ktest] [----------] Suite init failed (%d)\n",
			       rc);
			return 0;
		}
	}

	for (c = suite->test_cases; c->name; c++) {
		ktest_run_case(suite, c);
		tests++;
	}

	if (suite->suite_exit) {
		suite->suite_exit(suite);
	}

	delta = ktime_to_ms(ktime_sub(ktime_get(), start_time));
	printk(KERN_INFO
	       "[ktest] [----------] %d tests from %s (%lld ms total)\n",
	       tests, suite->name, delta);
	return 0;
}

void ktest_add_suite(struct ktest_suite *suite)
{
	struct ktest_suite *pos;

	list_for_each_entry(pos, &suite_list, list) {
		if (pos == suite)
			return;
	}

	list_add_tail(&suite->list, &suite_list);
	suite_count++;
}

int ktest_run(void)
{
	struct ktest_suite *suite;
	s64 delta;
	ktime_t start_time;

	if (!is_ktest_mode())
		return 0;

	success_case_counter = 0;
	failure_case_counter = 0;
	skipped_case_counter = 0;

	printk(KERN_INFO "[ktest] =======================================\n");
	printk(KERN_INFO "[ktest] =            Start All Tests          =\n");
	printk(KERN_INFO "[ktest] =======================================\n");

	start_time = ktime_get();
	list_for_each_entry(suite, &suite_list, list) {
		ktest_run_suite(suite);
	}
	delta = ktime_to_ms(ktime_sub(ktime_get(), start_time));

	printk(KERN_INFO
	       "[ktest] [==========] %d tests from %d test suite ran. (%lld ms total)\n",
	       success_case_counter + failure_case_counter +
		       skipped_case_counter,
	       suite_count, delta);

	printk(KERN_INFO
	       "[ktest] [==========] %d success , %d failed, %d skipped, %d disabled\n",
	       success_case_counter, failure_case_counter, skipped_case_counter,
	       disable_case_counter);
	printk(KERN_INFO "[ktest] ========== Finish All Tests ===========\n");
	return 0;
}
