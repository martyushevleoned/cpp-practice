#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_config.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <cstddef>
#include <map>
#include <string>
#include <type_traits>

namespace storage
{

class SettingValue
{
    template <typename T>
    struct Visitor : boost::static_visitor<T>
    {
        template <typename U>
        T operator()(const U& value) const
        {
            if (std::is_convertible<U, T>::value)
            {
                return static_cast<T>(value);
            }
            return boost::lexical_cast<T>(value);
        }
    };
public:
    using value_type = boost::variant<bool, size_t>;
    SettingValue(const value_type& value) : m_value(value)
    {
    }
    template <typename T>
    const T get()
    {
        return boost::apply_visitor(Visitor<T>{}, m_value);
    }
private:
    const value_type m_value;
};

enum class SettingKey
{
    COUNT,
    USERNAME,
    TIMEOUT
};

class SettingsStorage
{
    typedef std::map<SettingKey, SettingValue> SettingCollection;
    typedef SettingCollection::value_type Setting;
public:
    SettingsStorage() {};
    void set(SettingKey key, SettingValue value)
    {
        m_settings.insert(Setting{key, value});
    }
    template <typename T>
    const T get(SettingKey key)
    {
        return m_settings.at(key).get<T>();
    }
private:
    SettingCollection m_settings;
};

} // namespace storage