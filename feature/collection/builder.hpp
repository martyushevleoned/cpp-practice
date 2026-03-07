#include <list>
#include <vector>

namespace collection
{

namespace detail
{

template <typename Item, typename Collection, void (Collection::*Appender)(const Item&)>
class CollectionBuilder
{
public:
    CollectionBuilder(const Item& item)
    {
        (collection.*Appender)(item);
    }
    CollectionBuilder& operator()(const Item& item) &
    {
        (collection.*Appender)(item);
        return *this;
    }
    CollectionBuilder&& operator()(const Item& item) &&
    {
        (collection.*Appender)(item);
        return std::move(*this);
    }
    operator Collection() &&
    {
        return std::move(collection);
    }
private:
    Collection collection{};
};

template <typename Item>
using VectorBuilder = CollectionBuilder<Item, std::vector<Item>, &std::vector<Item>::push_back>;

template <typename Item>
using ListBuilder = CollectionBuilder<Item, std::list<Item>, &std::list<Item>::push_back>;

} // namespace detail

template <typename Item>
std::vector<Item> vector_of()
{
    return {};
}

template <typename Item>
std::list<Item> list_of()
{
    return {};
}

namespace boost_like
{

template <typename Item>
detail::VectorBuilder<Item> vector_of(const Item& item)
{
    return {item};
}

template <typename Item>
detail::ListBuilder<Item> list_of(const Item& item)
{
    return {item};
}

} // namespace boost_like

namespace std_like
{

template <typename Item, typename... Args>
std::vector<Item> vector_of(const Item& first, const Args&... rest)
{
    detail::VectorBuilder<Item> builder{first};
    using expander = int[];
    (void)expander{0, (builder(rest), 0)...};
    return std::move(builder);
}

template <typename Item, typename... Args>
std::list<Item> list_of(const Item& first, const Args&... rest)
{
    detail::ListBuilder<Item> builder{first};
    using expander = int[];
    (void)expander{0, (builder(rest), 0)...};
    return std::move(builder);
}

} // namespace std_like

// TODO map_of

} // namespace collection
