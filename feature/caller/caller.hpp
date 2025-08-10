#ifndef CALLER_HPP
#define CALLER_HPP

#include <list>
#include <memory>

namespace caller
{

class Caller
{
    struct Callable
    {
        virtual ~Callable() = default;
        virtual void operator()() = 0;
        virtual bool equals(const Callable* other) const = 0;
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
                (obj_ptr.get()->*m_method_ptr)();
            }
        }
        bool equals(const Callable* other_ptr) const override
        {
            if (const CallableImpl<T>* other = dynamic_cast<const CallableImpl<T>*>(other_ptr))
            {
                std::shared_ptr<T> this_obj = this->m_obj_ptr.lock();
                std::shared_ptr<T> other_obj = other->m_obj_ptr.lock();
                return this_obj && other_obj && this_obj.get() == other_obj.get() &&
                       this->m_method_ptr == other->m_method_ptr;
            }
            return false;
        }
    private:
        std::weak_ptr<T> m_obj_ptr;
        void (T::*m_method_ptr)();
    };
public:
    template <typename T>
    void add(std::shared_ptr<T> obj_ptr, void (T::*method)())
    {
        m_list.push_back(std::make_shared<CallableImpl<T>>(obj_ptr, method));
    }
    template <typename T>
    void remove(std::shared_ptr<T> obj_ptr, void (T::*method)())
    {
        std::shared_ptr<CallableImpl<T>> temp = std::make_shared<CallableImpl<T>>(obj_ptr, method);
        m_list.remove_if([&temp](const std::shared_ptr<Callable>& callable)
                         { return callable->equals(temp.get()); });
    }
    void operator()()
    {
        for (auto& callable : m_list)
        {
            (*callable)();
        }
    }
    void clear()
    {
        m_list.clear();
    }
private:
    std::list<std::shared_ptr<Callable>> m_list;
};

} // namespace caller

#endif // CALLER_HPP
