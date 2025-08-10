#include "service_locator.hpp"
#include <gtest/gtest.h>

struct TestClass
{
    TestClass(int v = 100) : value(v)
    {
    }
    const int value;
};

TEST(ServiceLocator, serviceReuse)
{
    singleton::ServiceLocator<TestClass>::set();
    EXPECT_EQ(100, singleton::ServiceLocator<TestClass>::get().value);
    singleton::ServiceLocator<TestClass>::reset();

    singleton::ServiceLocator<TestClass>::set(200);
    EXPECT_EQ(200, singleton::ServiceLocator<TestClass>::get().value);

    singleton::ServiceLocator<TestClass>::set(300);
    EXPECT_EQ(300, singleton::ServiceLocator<TestClass>::get().value);
    singleton::ServiceLocator<TestClass>::reset();
}

TEST(ServiceLocator, mutipleServices)
{
    enum
    {
        SERVICE_1 = 1,
        SERVICE_2 = 2,
        SERVICE_3 = 3
    };

    singleton::ServiceLocator<TestClass, SERVICE_1>::set();
    singleton::ServiceLocator<TestClass, SERVICE_2>::set(200);
    singleton::ServiceLocator<TestClass, SERVICE_3>::set(300);

    EXPECT_EQ(100, (singleton::ServiceLocator<TestClass, SERVICE_1>::get().value));
    EXPECT_EQ(200, (singleton::ServiceLocator<TestClass, SERVICE_2>::get().value));
    EXPECT_EQ(300, (singleton::ServiceLocator<TestClass, SERVICE_3>::get().value));

    singleton::ServiceLocator<TestClass, SERVICE_1>::reset();
    singleton::ServiceLocator<TestClass, SERVICE_2>::reset();
    singleton::ServiceLocator<TestClass, SERVICE_3>::reset();
}

TEST(ServiceLocator, conceptsCheck)
{
    EXPECT_FALSE(std::is_constructible<singleton::ServiceLocator<TestClass>>::value);
    EXPECT_FALSE(std::is_destructible<singleton::ServiceLocator<TestClass>>::value);
    EXPECT_FALSE(std::is_copy_constructible<singleton::ServiceLocator<TestClass>>::value);
    EXPECT_FALSE(std::is_move_constructible<singleton::ServiceLocator<TestClass>>::value);
    EXPECT_FALSE(std::is_copy_assignable<singleton::ServiceLocator<TestClass>>::value);
    EXPECT_FALSE(std::is_move_assignable<singleton::ServiceLocator<TestClass>>::value);
}

struct SingletonExample : private boost::noncopyable
{
    SingletonExample(const int v1, const std::string v2) : value1(v1), value2(v2)
    {
    }
    static SingletonExample& instance()
    {
        return singleton::ServiceLocator<SingletonExample>::get();
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
    singleton::ServiceLocator<SingletonExample>::set(1, "2");
    EXPECT_EQ(1, SingletonExample::instance().value1);
    EXPECT_EQ("2", SingletonExample::instance().value2);
    EXPECT_EQ(1, singleton::ServiceLocator<SingletonExample>::get().value1);
    EXPECT_EQ("2", singleton::ServiceLocator<SingletonExample>::get().value2);

    singleton::ServiceLocator<SingletonExample>::set(3, "4");
    EXPECT_EQ(3, SingletonExample::instance().value1);
    EXPECT_EQ("4", SingletonExample::instance().value2);
    EXPECT_EQ(3, singleton::ServiceLocator<SingletonExample>::get().value1);
    EXPECT_EQ("4", singleton::ServiceLocator<SingletonExample>::get().value2);

    singleton::ServiceLocator<SingletonExample>::set(5, "6");
    EXPECT_EQ(5, SingletonExample::instance().value1);
    EXPECT_EQ("6", SingletonExample::instance().value2);
    EXPECT_EQ(5, singleton::ServiceLocator<SingletonExample>::get().value1);
    EXPECT_EQ("6", singleton::ServiceLocator<SingletonExample>::get().value2);
    singleton::ServiceLocator<SingletonExample>::reset();
}

TEST(SingletonExample, singletonExampleInstance)
{
    EXPECT_THROW(SingletonExample::instance(), std::runtime_error);
    EXPECT_THROW(singleton::ServiceLocator<SingletonExample>::get(), std::runtime_error);

    singleton::ServiceLocator<SingletonExample>::set(1, "2");
    EXPECT_NO_THROW(SingletonExample::instance());
    EXPECT_NO_THROW(singleton::ServiceLocator<SingletonExample>::get());

    singleton::ServiceLocator<SingletonExample>::reset();
    EXPECT_THROW(SingletonExample::instance(), std::runtime_error);
    EXPECT_THROW(singleton::ServiceLocator<SingletonExample>::get(), std::runtime_error);
}
