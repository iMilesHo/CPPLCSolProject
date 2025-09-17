#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
// #include "Solution.h"

using namespace std;


class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        vector<bool> dp(n, false);
        dp[0] = true; // start is always reachable

        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (dp[j] && j + nums[j] >= i) {
                    dp[i] = true;
                    break; // no need to check further
                }
            }
        }

        return dp[n - 1];
    }
};


TEST(Problem55Test, BasicTest) {
    Solution sol;
    vector<int> nums1 = {2,3,1,1,4};
    EXPECT_EQ(sol.canJump(nums1), true);
    vector<int> nums2 = {3,2,1,0,4};
    EXPECT_EQ(sol.canJump(nums2), false);
}
