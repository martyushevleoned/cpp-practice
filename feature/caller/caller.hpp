#ifndef CLEANER_HPP
#define CLEANER_HPP

#include <csignal>
#include <cstdlib>
#include <list>
#include <memory>

namespace caller
{

class Caller
{
    struct Callable
    {
        virtual ~Callable() = default;
        virtual void operator()();
        virtual bool operator==(Callable*);
        bool operator!=(Callable* callable)
        {
            return !operator==(callable);
        }
    };
    template <typename T>
    struct CallableImpl : Callable
    {
        CallableImpl(std::shared_ptr<T> obj_ptr, void (T::*method_ptr)())
            : m_obj_ptr(obj_ptr), m_method_ptr(method_ptr)
        {
        }
        void operator()() override
        {
            if (std::shared_ptr<T> obj_ptr = m_obj_ptr.lock())
            {
                (obj_ptr->m_method_ptr())();
            }
        }
        bool operator==(Callable* callable) override
        {
            return static_cast<Callable*>(this) == callable; // TODO const ref comparation
        }
    private:
        std::weak_ptr<T> m_obj_ptr;
        void (T::*m_method_ptr)();
    };
public:
    template <typename T>
    void add(std::shared_ptr<T>& obj_ptr, void (T::*method_ptr)())
    {
        m_list.push_back(std::shared_ptr<Callable>(new CallableImpl<T>(obj_ptr, method_ptr)));
    }
    void operator()()
    {
        for (std::shared_ptr<Callable>& callable : m_list)
        {
            callable->operator()();
        }
    }
    void clear()
    {
        m_list.clear();
    }
private:
    std::list<std::shared_ptr<Callable>> m_list{};
};

} // namespace caller

#endif
