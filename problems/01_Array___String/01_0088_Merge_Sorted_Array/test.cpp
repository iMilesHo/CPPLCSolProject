#include <gtest/gtest.h>
#include "Solution.h"

void bar()
{
    // int *p = new int(123); // control block for `123` is on the heap
    // std::string *sp = new std::string("Hello, heap!");
    int *p = new int(123); // control block for `123` is on the heap
    std::string *sp = new std::string("Hello, heap!");
    // …
    delete p; // you MUST free what you allocate
    delete sp;
}

TEST(Problem88Test, TestStdInplaceMerge)
{
    std::vector<int> nums{1, 3, 5, 7, 4, 6};

    std::inplace_merge(nums.begin(), nums.begin() + 4, nums.end());
    std::vector<int> expected{1, 3, 4, 5, 6, 7};
    EXPECT_EQ(nums, expected);
}

TEST(Problem88Test, TestMergeSortedArray)
{
    std::vector<int> nums1{1, 2, 3};
    std::vector<int> nums2{2, 5, 6};
    std::vector<int> result(nums1.size() + nums2.size()); // will automatically initialize with zeros

    for (auto i : result)
        std::cout << i << " ";

    std::merge(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), result.begin());

    std::vector<int> expected{1, 2, 2, 3, 5, 6};
    EXPECT_EQ(result, expected);
}

void testStatic()
{
    static int x = 0;
    x++;
    std::cout << "x: " << x << std::endl;
}

TEST(Problem88Test, test2)
{
    testStatic();
    testStatic();
    testStatic();
}