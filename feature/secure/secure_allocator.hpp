#ifndef SECURE_ALLOCATOR_HPP
#define SECURE_ALLOCATOR_HPP

#include <cstddef>
#include <cstring>
#include <limits>
#include <new>
#include <type_traits>
#include <utility>

namespace secure
{

template <class T>
class SecureAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
public:
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::true_type;
public:
    constexpr SecureAllocator() noexcept
    {
    }
    template <class U>
    constexpr SecureAllocator(const SecureAllocator<U>&) noexcept
    {
    }
    template <class U>
    struct rebind
    {
        using other = SecureAllocator<U>;
    };
public:
    pointer allocate(size_type n, const void* /*hint*/ = nullptr)
    {
        if (n > std::numeric_limits<size_type>::max() / sizeof(T))
            throw std::bad_alloc();
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }
    void deallocate(pointer p, size_type n) noexcept
    {
        if (!p)
            return;
        clear(p, n * sizeof(T));
        ::operator delete(p);
    }
public:
    template <class U, class... Args>
    void construct(U* p, Args&&... args)
    {
        ::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }
    template <class U>
    void destroy(U* p) noexcept
    {
        if (!p)
            return;
        p->~U();
        clear(static_cast<void*>(p), sizeof(U));
    }
private:
    inline void clear(void* p, std::size_t n)
    {
        volatile unsigned char* vp = static_cast<volatile unsigned char*>(p);
        while (n--)
        {
            *vp++ = 0;
        }
    }
};

template <class T, class U>
inline bool operator==(const SecureAllocator<T>&, const SecureAllocator<U>&) noexcept
{
    return true;
}
template <class T, class U>
inline bool operator!=(const SecureAllocator<T>&, const SecureAllocator<U>&) noexcept
{
    return false;
}

} // namespace secure

#endif // SECURE_ALLOCATOR_HPP