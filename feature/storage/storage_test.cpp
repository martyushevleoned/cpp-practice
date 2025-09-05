#define BOOST_TEST_MODULE MyTestSuite

#include "storage.hpp"
#include <boost/test/included/unit_test.hpp>
#include <string>

BOOST_AUTO_TEST_CASE(get_time)
{
    storage::Storage<std::string> storage;
    storage.set("count", size_t(1));
    BOOST_TEST(storage.get<size_t>("count"), size_t(1));
}