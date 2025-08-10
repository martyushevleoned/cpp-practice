# Dependencies
find_package(GTest REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

# Warning
set(WARNING_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND WARNING_FLAGS
        -Wduplicated-cond
        -Wlogical-op
        -Wuseless-cast
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND WARNING_FLAGS
        -Wextra-semi
        -Wnon-virtual-dtor
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(WARNING_FLAGS
        /W4
        /WX
        /permissive-
    )
endif()

# Tests
enable_testing()
function(add_unit_test TEST_NAME)
    cmake_parse_arguments(ARG "" "" "SRC;LIB" ${ARGN})
    add_executable(${TEST_NAME} ${ARG_SRC})
    target_compile_options(${TEST_NAME} PRIVATE ${WARNING_FLAGS})
    target_link_libraries(${TEST_NAME} PRIVATE ${ARG_LIB})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction(add_unit_test)

# Targets
add_unit_test(
    singleton_test
    SRC singleton/singleton_test.cpp
    LIB GTest::gtest_main Boost::boost
)
add_unit_test(
    cleaner_test
    SRC cleaner/cleaner_test.cpp
    LIB GTest::gtest_main
)
add_unit_test(
    secure_string_test
    SRC secure/secure_string_test.cpp
    LIB GTest::gtest_main
)
add_unit_test(
    hash_stream_test
    SRC hash/hash_stream_test.cpp
    LIB GTest::gtest_main openssl::openssl
)
