#include "hash_stream.hpp"
#include <gtest/gtest.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/provider.h>

using namespace hash;

/**
 * Фикстура с инициализацией openssl 3.0+ провайдера
 */
class OSSLHashFixture : public testing::Test
{
protected:
    void SetUp() override
    {
        default_provider = OSSL_PROVIDER_load(nullptr, "default");
        ASSERT_NE(default_provider, nullptr) << "Failed to load OpenSSL default provider";
        md_sha256 = EVP_MD_fetch(nullptr, "SHA256", nullptr);
        ASSERT_NE(md_sha256, nullptr) << "Failed to fetch SHA256 digest";
    }
    void TearDown() override
    {
        EVP_MD_free(md_sha256);
        md_sha256 = nullptr;
        OSSL_ASSERT(OSSL_PROVIDER_unload(default_provider));
        default_provider = nullptr;
        ASSERT_EQ(ERR_peek_error(), 0u);
        ERR_clear_error();
    }
protected:
    const size_t SHA256_HASH_LENGTH = 32;
    const size_t SHA256_HASH_STRING_LENGTH = 64;
    const char* SHA256_EMPTY_HASH =
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
    const char* SHA256_HASH_OF_ZEROS =
        "2ac9a6746aca543af8dff39894cfe8173afba21eb01c6fae33d52947222855ef";
    const char* SHA256_HASH_OF_ONES =
        "f6e0a1e2ac41945a9aa7ff8a8aaa0cebc12a3bcc981a929ad5cf810a090e11ae";
private:
    OSSL_PROVIDER* default_provider = nullptr;
    EVP_MD* md_sha256 = nullptr;
};

TEST_F(OSSLHashFixture, GetHash_Success)
{
    HashStream hashStream;
    hashStream << "000";
    const Hash hash = hashStream.getHash();
    EXPECT_EQ(SHA256_HASH_LENGTH, hash.size());
    const std::string hashString = hash.toString();
    EXPECT_EQ(SHA256_HASH_OF_ZEROS, hashString);
    EXPECT_EQ(SHA256_HASH_STRING_LENGTH, hashString.size());
}

TEST_F(OSSLHashFixture, EmptyHash_Success)
{
    EXPECT_EQ(SHA256_EMPTY_HASH, HashStream().getHash().toString());
    EXPECT_EQ(SHA256_EMPTY_HASH, (HashStream() << "").getHash().toString());
    EXPECT_EQ(SHA256_EMPTY_HASH, (HashStream() << "" << "").getHash().toString());
}

TEST_F(OSSLHashFixture, ReuseHashStream_Success)
{
    HashStream hashStream;
    hashStream << "000";
    EXPECT_EQ(SHA256_HASH_OF_ZEROS, hashStream.getHash().toString());
    hashStream << "111";
    EXPECT_EQ(SHA256_HASH_OF_ONES, hashStream.getHash().toString());
    hashStream << "";
    EXPECT_EQ(SHA256_EMPTY_HASH, hashStream.getHash().toString());
}

TEST_F(OSSLHashFixture, SplitHashStream_Success)
{
    EXPECT_EQ(SHA256_HASH_OF_ZEROS, (HashStream() << "000").getHash().toString());
    EXPECT_EQ(SHA256_HASH_OF_ZEROS, (HashStream() << "0" << "00").getHash().toString());
    EXPECT_EQ(SHA256_HASH_OF_ZEROS, (HashStream() << "00" << "0").getHash().toString());
    EXPECT_EQ(SHA256_HASH_OF_ZEROS, (HashStream() << "0" << "0" << "0").getHash().toString());
}