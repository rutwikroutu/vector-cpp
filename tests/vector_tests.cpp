#include<gtest/gtest.h>
#include "dev_vector.hpp"

TEST(VectorTest, DefaultConstructorCompiles) 
{
    dev::vector<int> v;
    SUCCEED();
}

TEST(VectorTest, DefaultConstructorCreatesEmptyVector) 
{
    dev::vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, VectorIsNotEmptyAfterPushBack)
{
    dev::vector<int> v;
    v.push_back(10);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v.size(), 1);
}

TEST(VectorTest, PushBackThreeElements)
{
    dev::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, CopyConstructorCopiesElements) {
    dev::vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);

    dev::vector<int> v2(v1); //copy constructor

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1[0], 10);
    EXPECT_EQ(v1[1], 20);
    EXPECT_EQ(v1[2], 30);

    //sanity check
    v2[0] = 99;
    EXPECT_EQ(v1[0], 10);
}

TEST(VectorTest, MoveConstructorStealsBuffer) {
    dev::vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);

    dev::vector<int> v2(std::move(v1));

    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 10);
    EXPECT_EQ(v2[1], 20);

    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, CopyAssignmentDeepCopies)
{
    dev::vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);

    dev::vector<int> v2;
    v2.push_back(9);

    v2 = v1;

    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 10);
    EXPECT_EQ(v2[1], 20);

    v2[0] = 99;
    EXPECT_EQ(v1[0], 10);
}

TEST(VectorTest, MoveAssignmentStealsBuffer)
{
    dev::vector<int> v1;
    v1.push_back(7);
    v1.push_back(8);

    dev::vector<int> v2;
    v2.push_back(100);

    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 7);
    EXPECT_EQ(v2[1], 8);

    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, BeginEndWorks) {
    dev::vector<int> v1;
    EXPECT_TRUE(v1.begin() == v1.end());

    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);

    int sum = 0;
    for(auto it = v1.begin(); it != v1.end(); ++it) {
        sum+= *it;
    }

    EXPECT_EQ(sum, 60);
}

TEST(VectorTest, FrontAndBack)
{
    dev::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
}

TEST(VectorTest, AtThrowsOutOfRange)
{
    dev::vector<int> v;
    v.push_back(1);
    v.push_back(2);

    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_THROW(v.at(2), std::out_of_range);
}

TEST(VectorTest, PopBack)
{
    dev::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.back(), 1);
}

TEST(VectorTest, ReserveDoesNotChangeSizeButIncreasesCapacity)
{
    dev::vector<int> v;
    v.push_back(1);
    v.push_back(2);

    auto old_size = v.size();
    auto old_capacity = v.capacity();

    v.reserve(old_capacity + 10);

    EXPECT_EQ(v.size(), old_size);
    EXPECT_GE(v.capacity(), old_capacity + 10);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, ResizeGrowsAndDefaultInitializes)
{
    dev::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.resize(5);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 0);  // default int
    EXPECT_EQ(v[4], 0);

    v.resize(2);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, EmplaceBackConstructsInPlace)
{
    struct Point {
        int x, y;
        Point(int a, int b) : x(a), y(b) {}
    };

    dev::vector<Point> v;
    v.emplace_back(1, 2);
    v.emplace_back(3, 4);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].x, 1);
    EXPECT_EQ(v[0].y, 2);
    EXPECT_EQ(v[1].x, 3);
    EXPECT_EQ(v[1].y, 4);
}

TEST(VectorTest, InitializerListConstructor)
{
    dev::vector<int> v{1,2,3,4,5};
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, FillConstructor)
{
    dev::vector<int> v(5, 42);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);

    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST(VectorTest, EqualityOperator)
{
    dev::vector<int> a{1,2,3};
    dev::vector<int> b{1,2,3};
    dev::vector<int> c{1,2,4};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(VectorTest, ClearDestroysElementsButKeepsCapacity)
{
    dev::vector<int> v{1, 2, 3, 4};
    auto old_capacity = v.capacity();

    v.clear();

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), old_capacity);

    // vector should still be usable
    v.push_back(10);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 10);
}

TEST(VectorTest, ShrinkToFitReducesCapacity)
{
    dev::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.reserve(20);
    EXPECT_GT(v.capacity(), v.size());

    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, ShrinkToFitOnEmpty)
{
    dev::vector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);

    v.clear();
    EXPECT_EQ(v.size(), 0);

    v.shrink_to_fit();
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);

    v.push_back(5);
    EXPECT_EQ(v[0], 5);
}

TEST(VectorTest, ClearThenResize)
{
    dev::vector<int> v{1,2,3};
    v.clear();
    EXPECT_EQ(v.size(), 0);

    v.resize(3);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 0);
}
