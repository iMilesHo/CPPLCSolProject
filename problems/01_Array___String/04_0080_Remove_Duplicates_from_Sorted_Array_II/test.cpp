#include <gtest/gtest.h>
#include "Solution.h"

TEST(Problem80Test, BasicTest)
{
    Solution sol;
    vector<int> nums1 = {1, 1, 1, 2, 2, 3};
    int result1 = sol.removeDuplicates(nums1);
    EXPECT_EQ(result1, 5); // Expected length after removal
    vector<int> expected1 = {1, 1, 2, 2, 3, 3};
    EXPECT_EQ(expected1, nums1);
}
