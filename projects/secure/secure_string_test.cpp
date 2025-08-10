#include "secure_string.hpp"
#include <gtest/gtest.h>

bool is_zeroed(const char* data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (data[i] != 0)
            return false;
    }
    return true;
}

TEST(SecureStringTest, CreateString)
{
    secure::SecureString s("hello");
    EXPECT_EQ(s.size(), 5);
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(SecureStringTest, CopyString)
{
    secure::SecureString s1("copy_me");
    secure::SecureString s2 = s1;
    EXPECT_EQ(s1, s2);
}

TEST(SecureStringTest, MoveString)
{
    secure::SecureString s1("move_me");
    secure::SecureString s2 = std::move(s1);
    EXPECT_EQ(s2, "move_me");
    EXPECT_TRUE(s1.empty());
}

TEST(SecureStringTest, ClearMemory)
{
    secure::SecureString s("clear_me");
    char* data = &s[0];
    size_t size = s.size();
    s.clear();
    EXPECT_TRUE(is_zeroed(data, size));
}
