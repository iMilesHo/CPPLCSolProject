#include <gtest/gtest.h>
#include "Solution.h"

TEST(Problem76Test, BasicTest)
{
    Solution sol;
    EXPECT_EQ(sol.minWindow("ADOBECODEBANC", "ABC"), "BANC");
    EXPECT_EQ(sol.minWindow("a", "a"), "a");
    EXPECT_EQ(sol.minWindow("a", "aa"), "");
    EXPECT_EQ(sol.minWindow("a", "b"), "");
    EXPECT_EQ(sol.minWindow("ABCD", "ABCD"), "ABCD");
}

TEST(Problem76Test, EdgeCases)
{
    Solution sol;
    EXPECT_EQ(sol.minWindow("", ""), "");
    EXPECT_EQ(sol.minWindow("a", ""), "");
    EXPECT_EQ(sol.minWindow("", "a"), "");
    EXPECT_EQ(sol.minWindow("abc", "d"), "");
    EXPECT_EQ(sol.minWindow("abcde", "ace"), "abcde");
}

TEST(Problem76Test, TestASSCIICodeNumber)
{
    int a = static_cast<int>('a');
    int z = static_cast<int>('z');
    int A = static_cast<int>('A');
    int Z = static_cast<int>('Z');
    std::cout << "a: " << a << ", z: " << z << std::endl;
    std::cout << "A: " << A << ", Z: " << Z << std::endl;
}

TEST(Problem76Test, RandomTest)
{
    std::vector<int> x = {1, 2, 3};
    x.pop_back();
    std::cout << "pop_back()" << x[0] << std::endl;
}
