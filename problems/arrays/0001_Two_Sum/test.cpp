// problems/arrays/0001_Two_Sum/test.cpp
#include <gtest/gtest.h>
#include "Solution.h"
#include "../../../include/printer.hpp"  // Optional, for printing vectors

TEST(TwoSumTest, BasicTest) {
    Solution sol;
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> expected = {0, 1};
    EXPECT_EQ(sol.twoSum(nums, target), expected);
}

TEST(TwoSumTest, SinglePair) {
    Solution sol;
    std::vector<int> nums = {3, 2, 4};
    int target = 6;
    std::vector<int> expected = {1, 2};
    EXPECT_EQ(sol.twoSum(nums, target), expected);
}