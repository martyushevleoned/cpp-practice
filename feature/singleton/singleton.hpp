#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>

namespace singleton
{

class SingletonBase
{
protected:
    SingletonBase() = default;
    ~SingletonBase() = default;
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;
    SingletonBase(SingletonBase&&) = delete;
    SingletonBase& operator=(SingletonBase&&) = delete;
};

template <typename T, size_t ID = 0>
class StaticSingleton : private SingletonBase
{
    StaticSingleton() = delete;
    ~StaticSingleton() = delete;
public:
    static T& get(T* t = new T())
    {
        if (!instance)
        {
            instance.reset(t);
        }
        return *instance.get();
    }
    template <typename... Args>
    static T& get(Args&&... args)
    {
        if (!instance)
        {
            instance.reset(new T(std::forward<Args>(args)...));
        }
        return *instance.get();
    }
    static void reset()
    {
        instance.reset();
    }
private:
    static std::unique_ptr<T> instance;
};

// static
template <typename T, size_t ID>
std::unique_ptr<T> StaticSingleton<T, ID>::instance{};

} // namespace singleton

#endif