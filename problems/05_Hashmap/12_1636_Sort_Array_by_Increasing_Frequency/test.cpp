#include <gtest/gtest.h>

#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution
{
  public:
    vector<int> frequencySort(vector<int>& nums)
    {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        sort(nums.begin(),
             nums.end(),
             [&](int a, int b)
             {
                 if (freq[a] != freq[b]) return freq[a] < freq[b];
                 return a > b;
             });
        return nums;
    }
};

TEST(Problem1636Test, BasicTest)
{
    Solution sol;
    vector<int> nums1 = {1, 1, 2, 2, 2, 3};
    vector<int> expected1 = {3, 1, 1, 2, 2, 2};
    EXPECT_EQ(sol.frequencySort(nums1), expected1);

    vector<int> nums2 = {2, 3, 1, 3, 2};
    vector<int> expected2 = {1, 3, 3, 2, 2};
    EXPECT_EQ(sol.frequencySort(nums2), expected2);

    vector<int> nums3 = {-1, 1, -6, 4, 5, -6, 1, 4, 1};
    vector<int> expected3 = {5, -1, 4, 4, -6, -6, 1, 1, 1};
    EXPECT_EQ(sol.frequencySort(nums3), expected3);
}

TEST(Problem1636Test, EdgeCases)
{
    Solution sol;
    // 单元素
    vector<int> nums1 = {42};
    vector<int> expected1 = {42};
    EXPECT_EQ(sol.frequencySort(nums1), expected1);
    // 全部相同
    vector<int> nums2 = {7, 7, 7, 7};
    vector<int> expected2 = {7, 7, 7, 7};
    EXPECT_EQ(sol.frequencySort(nums2), expected2);
    // 频率相同，数值不同
    vector<int> nums3 = {1, 2, 3, 4};
    vector<int> expected3 = {4, 3, 2, 1};
    EXPECT_EQ(sol.frequencySort(nums3), expected3);
}
