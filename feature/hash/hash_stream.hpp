#ifndef HASH_STREAM_H
#define HASH_STREAM_H

#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

#define ASSERT_NOT_NULL(ptr)                                                                       \
    do                                                                                             \
    {                                                                                              \
        if (!(ptr))                                                                                \
        {                                                                                          \
            throw std::runtime_error("Null pointer encountered: " #ptr);                           \
        }                                                                                          \
    } while (0)

#define OSSL_ASSERT(expr)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if ((expr) != 1)                                                                           \
        {                                                                                          \
            throw std::runtime_error("OpenSSL function failed: " #expr);                           \
        }                                                                                          \
    } while (0)

namespace hash
{

/**
 * Класс, инкапсулирующий результат хэширования
 */
class Hash
{
public:
    Hash()
    {
    }
    explicit Hash(const size_t size) : m_data(size)
    {
    }
    unsigned char* data()
    {
        return m_data.data();
    }
    size_t size() const
    {
        return m_data.size();
    }
    void resize(const size_t newSize)
    {
        m_data.resize(newSize);
    }
    /**
     * Преобразовать байты хэша в строку hex
     */
    std::string toString() const
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (unsigned char byte : m_data)
        {
            oss << std::setw(2) << static_cast<unsigned int>(byte);
        }
        return oss.str();
    }
private:
    std::vector<unsigned char> m_data;
};

/**
 * Буфер для хэширования
 */
class HashBuf : public std::streambuf
{
public:
    explicit HashBuf(const EVP_MD* md_type) : m_md_type(md_type), m_md_ctx(nullptr)
    {
        m_md_ctx = EVP_MD_CTX_new();
        ASSERT_NOT_NULL(m_md_ctx);
        OSSL_ASSERT(EVP_DigestInit_ex(m_md_ctx, md_type, NULL));
    }
    ~HashBuf()
    {
        EVP_MD_CTX_free(m_md_ctx);
    }
    /**
     * Получить хэш
     * @attention Сбрасывает контекст
     */
    Hash getHash()
    {
        Hash result(EVP_MAX_MD_SIZE);
        unsigned int len = 0;
        OSSL_ASSERT(EVP_DigestFinal_ex(m_md_ctx, result.data(), &len));
        result.resize(len);
        EVP_MD_CTX_free(m_md_ctx);
        m_md_ctx = EVP_MD_CTX_new();
        ASSERT_NOT_NULL(m_md_ctx);
        OSSL_ASSERT(EVP_DigestInit_ex(m_md_ctx, m_md_type, NULL));
        return result;
    }
protected:
    // добавить к хэшу один символ
    int overflow(int_type ch) override
    {
        if (ch == traits_type::eof())
        {
            return traits_type::not_eof(ch);
        }
        char c = static_cast<char>(ch);
        OSSL_ASSERT(EVP_DigestUpdate(m_md_ctx, &c, 1));
        return ch;
    }
    // добавить к хэшу массив символов
    std::streamsize xsputn(const char* s, std::streamsize count) override
    {
        OSSL_ASSERT(EVP_DigestUpdate(m_md_ctx, s, static_cast<size_t>(count)));
        return count;
    }
private:
    const EVP_MD* m_md_type;
    EVP_MD_CTX* m_md_ctx;
};

/**
 * Поток, производящий хэширование записанных данных
 */
class HashStream : public std::ostream
{
public:
    explicit HashStream(const EVP_MD* md_type = EVP_sha256())
        : std::ostream(nullptr), m_buf(md_type)
    {
        this->rdbuf(&m_buf);
    }

    Hash getHash()
    {
        return m_buf.getHash();
    }
private:
    HashBuf m_buf;
};

} // namespace hash

#endif // HASH_STREAM_H