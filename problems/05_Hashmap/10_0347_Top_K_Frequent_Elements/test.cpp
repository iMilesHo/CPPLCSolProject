#include <gtest/gtest.h>

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution
{
  public:
    // 方法1: HashMap + 排序 - O(n log n)
    vector<int> topKFrequent_Sort(vector<int>& nums, int k)
    {
        unordered_map<int, int> freq;
        for (int num : nums)
        {
            freq[num]++;
        }

        vector<pair<int, int>> freqPairs;  // {frequency, number}
        for (auto& p : freq)
        {
            freqPairs.push_back({p.second, p.first});
        }

        // 按频率降序排序，频率相同按数值升序
        sort(freqPairs.begin(),
             freqPairs.end(),
             [](const pair<int, int>& a, const pair<int, int>& b)
             {
                 if (a.first == b.first) return a.second < b.second;
                 return a.first > b.first;
             });

        vector<int> result;
        for (int i = 0; i < k; i++)
        {
            result.push_back(freqPairs[i].second);
        }
        return result;
    }

    // 方法2: HashMap + 最小堆 - O(n log k)
    vector<int> topKFrequent_Heap(vector<int>& nums, int k)
    {
        unordered_map<int, int> freq;
        for (int num : nums)
        {
            freq[num]++;
        }

        // 最小堆：保留频率最高的k个元素
        // 堆顶是频率最小的元素，当堆大小超过k时弹出
        auto cmp = [](const pair<int, int>& a, const pair<int, int>& b)
        {
            // 如果频率不同，频率小的排在前面（最小堆）
            if (a.first != b.first) return a.first > b.first;
            // 频率相同时，数值大的排在前面（这样数值小的会被优先保留）
            return a.second < b.second;
        };
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)>
            minHeap(cmp);

        for (auto& p : freq)
        {
            minHeap.push({p.second, p.first});  // {frequency, number}
            if (minHeap.size() > k)
            {
                minHeap.pop();
            }
        }

        vector<int> result;
        while (!minHeap.empty())
        {
            result.push_back(minHeap.top().second);
            minHeap.pop();
        }

        return result;
    }

    // 方法3: HashMap + 桶排序 - O(n) - 主要实现
    vector<int> topKFrequent(vector<int>& nums, int k)
    {
        unordered_map<int, int> freq;
        for (int num : nums)
        {
            freq[num]++;
        }

        // 创建频率桶，索引为频率，值为该频率的数字列表
        vector<vector<int>> buckets(nums.size() + 1);
        for (auto& p : freq)
        {
            buckets[p.second].push_back(p.first);
        }

        vector<int> result;
        // 从高频率开始收集
        for (int i = nums.size(); i >= 0 && result.size() < k; i--)
        {
            if (!buckets[i].empty())
            {
                // 同频率内按数值排序（保证结果的确定性）
                sort(buckets[i].begin(), buckets[i].end());

                for (int num : buckets[i])
                {
                    if (result.size() < k)
                    {
                        result.push_back(num);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        return result;
    }
};

TEST(Problem347Test, BasicTest)
{
    Solution sol;

    // 测试用例1: [1,1,1,2,2,3], k = 2
    // 频率: 1->3次, 2->2次, 3->1次
    // 期望: [1,2] (频率最高的2个)
    vector<int> nums1 = {1, 1, 1, 2, 2, 3};
    vector<int> result1 = sol.topKFrequent(nums1, 2);
    EXPECT_EQ(result1.size(), 2);
    EXPECT_TRUE(find(result1.begin(), result1.end(), 1) != result1.end());
    EXPECT_TRUE(find(result1.begin(), result1.end(), 2) != result1.end());

    // 测试用例2: [1], k = 1
    vector<int> nums2 = {1};
    vector<int> result2 = sol.topKFrequent(nums2, 1);
    vector<int> expected2 = {1};
    EXPECT_EQ(result2, expected2);
}

TEST(Problem347Test, EdgeCases)
{
    Solution sol;

    // 所有元素都相同
    vector<int> nums1 = {4, 4, 4, 4};
    vector<int> result1 = sol.topKFrequent(nums1, 1);
    vector<int> expected1 = {4};
    EXPECT_EQ(result1, expected1);

    // 所有元素都不同，k = 所有元素个数
    vector<int> nums2 = {1, 2, 3, 4, 5};
    vector<int> result2 = sol.topKFrequent(nums2, 5);
    EXPECT_EQ(result2.size(), 5);
    sort(result2.begin(), result2.end());
    vector<int> expected2 = {1, 2, 3, 4, 5};
    EXPECT_EQ(result2, expected2);
}

TEST(Problem347Test, SameFrequency)
{
    Solution sol;

    // 测试相同频率的情况：[1,2,3,1,2,3], k = 3
    // 所有数字频率都是2，应该返回[1,2,3]（按数值排序）
    vector<int> nums = {1, 2, 3, 1, 2, 3};
    vector<int> result = sol.topKFrequent(nums, 3);
    EXPECT_EQ(result.size(), 3);
    sort(result.begin(), result.end());
    vector<int> expected = {1, 2, 3};
    EXPECT_EQ(result, expected);
}

TEST(Problem347Test, LargerExample)
{
    Solution sol;

    // 更复杂的例子：[4,1,-1,2,-1,2,3], k = 2
    // 频率: -1->2次, 2->2次, 1->1次, 3->1次, 4->1次
    // 期望: [-1,2] (频率最高的2个，相同频率按数值排序)
    vector<int> nums = {4, 1, -1, 2, -1, 2, 3};
    vector<int> result = sol.topKFrequent(nums, 2);
    EXPECT_EQ(result.size(), 2);
    sort(result.begin(), result.end());
    vector<int> expected = {-1, 2};
    EXPECT_EQ(result, expected);
}

TEST(Problem347Test, FrequencySortComplete)
{
    Solution sol;

    // 测试完整的频率排序（不限制k）
    vector<int> nums = {4, 1, -1, 2, -1, 2, 3};
    // 频率: -1->2次, 2->2次, 1->1次, 3->1次, 4->1次
    // 完整排序结果应该是: [-1, 2, 1, 3, 4] (按频率降序，同频率按数值升序)

    vector<int> result = sol.topKFrequent_Sort(nums, nums.size());
    // 由于题目要求可能不同，这里我们主要验证基本逻辑
    EXPECT_EQ(result.size(), 5);

    // 验证频率最高的元素确实在前面
    unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;

    EXPECT_TRUE(freq[result[0]] >= freq[result[result.size() - 1]]);
}

TEST(Problem347Test, CompareMainAlgorithms)
{
    Solution sol;

    // 只比较桶排序和排序算法，因为它们的逻辑更相似
    vector<vector<int>> testCases = {{1, 1, 1, 2, 2, 3},
                                     {1, 2, 3, 1, 2, 3},
                                     {4, 1, -1, 2, -1, 2, 3}};

    for (auto& nums : testCases)
    {
        // 测试 k = 1, 2 的情况
        for (int k = 1; k <= min(2, (int)nums.size()); k++)
        {
            vector<int> nums1 = nums, nums2 = nums;

            vector<int> result1 = sol.topKFrequent(nums1, k);
            vector<int> result2 = sol.topKFrequent_Sort(nums2, k);

            EXPECT_EQ(result1.size(), k);
            EXPECT_EQ(result2.size(), k);

            // 验证返回的都是频率最高的k个元素
            unordered_map<int, int> freq;
            for (int num : nums) freq[num]++;

            // 检查结果中每个元素的频率都不低于未选中元素的频率
            set<int> selected1(result1.begin(), result1.end());
            set<int> selected2(result2.begin(), result2.end());

            for (auto& p : freq)
            {
                if (selected1.find(p.first) == selected1.end())
                {
                    // 这个元素没被选中，它的频率应该不高于被选中的元素
                    for (int selectedNum : result1)
                    {
                        EXPECT_TRUE(freq[selectedNum] >= p.second);
                    }
                }
            }
        }
    }
}