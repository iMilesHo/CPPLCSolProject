#include <gtest/gtest.h>
#include "Solution.h"
#include "../../../include/printer.hpp"  // Optional, for printing vectors

// unit tests for the Solution methods
TEST(ValidAnagramTest, Test_1) {
    Solution* sol = new Solution();
    std::string s = "anagram";
    std::string t = "nagrama";
    bool res = sol->isAnagram(s, t);
    ASSERT_TRUE(res);
}
