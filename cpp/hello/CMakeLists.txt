
add_executable(cpp_hello main.cc)

if(HELLO_BUILD_TESTS)
    enable_testing()

    add_executable(cpp_hello_test 
        hello_test.cc)
    add_test(NAME cpp_hello_test COMMAND cpp_hello_test)
endif()

if(HELLO_BUILD_BENCHMARKS)
  add_executable(cpp_hello_benchmark hello_benchmark.cc)
endif()