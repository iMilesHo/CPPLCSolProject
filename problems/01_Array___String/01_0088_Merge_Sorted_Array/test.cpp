#include <gtest/gtest.h>
#include "Solution.h"

/*
Example 1:

Input: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
Output: [1,2,2,3,5,6]
Explanation: The arrays we are merging are [1,2,3] and [2,5,6].
The result of the merge is [1,2,2,3,5,6] with the underlined elements coming from nums1.
Example 2:

Input: nums1 = [1], m = 1, nums2 = [], n = 0
Output: [1]
Explanation: The arrays we are merging are [1] and [].
The result of the merge is [1].
Example 3:

Input: nums1 = [0], m = 0, nums2 = [1], n = 1
Output: [1]
Explanation: The arrays we are merging are [] and [1].
The result of the merge is [1].
Note that because m = 0, there are no elements in nums1. The 0 is only there to ensure the merge result can fit in nums1.
*/

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

TEST(Problem88Test, EdgeCase)
{
    Solution sol;
    std::vector<int> nums1 = {0};
    std::vector<int> nums2 = {};
    sol.merge(nums1, 1, nums2, 0);
    std::vector<int> expected = {0};
    EXPECT_EQ(nums1, expected);
}

TEST(Problem88Test, AnotherEdgeCase)
{
    Solution sol;
    std::vector<int> nums1 = {0};
    std::vector<int> nums2 = {1};
    sol.merge(nums1, 0, nums2, 1);
    std::vector<int> expected = {1};
    EXPECT_EQ(nums1, expected);
}
