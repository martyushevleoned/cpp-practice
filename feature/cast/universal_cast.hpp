#include <stdexcept>

namespace cast
{

namespace detail
{

namespace relative_time
{

// using time_type = boost::variant<>;

template <typename T, typename F>
constexpr bool is_castable()
{
    return false; // std::is_constructible_v<time_type, F>;
}

template <typename T, typename F>
T cast(const F& from)
{
    static_assert(true, "asd");
}

// Специализации

} // namespace relative_time

namespace absolute_time
{

// using time_type = boost::variant<>;

template <typename T, typename F>
constexpr bool is_castable()
{
    return false; // std::is_constructible_v<time_type, F>;
}

template <typename T, typename F>
T cast(const F& from)
{
    static_assert(true, "asd");
}

// Специализации

} // namespace absolute_time

} // namespace detail

template <typename T, typename F>
T universal_cast(const F& from)
{
    if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<F>)
        return static_cast<T>(from);
    else if constexpr (std::is_constructible_v<T, F>)
        return T(from);
    else if constexpr (detail::relative_time::is_castable<T, F>)
        return detail::relative_time::cast<T, F>(from);
    else if constexpr (detail::absolute_time::is_castable<T, F>)
        return detail::absolute_time::cast<T, F>(from);
    else
        static_assert(true, "No conversion available");
}

} // namespace cast
