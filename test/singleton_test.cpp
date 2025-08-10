#include "src/singleton.hpp"
#include <gtest/gtest.h>
#include <string>

struct TestClass
{
    TestClass(int v = 100) : value(v)
    {
    }
    const int value;
};

TEST(StaticSingleton, singletonReuse)
{
    EXPECT_EQ(100, singleton::StaticSingleton<TestClass>::get().value);
    singleton::StaticSingleton<TestClass>::reset();

    EXPECT_EQ(200, singleton::StaticSingleton<TestClass>::get(200).value);
    singleton::StaticSingleton<TestClass>::reset();

    TestClass firstObj{300};
    EXPECT_EQ(300, singleton::StaticSingleton<TestClass>::get(firstObj).value);
    singleton::StaticSingleton<TestClass>::reset();

    TestClass* secondObj = new TestClass{400};
    EXPECT_EQ(400, singleton::StaticSingleton<TestClass>::get(secondObj).value);
    EXPECT_EQ(secondObj, &singleton::StaticSingleton<TestClass>::get());
    singleton::StaticSingleton<TestClass>::reset();
}

TEST(StaticSingleton, mutipleSingleton)
{
    EXPECT_EQ(100, (singleton::StaticSingleton<TestClass, 1>::get().value));
    EXPECT_EQ(200, (singleton::StaticSingleton<TestClass, 2>::get(200).value));
    EXPECT_EQ(300, (singleton::StaticSingleton<TestClass, 3>::get(300).value));

    EXPECT_EQ(100, (singleton::StaticSingleton<TestClass, 1>::get().value));
    EXPECT_EQ(200, (singleton::StaticSingleton<TestClass, 2>::get().value));
    EXPECT_EQ(300, (singleton::StaticSingleton<TestClass, 3>::get().value));
    
    singleton::StaticSingleton<TestClass, 2>::reset();
    singleton::StaticSingleton<TestClass, 3>::reset();
    EXPECT_EQ(100, (singleton::StaticSingleton<TestClass, 1>::get().value));
    EXPECT_EQ(100, (singleton::StaticSingleton<TestClass, 2>::get().value));
    EXPECT_EQ(3000, (singleton::StaticSingleton<TestClass, 3>::get(3000).value));
}

TEST(StaticSingleton, conceptsCheck)
{
    EXPECT_FALSE(std::is_constructible<singleton::StaticSingleton<TestClass>>::value);
    EXPECT_FALSE(std::is_destructible<singleton::StaticSingleton<TestClass>>::value);
    EXPECT_FALSE(std::is_copy_constructible<singleton::StaticSingleton<TestClass>>::value);
    EXPECT_FALSE(std::is_move_constructible<singleton::StaticSingleton<TestClass>>::value);
    EXPECT_FALSE(std::is_copy_assignable<singleton::StaticSingleton<TestClass>>::value);
    EXPECT_FALSE(std::is_move_assignable<singleton::StaticSingleton<TestClass>>::value);
}

class SingletonExample : singleton::SingletonBase
{
public:
    SingletonExample(const int v1, const std::string v2) : value1(v1), value2(v2)
    {
    }
    static SingletonExample& instance()
    {
        return singleton::StaticSingleton<SingletonExample>::get(1, "2");
    }
    const int value1;
    const std::string value2;
};

TEST(SingletonExample, singletonExampleConcepts)
{
    EXPECT_FALSE(std::is_copy_constructible<SingletonExample>::value);
    EXPECT_FALSE(std::is_move_constructible<SingletonExample>::value);
    EXPECT_FALSE(std::is_copy_assignable<SingletonExample>::value);
    EXPECT_FALSE(std::is_move_assignable<SingletonExample>::value);
}

TEST(SingletonExample, initSingletonExample)
{
    EXPECT_EQ(1, SingletonExample::instance().value1);
    EXPECT_EQ("2", SingletonExample::instance().value2);
    singleton::StaticSingleton<SingletonExample>::reset();

    singleton::StaticSingleton<SingletonExample>::get(3, "4");
    EXPECT_EQ(3, SingletonExample::instance().value1);
    EXPECT_EQ("4", SingletonExample::instance().value2);
    singleton::StaticSingleton<SingletonExample>::reset();

    singleton::StaticSingleton<SingletonExample>::get(new SingletonExample(5, "6"));
    EXPECT_EQ(5, SingletonExample::instance().value1);
    EXPECT_EQ("6", SingletonExample::instance().value2);
    singleton::StaticSingleton<SingletonExample>::reset();
}