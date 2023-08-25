#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "fakeit.hpp"

#include <iostream>
using namespace std;

int factorial(int number) {
  return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(3) == 6);
  CHECK(factorial(10) == 3628800);
}

TEST_CASE("vectors can be sized and resized") {
  std::vector<int> v(5);

  REQUIRE(v.size() == 5);
  REQUIRE(v.capacity() >= 5);

  SUBCASE("adding to the vector increases its size") {
    v.push_back(1);

    CHECK(v.size() == 6);
    CHECK(v.capacity() >= 6);
  }
  SUBCASE("reserving increases just the capacity") {
    v.reserve(6);

    CHECK(v.size() == 5);
    CHECK(v.capacity() >= 6);
  }
}

TEST_CASE("lots of nested subcases") {
  cout << endl << "root" << endl;
  SUBCASE("") {
    cout << "1" << endl;
    SUBCASE("") { cout << "1.1" << endl; }
  }
  SUBCASE("") {
    cout << "2" << endl;
    SUBCASE("") { cout << "2.1" << endl; }
    SUBCASE("") {
      cout << "2.2" << endl;
      SUBCASE("") {
        cout << "2.2.1" << endl;
        SUBCASE("") { cout << "2.2.1.1" << endl; }
        SUBCASE("") { cout << "2.2.1.2" << endl; }
      }
    }
    SUBCASE("") { cout << "2.3" << endl; }
    SUBCASE("") { cout << "2.4" << endl; }
  }
}

//
// Mock
//

struct SomeInterface {
  virtual int foo(int) = 0;
  virtual int bar(string) = 0;
};

using namespace fakeit;

TEST_CASE("lots mock") {
  Mock<SomeInterface> mock;

  When(Method(mock, foo)).Return(0);

  SomeInterface& i = mock.get();

  // Production code
  i.foo(1);

  // Verify method mock.foo was invoked.
  Verify(Method(mock, foo));

  // Verify method mock.foo was invoked with specific arguments.
  Verify(Method(mock, foo).Using(1));
}