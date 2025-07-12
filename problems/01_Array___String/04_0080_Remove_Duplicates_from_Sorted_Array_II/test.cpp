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

    vector<int> nums2{1, 1, 2};
    int r2 = sol.removeDuplicates(nums2);
    std::cout << "r = " << r2 << endl;
    for (auto i : nums2)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
