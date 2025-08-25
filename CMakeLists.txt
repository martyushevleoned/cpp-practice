cmake_minimum_required(VERSION 3.15)
project(CppPractice VERSION 0.1 LANGUAGES CXX)

# Standart
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Conan toolchain
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Projects
add_subdirectory(feature)