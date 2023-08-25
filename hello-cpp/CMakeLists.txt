cmake_minimum_required(VERSION 3.10)
project(hello)

# Use Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


# Enable C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(HELLO_BUILD_TESTS "Build unit tests" ON)
option(HELLO_BUILD_BENCHMARKS "Build benchmarks" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Compile flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Werror")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping")
endif()

# Include
include_directories(3rd)

add_executable(hello src/main.cc)

if(HELLO_BUILD_TESTS)
    enable_testing()

    file(GLOB HELLO_TESTS_SRCS
        "src/*_test.cc"
    )

    add_executable(hello_test ${HELLO_TESTS_SRCS})
    add_test(NAME hello_test COMMAND hello_test)


    # Code Coverage
    include(CodeCoverage)
    append_coverage_compiler_flags()

    setup_target_for_coverage_lcov(
        NAME coverage
        EXECUTABLE make test -j ${n_cores}
        DEPENDENCIES hello_test
    )

endif()

if(HELLO_BUILD_BENCHMARKS)
  add_executable(hello_benchmark src/hello_benchmark.cc)
endif()