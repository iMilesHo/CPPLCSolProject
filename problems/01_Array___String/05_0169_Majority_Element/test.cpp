#include <gtest/gtest.h>
#include "Solution.h"

TEST(Problem169Test, BasicTest) {
    Solution sol;
    // TODO: Add your basic tests here
    vector<int> nums1 = {3,2,3};
    EXPECT_EQ(sol.majorityElement(nums1), 3);

    vector<int> nums2 = {2,2,1,1,1,2,2};
    EXPECT_EQ(sol.majorityElement(nums2), 2);

    // 
    auto length = nums1.size();
    int div = length / 2;
    int div2 = length / -2;

    std::cout << "test /: " << div << std::endl;
    std::cout << "test /: " << div2 << std::endl;
}
