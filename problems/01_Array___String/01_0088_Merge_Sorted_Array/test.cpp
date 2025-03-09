#include <gtest/gtest.h>
#include "Solution.h"

TEST(Problem88Test, BasicTest)
{
    Solution sol;
    // TODO: Add your basic tests here
    std::vector<int> nums1 = {1, 2, 3, 0, 0, 0};
    std::vector<int> nums2 = {2, 5, 6};
    sol.merge(nums1, 3, nums2, 3);
    std::vector<int> expected = {1, 2, 2, 3, 5, 6};
    EXPECT_EQ(nums1, expected);
}
