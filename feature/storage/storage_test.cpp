#define BOOST_TEST_MODULE MyTestSuite

#include "storage.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(get_time)
{
    storage::SettingsStorage storage;
    storage.set(storage::SettingKey::COUNT, storage::SettingValue(size_t(1)));
    BOOST_TEST(storage.get<size_t>(storage::SettingKey::COUNT), size_t(1));
    BOOST_TEST(storage.get<bool>(storage::SettingKey::COUNT), true);
}