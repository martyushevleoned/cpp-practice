#ifndef CLEANER_HPP
#define CLEANER_HPP

#include <csignal>
#include <cstdlib>
#include <list>
#include <memory>

namespace cleaner
{

class DynamicCleaner
{
    class Entry
    {
    public:
        virtual ~Entry() = default;
        virtual void call() const = 0;
        virtual bool equals(void* obj_ptr, void* method_ptr) const = 0;
    };
    template <typename T>
    class EntryImpl : public Entry
    {
    public:
        EntryImpl(T* obj_ptr, void (T::*method_ptr)())
            : m_obj_ptr(obj_ptr), m_method_ptr(method_ptr)
        {
        }
        void call() const override
        {
            (m_obj_ptr->*m_method_ptr)();
        }
        bool equals(void* obj_ptr, void* method_ptr) const override
        {
            return obj_ptr == static_cast<void*>(m_obj_ptr) &&
                   *reinterpret_cast<void (T::**)()>(method_ptr) == m_method_ptr;
        }
    private:
        T* m_obj_ptr;
        void (T::*m_method_ptr)();
    };
public:
    template <typename T>
    static void registrate(T* obj, void (T::*method)())
    {
        m_entries.emplace_back(std::make_shared<EntryImpl<T>>(obj, method));
    };
    template <typename T>
    static void unregistrate(T* obj, void (T::*method)())
    {
        m_entries.remove_if(
            [&](const std::shared_ptr<Entry>& entry)
            { return entry->equals(static_cast<void*>(obj), reinterpret_cast<void*>(&method)); });
    }
    static void call()
    {
        for (const std::shared_ptr<Entry>& entry : m_entries)
        {
            entry->call();
        }
    }
    static void clear()
    {
        m_entries.clear();
    }
private:
    static std::list<std::shared_ptr<Entry>> m_entries;
};

// static
std::list<std::shared_ptr<DynamicCleaner::Entry>> DynamicCleaner::m_entries{};

} // namespace cleaner

#endif