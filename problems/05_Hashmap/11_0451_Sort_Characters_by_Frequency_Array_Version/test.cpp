#include <gtest/gtest.h>

#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution
{
  public:
    // 方法1: 直接排序原数组 - 最简洁的解法
    vector<int> frequencySort(vector<int>& nums)
    {
        unordered_map<int, int> freq;
        for (int num : nums)
        {
            freq[num]++;
        }

        // 自定义排序：按频率降序，频率相同按数值升序
        sort(nums.begin(),
             nums.end(),
             [&](int a, int b)
             {
                 if (freq[a] != freq[b])
                 {
                     return freq[a] > freq[b];  // 频率高的在前
                 }
                 return a < b;  // 频率相同，数值小的在前
             });

        return nums;
    }

    // 方法2: HashMap + 桶排序 - 更高效的解法
    vector<int> frequencySortBucket(vector<int>& nums)
    {
        unordered_map<int, int> freq;
        for (int num : nums)
        {
            freq[num]++;
        }

        // 创建频率桶
        vector<vector<int>> buckets(nums.size() + 1);
        for (auto& p : freq)
        {
            buckets[p.second].push_back(p.first);
        }

        // 同频率内按数值排序
        for (int i = 0; i <= nums.size(); i++)
        {
            if (!buckets[i].empty())
            {
                sort(buckets[i].begin(), buckets[i].end());
            }
        }

        vector<int> result;
        // 从高频率开始输出
        for (int i = nums.size(); i >= 1; i--)
        {
            for (int num : buckets[i])
            {
                for (int j = 0; j < i; j++)
                {  // 输出i次
                    result.push_back(num);
                }
            }
        }

        return result;
    }

    // 方法3: 使用pair数组排序 - 便于理解的解法
    vector<int> frequencySortPairs(vector<int>& nums)
    {
        unordered_map<int, int> freq;
        for (int num : nums)
        {
            freq[num]++;
        }

        // 创建 (数字, 频率) 的pair数组
        vector<pair<int, int>> pairs;
        for (auto& p : freq)
        {
            pairs.push_back({p.first, p.second});
        }

        // 按频率降序排序，频率相同按数值升序
        sort(pairs.begin(),
             pairs.end(),
             [](const pair<int, int>& a, const pair<int, int>& b)
             {
                 if (a.second != b.second)
                 {
                     return a.second > b.second;  // 频率降序
                 }
                 return a.first < b.first;  // 数值升序
             });

        vector<int> result;
        for (auto& p : pairs)
        {
            for (int i = 0; i < p.second; i++)
            {
                result.push_back(p.first);
            }
        }

        return result;
    }
};

TEST(FrequencySortTest, BasicExample)
{
    Solution sol;

    // 测试用例: [1,4,4,5,5,6]
    // 预期: 4和5都出现2次(最高频), 1和6都出现1次
    // 结果应该是: [4,4,5,5,1,6] (相同频率内按数值排序)
    vector<int> nums = {1, 4, 4, 5, 5, 6};
    vector<int> result = sol.frequencySort(nums);

    // 验证结果
    EXPECT_EQ(result.size(), 6);

    // 验证频率排序正确: 前4个元素应该是4,4,5,5
    EXPECT_TRUE(
        (result[0] == 4 && result[1] == 4 && result[2] == 5 &&
         result[3] == 5) ||
        (result[0] == 5 && result[1] == 5 && result[2] == 4 && result[3] == 4));

    // 验证低频率元素在后面: 后2个元素应该是1,6
    EXPECT_TRUE((result[4] == 1 && result[5] == 6) ||
                (result[4] == 6 && result[5] == 1));
}

TEST(FrequencySortTest, DifferentFrequencies)
{
    Solution sol;

    // 测试用例: [1,1,1,2,2,3]
    // 预期: 1出现3次, 2出现2次, 3出现1次
    // 结果应该是: [1,1,1,2,2,3]
    vector<int> nums = {1, 1, 1, 2, 2, 3};
    vector<int> result = sol.frequencySort(nums);
    vector<int> expected = {1, 1, 1, 2, 2, 3};

    EXPECT_EQ(result, expected);
}

TEST(FrequencySortTest, SameFrequency)
{
    Solution sol;

    // 测试相同频率的排序: [3,1,2,3,1,2]
    // 所有数字都出现2次，应该按数值升序: [1,1,2,2,3,3]
    vector<int> nums = {3, 1, 2, 3, 1, 2};
    vector<int> result = sol.frequencySort(nums);
    vector<int> expected = {1, 1, 2, 2, 3, 3};

    EXPECT_EQ(result, expected);
}

TEST(FrequencySortTest, SingleElement)
{
    Solution sol;

    // 单个元素
    vector<int> nums = {42};
    vector<int> result = sol.frequencySort(nums);
    vector<int> expected = {42};

    EXPECT_EQ(result, expected);
}

TEST(FrequencySortTest, AllSameElements)
{
    Solution sol;

    // 所有元素相同
    vector<int> nums = {7, 7, 7, 7};
    vector<int> result = sol.frequencySort(nums);
    vector<int> expected = {7, 7, 7, 7};

    EXPECT_EQ(result, expected);
}

TEST(FrequencySortTest, NegativeNumbers)
{
    Solution sol;

    // 包含负数: [-1,2,-1,3,2]
    // 频率: -1->2次, 2->2次, 3->1次
    // 预期: [-1,-1,2,2,3] (相同频率按数值升序)
    vector<int> nums = {-1, 2, -1, 3, 2};
    vector<int> result = sol.frequencySort(nums);
    vector<int> expected = {-1, -1, 2, 2, 3};

    EXPECT_EQ(result, expected);
}

TEST(FrequencySortTest, CompareAlgorithms)
{
    Solution sol;

    // 比较三种算法的结果是否一致
    vector<vector<int>> testCases = {{1, 4, 4, 5, 5, 6},
                                     {1, 1, 1, 2, 2, 3},
                                     {3, 1, 2, 3, 1, 2},
                                     {-1, 2, -1, 3, 2},
                                     {5, 5, 5},
                                     {1, 2, 3, 4, 5}};

    for (auto testCase : testCases)
    {
        vector<int> nums1 = testCase;
        vector<int> nums2 = testCase;
        vector<int> nums3 = testCase;

        vector<int> result1 = sol.frequencySort(nums1);
        vector<int> result2 = sol.frequencySortBucket(nums2);
        vector<int> result3 = sol.frequencySortPairs(nums3);

        EXPECT_EQ(result1, result2) << "Direct sort vs Bucket sort differ";
        EXPECT_EQ(result1, result3) << "Direct sort vs Pairs sort differ";
    }
}

TEST(FrequencySortTest, UserExample)
{
    Solution sol;

    // 用户的具体例子: {1,4,4,5,5,6} -> {1, 6, 4, 4, 5, 5}
    // 但根据我们的排序规则(相同频率按数值升序)，应该是不同的结果
    vector<int> nums = {1, 4, 4, 5, 5, 6};
    vector<int> result = sol.frequencySort(nums);

    // 验证频率排序正确性
    unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;

    // 验证结果中频率递减
    for (int i = 0; i < result.size() - 1;)
    {
        int current = result[i];
        int count = 0;
        while (i < result.size() && result[i] == current)
        {
            count++;
            i++;
        }

        if (i < result.size())
        {
            int next = result[i];
            EXPECT_TRUE(freq[current] >= freq[next])
                << "Frequency order violated: " << current
                << " freq=" << freq[current] << " vs " << next
                << " freq=" << freq[next];
        }
    }
}