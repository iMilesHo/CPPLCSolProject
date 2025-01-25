#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>
#include <functional>
#include "Solution.h"
#include "printer.hpp"



TEST(SolutionTest, ArrayTest) {
    Solution sol {};
    // std::vector<int> input {0,0,1,1,1,2,2,3,3,4};
    // std::cout << "k: " << sol.removeDuplicates(input) << std::endl;
    // std::cout <<  "arr: " << input << std::endl;

    std::vector<int> input {1};
    std::cout << "k: " << sol.removeDuplicates(input) << std::endl;
    std::cout <<  "arr: " << input << std::endl;

}