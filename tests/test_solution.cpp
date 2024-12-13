#include <gtest/gtest.h>
#include <vector>
#include "Solution.h"
#include "printer.hpp"

TEST(SolutionTest, BasicTest) {
    Solution sol;
    std::vector<int> nums = {3, 2, 4};
    int target = 6;
    std::cout << "result: " << sol.twoSum(nums, target) << std::endl;
    sol.test();
}