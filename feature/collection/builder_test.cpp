#include "builder.hpp"
#include <boost/assign/list_of.hpp>
#include <gtest/gtest.h>

TEST(CollectionBuilderTest, BuildEmptyCollection)
{
    ASSERT_EQ(std::vector<int>{}, collection::vector_of<int>());
    ASSERT_EQ(std::list<int>{}, collection::list_of<int>());
}

TEST(CollectionBuilderTest, BuildStdLikeCollection)
{
    ASSERT_EQ(std::vector<int>({1, 2, 3}), collection::std_like::vector_of(1, 2, 3));
    ASSERT_EQ(std::list<int>({1, 2, 3}), collection::std_like::list_of(1, 2, 3));
}

TEST(CollectionBuilderTest, BuildBoostLikeCollection)
{
    ASSERT_EQ(std::vector<int>({1, 2, 3}), static_cast<std::vector<int>>(collection::boost_like::vector_of(1)(2)(3)));
    ASSERT_EQ(std::vector<int>({1, 2, 3}), boost::assign::list_of(1)(2)(3));
    ASSERT_EQ(std::list<int>({1, 2, 3}), static_cast<std::list<int>>(collection::boost_like::list_of(1)(2)(3)));
    ASSERT_EQ(std::list<int>({1, 2, 3}), boost::assign::list_of(1)(2)(3));
}
