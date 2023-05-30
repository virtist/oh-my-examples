cmake_minimum_required(VERSION 3.5)
project(hello)


add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(sources 
    src/hello.cc
)

add_executable(hello 
    src/main.cc
    ${sources}
)

#
# Test
#

set(GOOGLETEST_PATH "../cxx_libs/googletest-1.12.1")
set(GMOCK_PATH ${GOOGLETEST_PATH}/googlemock)
set(GTEST_PATH ${GOOGLETEST_PATH}/googletest)

add_executable(test 
    ${GMOCK_PATH}/src/gmock_main.cc
    ${GMOCK_PATH}/src/gmock-all.cc 
    ${GTEST_PATH}/src/gtest-all.cc
    ${sources} 
)
target_compile_definitions(test PRIVATE ENABLE_TEST=1)
target_include_directories(test PRIVATE 
    ${GMOCK_PATH} 
    ${GMOCK_PATH}/include 
    ${GTEST_PATH} 
    ${GTEST_PATH}/include)

target_link_libraries(test pthread)

add_custom_command(TARGET test POST_BUILD
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/test
)
