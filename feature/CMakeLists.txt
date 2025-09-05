# Dependencies
find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

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
    cmake_parse_arguments(ARG "" "" "SRC;LIB;TEST_FRAMEWORK" ${ARGN})
    add_executable(${TEST_NAME} ${ARG_SRC})
    target_compile_options(${TEST_NAME} PRIVATE ${WARNING_FLAGS})
    if(${ARG_TEST_FRAMEWORK} STREQUAL "BOOST")
        target_compile_definitions(${TEST_NAME} PRIVATE BOOST_TEST_DYN_LINK)
        target_link_libraries(${TEST_NAME} PRIVATE ${ARG_LIB} Boost::unit_test_framework)
        message(STATUS "Adding Boost test: ${TEST_NAME}")
    endif()
    if(${ARG_TEST_FRAMEWORK} STREQUAL "GTEST")
        target_link_libraries(${TEST_NAME} PRIVATE ${ARG_LIB} GTest::gtest_main)
        message(STATUS "Adding Google test: ${TEST_NAME}")
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction(add_unit_test)

# Targets
add_unit_test(
    singleton_test
    TEST_FRAMEWORK GTEST
    SRC singleton/singleton_test.cpp
)
add_unit_test(
    cleaner_test
    TEST_FRAMEWORK GTEST
    SRC cleaner/cleaner_test.cpp
)
add_unit_test(
    secure_string_test
    TEST_FRAMEWORK GTEST
    SRC secure/secure_string_test.cpp
)
add_unit_test(
    storage_test
    TEST_FRAMEWORK BOOST
    SRC storage/storage_test.cpp
)
