#include <gtest/gtest.h>
// #include "Solution.h"
#include <algorithm>
#include <vector>
#include <numeric>


class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        const auto n = static_cast<int>(nums.size());
        if (n == 0) return;
        k %= n;
        if (k == 0) return;

        // groups: gcd(n,k);
        // per groups length: n / gcd(n, k);

        const int groups = std::gcd(n,k);
        for (int startI = 0; startI < groups; ++startI)
        { 
            int prevV = nums[startI];
            int i = (startI + k) % n;
            while (i != startI)
            {
                int temp = nums[i];
                nums[i] = prevV;
                prevV = temp;
                i = (i+k) % n;
            }
            nums[startI] = prevV;
        }
    }
};


TEST(Problem189Test, BasicTest) {
    Solution s;
    {
        std::vector<int> input{1,2,3,4,5,6};
        s.rotate(input, 2);
        std::vector<int> expect{5,6,1,2,3,4};
        EXPECT_EQ(expect, input);
    }
    {
        std::vector<int> input{1,2,3,4,5,6};
        s.rotate(input, 0);
        std::vector<int> expect{1,2,3,4,5,6};
        EXPECT_EQ(expect, input);
    }
    {
        std::vector<int> input{1,2,3,4,5,6};
        s.rotate(input, 6);
        std::vector<int> expect{1,2,3,4,5,6};
        EXPECT_EQ(expect, input);
    }
    {
        std::vector<int> input{1};
        s.rotate(input, 0);
        std::vector<int> expect{1};
        EXPECT_EQ(expect, input);
    }
    {
        std::vector<int> input{1};
        s.rotate(input, 1);
        std::vector<int> expect{1};
        EXPECT_EQ(expect, input);
    }
    {
        std::vector<int> input{};
        s.rotate(input, 1);
        std::vector<int> expect{};
        EXPECT_EQ(expect, input);
    }
    
}

