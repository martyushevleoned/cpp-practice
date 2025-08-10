#ifndef SECURE_STRING_H
#define SECURE_STRING_H

#include "secure_allocator.hpp"
#include <cstring>
#include <string>
#include <vector>

namespace secure
{

namespace secure_allocator_string
{
// @attention Small String Optimization
using SecureString = std::basic_string<char, std::char_traits<char>, SecureAllocator<char>>;
} // namespace secure_allocator_string

inline namespace vector_based_string
{

class SecureString
{
public:
    using value_type = char;
    using allocator_type = SecureAllocator<value_type>;
    using container_type = std::vector<value_type, allocator_type>;
    using size_type = typename container_type::size_type;
public:
    SecureString() = default;
    SecureString(const char* s)
    {
        assign(s);
    }
    SecureString(const SecureString& other)
    {
        assign(other.c_str());
    }
    SecureString(SecureString&& other) : m_buffer(std::move(other.m_buffer))
    {
        other.clear();
    }
    SecureString& operator=(const SecureString& other)
    {
        if (this != &other)
            assign(other.c_str());
        return *this;
    }
    SecureString& operator=(SecureString&& other)
    {
        if (this != &other)
        {
            m_buffer = std::move(other.m_buffer);
            other.clear();
        }
        return *this;
    }
    ~SecureString()
    {
        clear();
    }
public:
    void assign(const char* s)
    {
        clear();
        size_t len = std::strlen(s);
        m_buffer.resize(len + 1);
        std::memcpy(m_buffer.data(), s, len);
        m_buffer[len] = '\0';
    }
    void clear()
    {
        if (!m_buffer.empty())
        {
            volatile char* p = m_buffer.data();
            for (size_type i = 0; i < m_buffer.size(); ++i)
            {
                p[i] = 0;
            }
            m_buffer.clear();
        }
    }
    size_type size() const
    {
        return m_buffer.empty() ? 0 : m_buffer.size() - 1;
    }
    const char* c_str() const
    {
        return m_buffer.empty() ? "" : m_buffer.data();
    }
    void append(const char* s)
    {
        size_t len = std::strlen(s);
        if (len == 0)
            return;
        size_type old_size = m_buffer.empty() ? 0 : m_buffer.size() - 1;
        m_buffer.resize(old_size + len + 1);
        std::memcpy(m_buffer.data() + old_size, s, len);
        m_buffer.back() = '\0';
    }
    bool empty() const
    {
        return size() == 0;
    }
    void swap(SecureString& other)
    {
        m_buffer.swap(other.m_buffer);
    }
public:
    char& operator[](size_type idx)
    {
        return m_buffer[idx];
    }
    const char& operator[](size_type idx) const
    {
        return m_buffer[idx];
    }
    friend bool operator==(const SecureString& lhs, const SecureString& rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        return std::memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
    }
    friend bool operator!=(const SecureString& lhs, const SecureString& rhs)
    {
        return !(lhs == rhs);
    }
    friend bool operator==(const SecureString& lhs, const char* rhs)
    {
        return std::strcmp(lhs.c_str(), rhs) == 0;
    }
    friend bool operator==(const char* lhs, const SecureString& rhs)
    {
        return rhs == lhs;
    }
    friend bool operator!=(const SecureString& lhs, const char* rhs)
    {
        return !(lhs == rhs);
    }
    friend bool operator!=(const char* lhs, const SecureString& rhs)
    {
        return !(lhs == rhs);
    }
private:
    container_type m_buffer;
};

} // namespace vector_based_string

} // namespace secure

#endif // SECURE_STRING_H