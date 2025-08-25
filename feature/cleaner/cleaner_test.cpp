#include "cleaner.hpp"
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
    int callCount()
    {
        int copy = call_count;
        call_count = 0;
        return copy;
    }
private:
    int call_count;
};

class DynamicCleanerTest : public ::testing::Test
{
protected:
    void TearDown() override
    {
        cleaner::DynamicCleaner::clear();
    }
};

TEST_F(DynamicCleanerTest, BasicRegistrationAndCall)
{
    TestObject object{};
    cleaner::DynamicCleaner::registrate(&object, &TestObject::call);
    EXPECT_EQ(object.callCount(), 0);
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object.callCount(), 1);
    cleaner::DynamicCleaner::call();
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object.callCount(), 2);
}

TEST_F(DynamicCleanerTest, RegistrateDifferentObjects) {
    TestObject object1{};
    TestObject object2{};
    TestObject object3{};
    cleaner::DynamicCleaner::registrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::registrate(&object2, &TestObject::call);
    cleaner::DynamicCleaner::registrate(&object3, &TestObject::call);
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object1.callCount(), 1);
    EXPECT_EQ(object2.callCount(), 1);
    EXPECT_EQ(object3.callCount(), 1);
}

TEST_F(DynamicCleanerTest, BasicUnregistrationAndCall) {
    TestObject object1{};
    TestObject object2{};
    cleaner::DynamicCleaner::registrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::unregistrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object1.callCount(), 0);
}

TEST_F(DynamicCleanerTest, UnregistrateDifferentObjects) {
    TestObject object1{};
    TestObject object2{};
    cleaner::DynamicCleaner::registrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::registrate(&object2, &TestObject::call);
    cleaner::DynamicCleaner::unregistrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object1.callCount(), 0);
    EXPECT_EQ(object2.callCount(), 1);
}

TEST_F(DynamicCleanerTest, MultipleCalls) {
    TestObject object1{};
    cleaner::DynamicCleaner::registrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::call();
    cleaner::DynamicCleaner::call();
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object1.callCount(), 3);
}

TEST_F(DynamicCleanerTest, ClearRegistrationList) {
    TestObject object1{};
    TestObject object2{};
    TestObject object3{};
    cleaner::DynamicCleaner::registrate(&object1, &TestObject::call);
    cleaner::DynamicCleaner::registrate(&object2, &TestObject::call);
    cleaner::DynamicCleaner::registrate(&object3, &TestObject::call);
    cleaner::DynamicCleaner::clear();
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object1.callCount(), 0);
    EXPECT_EQ(object2.callCount(), 0);
    EXPECT_EQ(object3.callCount(), 0);
}

TEST_F(DynamicCleanerTest, ReregistrateObject) {
    TestObject object{};
    cleaner::DynamicCleaner::registrate(&object, &TestObject::call);
    cleaner::DynamicCleaner::unregistrate(&object, &TestObject::call);
    cleaner::DynamicCleaner::registrate(&object, &TestObject::call);
    cleaner::DynamicCleaner::call();
    EXPECT_EQ(object.callCount(), 1);
}