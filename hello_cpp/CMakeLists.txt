cmake_minimum_required(VERSION 3.10)
project(hello)

set(CMAKE_BUILD_TYPE "Debug")

# enable c++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(HELLO_BUILD_TESTS "Build unit tests" ON)
option(HELLO_BUILD_BENCHMARKS "Build benchmarks" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall -Werror")

add_executable(hello src/main.cc)

if(HELLO_BUILD_TESTS)
    enable_testing()

    # Use GoogleTest
    include(GoogleTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/googletest-1.12.1)

    file(GLOB HELLO_TESTS_SRCS
        "src/*_test.cc"
    )

    add_executable(tests ${HELLO_TESTS_SRCS})
    target_link_libraries(tests gtest gmock gmock_main)
    gtest_discover_tests(tests)


    # Code Coverage
    include(CodeCoverage)
    append_coverage_compiler_flags()

    setup_target_for_coverage_lcov(
        NAME coverage
        EXECUTABLE make test -j ${n_cores}
        DEPENDENCIES tests
    )

endif()
