#include "builder.hpp"
#include <boost/assign/list_of.hpp>
#include <gtest/gtest.h>

TEST(CollectionBuilderTest, BuildVector)
{
    std::vector<int> stdVector{{1, 2, 3}};
    std::vector<int> builderVector{collection::VectorBuilder<int>(1)(2)(3)};
    std::vector<int> boostVector{boost::assign::list_of<int>(1)(2)(3)};
    ASSERT_EQ(stdVector, builderVector);
    ASSERT_EQ(stdVector, boostVector);
}

TEST(CollectionBuilderTest, BuildList)
{
    std::list<int> stdList{{1, 2, 3}};
    std::list<int> builderList{collection::ListBuilder<int>(1)(2)(3)};
    std::list<int> boostList{boost::assign::list_of<int>(1)(2)(3)};
    ASSERT_EQ(stdList, builderList);
    ASSERT_EQ(stdList, boostList);
}
