#include "caller.hpp"
#include <gtest/gtest.h>

struct TestObject
{
    TestObject() : call_count(0)
    {
    }
    void call()
    {
        call_count++;
    }
    int callCount() const
    {
        return call_count;
    }
private:
    int call_count;
};

TEST(DynamicCallerTest, BasicFunctionality)
{
    caller::Caller caller{};
    std::shared_ptr<TestObject> object = std::make_shared<TestObject>();
    caller.add(object, &TestObject::call);
    EXPECT_EQ(object->callCount(), 0);
    caller();
    EXPECT_EQ(object->callCount(), 1);
    caller();
    EXPECT_EQ(object->callCount(), 2);
    caller.remove(object, &TestObject::call);
    caller();
    EXPECT_EQ(object->callCount(), 2);
}
