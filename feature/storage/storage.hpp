#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <cstddef>
#include <map>

namespace storage
{

namespace detail
{
template <typename T>
struct Visitor : boost::static_visitor<T>
{
    template <typename U>
    T operator()(const U& value) const;
};

template <typename T>
template <typename U>
T Visitor<T>::operator()(const U& value) const
{
    if (std::is_convertible<U, T>::value)
    {
        return static_cast<T>(value);
    }
    return boost::lexical_cast<T>(value);
}
} // namespace detail

template <typename To, typename From>
To storage_cast(const From& value)
{
    return detail::Visitor<To>{}(value);
}

template <typename StorageKey>
class Storage
{
    using StorageValue = boost::variant<bool, size_t>;
    using StorageCollection = std::map<StorageKey, StorageValue>;
public:
    void set(StorageKey key, StorageValue value)
    {
        m_collection.insert({key, value});
    }
    template <typename Value>
    const Value get(StorageKey key)
    {
        return boost::apply_visitor(detail::Visitor<Value>{}, m_collection.at(key));
    }
private:
    StorageCollection m_collection;
};

} // namespace storage