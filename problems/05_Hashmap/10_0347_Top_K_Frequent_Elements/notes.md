# Top K Frequent Elements

- **ID:** 347
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Hash Table, Divide and Conquer, Sorting, Heap (Priority Queue), Bucket Sort, Counting, Quickselect
- **Link:** [LeetCode Problem](https://leetcode.com/problems/top-k-frequent-elements/description/)

## Problem Description

Given an integer array `nums` and an integer `k`, return the `k` most frequent elements. You may return the answer in **any order**.

**Example 1:**

```
Input: nums = [1,1,1,2,2,3], k = 2
Output: [1,2]
```

**Example 2:**

```
Input: nums = [1], k = 1
Output: [1]
```

**Constraints:**

- `1 <= nums.length <= 10^5`
- `-10^4 <= nums[i] <= 10^4`
- `k` is in the range `[1, the number of unique elements in the array]`
- It is **guaranteed** that the answer is **unique**

**Follow up:** Your algorithm's time complexity must be better than `O(n log n)`, where n is the array's size.

## Approach

这个问题有多种解法，每种都有不同的时间和空间复杂度：

### 方法 1: HashMap + 排序

1. 用 HashMap 统计每个数字的频率
2. 将频率和数字作为 pair 存入 vector
3. 按频率排序，频率相同按数值排序
4. 取前 k 个

### 方法 2: HashMap + 最小堆

1. 用 HashMap 统计频率
2. 维护大小为 k 的最小堆
3. 遍历频率 map，维护堆中 k 个最高频率的元素

### 方法 3: HashMap + 桶排序

1. 用 HashMap 统计频率
2. 创建频率桶（数组），索引为频率，值为该频率的数字列表
3. 从高频率桶开始收集，直到收集 k 个数字

### 方法 4: HashMap + 快速选择

1. 用 HashMap 统计频率
2. 使用快速选择算法找到第 k 大的频率
3. 收集所有频率不小于第 k 大频率的元素

## Implementation

```cpp
class Solution {
public:
    // 方法1: HashMap + 排序 - O(n log n)
    vector<int> topKFrequent_Sort(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) {
            freq[num]++;
        }

        vector<pair<int, int>> freqPairs;  // {frequency, number}
        for (auto& p : freq) {
            freqPairs.push_back({p.second, p.first});
        }

        // 按频率降序排序，频率相同按数值升序
        sort(freqPairs.begin(), freqPairs.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
            if (a.first == b.first) return a.second < b.second;
            return a.first > b.first;
        });

        vector<int> result;
        for (int i = 0; i < k; i++) {
            result.push_back(freqPairs[i].second);
        }
        return result;
    }

    // 方法2: HashMap + 最小堆 - O(n log k)
    vector<int> topKFrequent_Heap(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) {
            freq[num]++;
        }

        // 最小堆：按频率排序，频率相同按数值降序（保证相同频率时较小数值优先被移除）
        auto cmp = [](const pair<int,int>& a, const pair<int,int>& b) {
            if (a.first == b.first) return a.second > b.second;
            return a.first > b.first;
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> minHeap(cmp);

        for (auto& p : freq) {
            minHeap.push({p.second, p.first});  // {frequency, number}
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }

        vector<int> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top().second);
            minHeap.pop();
        }

        return result;
    }

    // 方法3: HashMap + 桶排序 - O(n)
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) {
            freq[num]++;
        }

        // 创建频率桶，索引为频率，值为该频率的数字列表
        vector<vector<int>> buckets(nums.size() + 1);
        for (auto& p : freq) {
            buckets[p.second].push_back(p.first);
        }

        vector<int> result;
        // 从高频率开始收集
        for (int i = nums.size(); i >= 0 && result.size() < k; i--) {
            if (!buckets[i].empty()) {
                // 同频率内按数值排序（可选，题目不要求特定顺序）
                sort(buckets[i].begin(), buckets[i].end());

                for (int num : buckets[i]) {
                    if (result.size() < k) {
                        result.push_back(num);
                    } else {
                        break;
                    }
                }
            }
        }

        return result;
    }
};
```

## Complexity Analysis

### 方法 1: HashMap + 排序

- **Time Complexity:** O(n log n)
  - HashMap 统计: O(n)
  - 排序: O(n log n)
- **Space Complexity:** O(n) - HashMap 和排序数组的空间

### 方法 2: HashMap + 最小堆

- **Time Complexity:** O(n log k)
  - HashMap 统计: O(n)
  - 堆操作: O(n log k)
- **Space Complexity:** O(n + k) - HashMap + 堆的空间

### 方法 3: HashMap + 桶排序

- **Time Complexity:** O(n)
  - HashMap 统计: O(n)
  - 桶排序: O(n)
- **Space Complexity:** O(n) - HashMap 和桶数组的空间

## Key Insights

1. **频率统计是核心**：所有方法都需要先用 HashMap 统计频率
2. **选择合适的排序策略**：
   - 完全排序：简单但可能过度（O(n log n)）
   - 堆排序：适合 k 较小的情况（O(n log k)）
   - 桶排序：最优解，利用频率范围有限的特点（O(n)）
3. **处理相同频率**：题目不要求特定顺序，但实际应用中可能需要额外排序规则

## Related Problems

- LeetCode 692: Top K Frequent Words
- LeetCode 451: Sort Characters By Frequency
- LeetCode 973: K Closest Points to Origin
