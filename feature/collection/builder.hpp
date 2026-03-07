#include <list>
#include <vector>

namespace collection
{

template <typename Item, typename Collection, void (Collection::*Appender)(const Item&)>
class CollectionBuilder
{
public:
    CollectionBuilder(const Item& item)
    {
        (collection.*Appender)(item);
    };
    CollectionBuilder& operator()(const Item& item)
    {
        (collection.*Appender)(item);
        return *this;
    };
    operator Collection() const
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

// TODO MapBuilder
// TODO boost::assign_detail::converter

} // namespace collection
