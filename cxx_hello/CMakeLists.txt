cmake_minimum_required(VERSION 3.1)
project(hello)


add_definitions(-std=c++0x)
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

set(GMOCK_PATH "~/source/libs/googletest-1.8.1/googlemock")
set(GTEST_PATH "~/source/libs/googletest-1.8.1/googletest")

add_executable(test 
    ${GMOCK_PATH}/src/gmock_main.cc
    ${GMOCK_PATH}/src/gmock-all.cc 
    ${GTEST_PATH}/src/gtest-all.cc
    ${sources} 
)

target_compile_definitions(test PRIVATE ENABLE_TEST=1)
target_include_directories(test PRIVATE ${GMOCK_PATH} ${GMOCK_PATH}/include ${GTEST_PATH} ${GTEST_PATH}/include)



target_link_libraries(test pthread)
