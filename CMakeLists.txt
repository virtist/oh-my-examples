cmake_minimum_required(VERSION 3.27)

if("$ENV{VCPKG_ROOT}" STREQUAL "")
	message(FATAL_ERROR "VCPKG_ROOT not found")
endif()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(oh-my-examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
find_package(GTest CONFIG REQUIRED)

include_directories(
    ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
)

add_subdirectory(cpp)
