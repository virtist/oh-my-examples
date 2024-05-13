cmake_minimum_required(VERSION 3.15)
project(grpc_test)

option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf Package: ${Protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC Package: ${gRPC_VERSION}")

find_package(GTest CONFIG REQUIRED)
message(STATUS "Using GTest Package: ${GTest_VERSION}")

find_program(PROTOC protoc PATHS)
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin PATHS)

execute_process(
    COMMAND ${PROTOC} -I. 
                      --grpc_out=generate_mock_code=true:${CMAKE_CURRENT_SOURCE_DIR} 
                      --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}
                      --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} 
                      hello.proto
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(grpc_test 
    grpc_test.cc
    hello.pb.cc
    hello.grpc.pb.cc
)

target_link_libraries(grpc_test 
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    GTest::gmock_main
)
add_test(NAME grpc_test COMMAND grpc_test)