#include <gtest/gtest.h>
#include "Solution.h"

using namespace std;

TEST(Problem26Test, StdMethod)
{
    vector<int> nums = {1, 1, 2, 2, 3};
    std::cout << "result: " << std::distance(nums.begin(), std::unique(nums.begin(), nums.end())) << std::endl;
    // print out the changed vector
    for (auto it = nums.begin(); it != nums.end(); ++it)
    {
        std::cout << *it << " ";
    }
}