int Add(int a, int b) { return a + b; }

#if ENABLE_TEST

#include <gmock/gmock.h>

TEST(Hello, should_one_equals_one) {
  ASSERT_EQ(Add(1, 2), 3);
  ASSERT_NE(Add(-1, -2), -1);
}

#endif