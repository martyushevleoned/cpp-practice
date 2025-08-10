cmake_minimum_required(VERSION 3.15)
project(CppPractice VERSION 0.1 LANGUAGES CXX)

# Standart
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Conan toolchain
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Dependencies
find_package(GTest REQUIRED)

# Sources
set(APP_SRC)
set(APP_LIB)

# Tests
enable_testing()
add_executable(test_target
    ${APP_SRC}
    test/singleton_test.cpp
)
target_include_directories(test_target PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_target ${APP_LIB} GTest::gtest_main)
add_test(NAME MyTests COMMAND test_target)
