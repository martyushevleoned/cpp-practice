#ifndef SERVICE_LOCATOR_HPP
#define SERVICE_LOCATOR_HPP

#include <boost/noncopyable.hpp>
#include <memory>
#include <stdexcept>

namespace singleton
{

template <typename T, size_t ID = 0>
class ServiceLocator : private boost::noncopyable
{
    ServiceLocator() = delete;
    ~ServiceLocator() = delete;
public:
    template <typename... Args>
    static void set(Args&&... args)
    {
        instance.reset(new T(std::forward<Args>(args)...));
    }
    static T& get()
    {
        return instance ? *instance.get() : throw std::runtime_error("service is not instanced");
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
std::unique_ptr<T> ServiceLocator<T, ID>::instance{};

} // namespace singleton

#endif
