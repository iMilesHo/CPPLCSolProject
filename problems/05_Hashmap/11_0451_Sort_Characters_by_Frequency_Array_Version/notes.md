# Sort Array by Frequency

- **ID:** 451 (Array Version)
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Hash Table, Sorting, Counting
- **Link:** Based on [LeetCode Problem 451](https://leetcode.com/problems/sort-characters-by-frequency/description/)

## Problem Description

给定一个整数数组，按照每个数字出现的频率对数组进行排序。频率高的数字排在前面，频率相同的数字按照数值大小升序排列。

**Example 1:**

```
Input: nums = [1,4,4,5,5,6]
Output: [4,4,5,5,1,6] 或 [5,5,4,4,1,6]
说明: 4和5都出现2次(频率最高), 1和6都出现1次(频率最低)
```

**Example 2:**

```
Input: nums = [1,1,1,2,2,3]
Output: [1,1,1,2,2,3]
说明: 1出现3次, 2出现2次, 3出现1次
```

**Example 3:**

```
Input: nums = [2,3,1,3,2]
Output: [2,2,3,3,1] 或 [3,3,2,2,1]
说明: 2和3都出现2次, 1出现1次
```

## 解题思路

### 方法 1: HashMap + 自定义排序

1. 统计每个数字的出现频率
2. 创建 (数字, 频率) 的 pair 数组
3. 根据频率排序，频率相同按数值排序
4. 按照排序结果重构数组

### 方法 2: HashMap + 桶排序

1. 统计频率
2. 创建频率桶，按频率分组
3. 从高频率桶开始，依次输出对应次数的数字

## Implementation

```cpp
class Solution {
public:
    // 方法1: 直接排序原数组
    vector<int> frequencySort(vector<int>& nums) {
        unordered_map<int, int> freq;
        for (int num : nums) {
            freq[num]++;
        }

        // 自定义排序：按频率降序，频率相同按数值升序
        sort(nums.begin(), nums.end(), [&](int a, int b) {
            if (freq[a] != freq[b]) {
                return freq[a] > freq[b];  // 频率高的在前
            }
            return a < b;  // 频率相同，数值小的在前
        });

        return nums;
    }

    // 方法2: HashMap + 桶排序
    vector<int> frequencySortBucket(vector<int>& nums) {
        unordered_map<int, int> freq;
        for (int num : nums) {
            freq[num]++;
        }

        // 创建频率桶
        vector<vector<int>> buckets(nums.size() + 1);
        for (auto& p : freq) {
            buckets[p.second].push_back(p.first);
        }

        // 同频率内按数值排序
        for (int i = 0; i <= nums.size(); i++) {
            sort(buckets[i].begin(), buckets[i].end());
        }

        vector<int> result;
        // 从高频率开始输出
        for (int i = nums.size(); i >= 1; i--) {
            for (int num : buckets[i]) {
                for (int j = 0; j < i; j++) {  // 输出i次
                    result.push_back(num);
                }
            }
        }

        return result;
    }
};
```

## Complexity Analysis

### 方法 1: 直接排序

- **Time Complexity:** O(n log n)
  - 频率统计: O(n)
  - 排序: O(n log n)
- **Space Complexity:** O(k) - k 为不同数字的个数，用于频率 map

### 方法 2: 桶排序

- **Time Complexity:** O(n + k log k)
  - 频率统计: O(n)
  - 桶内排序: O(k log k) (k 为不同数字个数)
  - 输出结果: O(n)
- **Space Complexity:** O(n + k) - 桶数组和频率 map 的空间

## Key Insights

1. **频率统计是基础**: 使用 HashMap 快速统计每个数字的出现次数
2. **排序策略选择**:
   - 直接排序: 代码简洁，适合大多数情况
   - 桶排序: 当频率范围较小时更优
3. **处理相同频率**: 题目要求相同频率按数值排序，保证结果的确定性
4. **空间优化**: 可以直接在原数组上排序，节省空间

## Related Problems

- LeetCode 451: Sort Characters By Frequency
- LeetCode 347: Top K Frequent Elements
- LeetCode 692: Top K Frequent Words
