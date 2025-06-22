#include <gtest/gtest.h>
#include "Solution.h"
#include <algorithm> // For std::sort

TEST(Problem27Test, BasicTest)
{
    ssize_t testSize_t = 0;
    std::cout << "before: " << testSize_t << std::endl;
    testSize_t--;
    std::cout << "before: " << testSize_t << std::endl;
    Solution sol;

    // Test case 1
    vector<int> nums1 = {3, 2, 2, 3};
    int val1 = 3;
    int result1 = sol.removeElement(nums1, val1);
    EXPECT_EQ(result1, 2);                             // Expected length after removal
    std::sort(nums1.begin(), nums1.begin() + result1); // Sort the remaining elements
    vector<int> expected1 = {2, 2};
    for (int i = 0; i < result1; ++i)
    {
        EXPECT_EQ(nums1[i], expected1[i]);
    }

    // Test case 2
    vector<int> nums2 = {0, 1, 2, 2, 3, 0, 4, 2};
    int val2 = 2;
    int result2 = sol.removeElement(nums2, val2);
    EXPECT_EQ(result2, 5);                             // Expected length after removal
    std::sort(nums2.begin(), nums2.begin() + result2); // Sort the remaining elements
    vector<int> expected2 = {0, 0, 1, 3, 4};
    for (int i = 0; i < result2; ++i)
    {
        EXPECT_EQ(nums2[i], expected2[i]);
    }
}
