# Dependencies
find_package(GTest REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

# Tests
enable_testing()
function(add_unit_test TEST_NAME)
    cmake_parse_arguments(ARG "" "" "SRC;LIB" ${ARGN})
    add_executable(${TEST_NAME} ${ARG_SRC})
    target_compile_options(${TEST_NAME} PRIVATE ${CMAKE_WARNING_FLAGS})
    target_link_libraries(${TEST_NAME} PRIVATE ${ARG_LIB})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction(add_unit_test)

# Targets
add_unit_test(
    collection_test
    SRC collection/builder_test.cpp
    LIB GTest::gtest_main Boost::boost
)

add_unit_test(
    universal_cast_test
    SRC cast/universal_cast_test.cpp
    LIB GTest::gtest_main
)
add_unit_test(
    caller_test
    SRC caller/caller_test.cpp
    LIB GTest::gtest_main
)
add_unit_test(
    hash_stream_test
    SRC hash/hash_stream_test.cpp
    LIB GTest::gtest_main openssl::openssl
)
add_unit_test(
    secure_string_test
    SRC secure/secure_string_test.cpp
    LIB GTest::gtest_main
)
add_unit_test(
    service_locator_test
    SRC singleton/service_locator_test.cpp
    LIB GTest::gtest_main Boost::boost
)
