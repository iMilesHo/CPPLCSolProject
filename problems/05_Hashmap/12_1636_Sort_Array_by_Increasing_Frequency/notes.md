# 1636. Sort Array by Increasing Frequency

- **ID:** 1636
- **Difficulty:** EASY
- **Topic Tags:** Array, Hash Table, Sorting
- **Link:** [LeetCode Problem](https://leetcode.com/problems/sort-array-by-increasing-frequency/)

## Problem Description

Given an array of integers `nums`, sort the array in **increasing order based on the frequency** of the values. If multiple values have the same frequency, sort them in **decreasing order**.

**Example 1:**

```
Input: nums = [1,1,2,2,2,3]
Output: [3,1,1,2,2,2]
Explanation: '3' has a frequency of 1, '1' has a frequency of 2, and '2' has a frequency of 3.
```

**Example 2:**

```
Input: nums = [2,3,1,3,2]
Output: [1,3,3,2,2]
Explanation: '1' has a frequency of 1, '2' and '3' have a frequency of 2. '3' should come before '2' in decreasing order.
```

**Example 3:**

```
Input: nums = [-1,1,-6,4,5,-6,1,4,1]
Output: [5,-1,4,4,-6,-6,1,1,1]
```

## My solution

```cpp
class Solution {
public:
    vector<int> frequencySort(vector<int>& nums) {
        map<int, int> valueFreqPair{};
        for(auto x: nums)
        {
            valueFreqPair[x] += 1;
        }

        map<int, vector<int>> freqValuesPair{};
        for(auto [value, freq]: valueFreqPair)
        {
            freqValuesPair[freq].insert(freqValuesPair[freq].begin(), value);
        }

        vector<int> ans{};
        for(auto [freq, values]: freqValuesPair)
        {
            for(auto x: values)
            {
                for(int i = 0; i < freq; ++i)
                {
                    ans.insert(ans.end(), x);
                }
            }
        }

        return ans;
    }
};
```

## Approach

1. 用 HashMap 统计每个数的频率。
2. 对原数组排序，排序规则：
   - 频率低的排前面
   - 频率相同，数值大的排前面（降序）

## Implementation

```cpp
class Solution {
public:
    vector<int> frequencySort(vector<int>& nums) {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        sort(nums.begin(), nums.end(), [&](int a, int b) {
            if (freq[a] != freq[b]) return freq[a] < freq[b];
            return a > b;
        });
        return nums;
    }
};
```

## Complexity

- **Time Complexity:** O(n log n)
- **Space Complexity:** O(n)

# Notes

Excellent — this is exactly how a top-tier interview review should look. Let’s go step-by-step.

---

## 🧩 Problem Context

**LeetCode 1636 – Sort Array by Increasing Frequency**

> Sort elements by frequency (ascending). If two elements have the same frequency, sort them by value (descending).

---

## 1️⃣ Time & Space Complexity + Readability Review

### ✅ Complexity

| Step                   | Operation                                                                               | Cost |
| ---------------------- | --------------------------------------------------------------------------------------- | ---- |
| Count frequencies      | `O(n)`                                                                                  |      |
| Build freq→values map  | `O(k)` where k = #unique elements                                                       |      |
| Insert into vectors    | Each `insert(begin(), value)` is **O(len)**, so total can reach **O(k²)** in worst case |      |
| Construct final result | Each element appended `freq` times → `O(n)`                                             |      |

**Total time:** `O(n + k²)` worst-case.
**Space:** `O(n)` for the maps and output vector.

> ⚠️ Inserting at `begin()` repeatedly is costly — that’s the main inefficiency.

---

### ✅ Readability & Naming

| Original         | Suggested      | Reason                        |
| ---------------- | -------------- | ----------------------------- |
| `valueFreqPair`  | `freqByValue`  | Clearer: what key→value means |
| `freqValuesPair` | `valuesByFreq` | Natural mapping               |
| `ans`            | `result`       | Descriptive                   |
| `x`              | `num`          | Clearer for number values     |

Add a short doc comment before the function:

```cpp
// Sort numbers by increasing frequency;
// if two numbers have the same frequency, sort by value descending.
freqByValue
valuesByFreq
result
num
```

---

## 2️⃣ Loop Invariant Derivation (for explanation)

Let’s formalize one invariant per loop:

### Loop 1

```cpp
for (auto x: nums)
    valueFreqPair[x] += 1;
```

**Invariant:**

> After the i-th iteration, `valueFreqPair` contains correct frequency counts for the first _i_ elements of `nums`.

**Proof idea:**

- Initialization: empty map is trivially correct for 0 elements.
- Maintenance: each iteration increments exactly one count.
- Termination: after n iterations, all elements processed → map is correct.

---

### Loop 2

```cpp
for (auto [value, freq]: valueFreqPair)
    freqValuesPair[freq].insert(freqValuesPair[freq].begin(), value);
```

**Invariant:**

> After processing the first i distinct values, all their values are grouped in `freqValuesPair` according to their frequency.

**Why it holds:**
Each insertion places `value` into the vector corresponding to its frequency.

---

### Loop 3

```cpp
for (auto [freq, values]: freqValuesPair)
    ...
```

**Invariant:**

> Before each outer iteration, `ans` contains all numbers with frequencies less than the current `freq`, in correct sorted order.

**Why it helps:**
It ensures the output is accumulated in ascending frequency order.

---

## 3️⃣ C++ Idioms & Style

- Prefer `unordered_map` for counting → `O(1)` average access.
- Use structured binding with `const auto&` for readability.
- Avoid multiple nested `insert` calls (inefficient). Use `reserve` and `push_back`.

---

## 4️⃣ Follow-Up / Harder Variants

1. **Descending frequency** (reverse logic).
2. **Custom comparator with `std::sort`** – e.g., sort vector of pairs `(value, freq)`.
3. **Stream processing**: if the array is too large, use external sorting or bucket partitioning.
4. **Stable sort by multiple criteria** — general pattern for multi-key sorting.

---

## 5️⃣ Elevator Pitch (30–90 sec)

> “We first count how often each number appears using a hash map. Then we sort the numbers by their frequency, putting rarer numbers first. If two numbers appear the same number of times, we order them by their value in descending order. Finally, we rebuild the array according to these frequencies.
>
> The algorithm runs in roughly linear time for counting and logarithmic for sorting unique numbers, and it uses extra space proportional to the number of distinct elements. It’s efficient because we only sort the unique values once rather than the whole array repeatedly.”

---

## 6️⃣ Optimized “Production-Ready” Solution

### Key improvements

- Replace `map` with `unordered_map`
- Sort once using a custom comparator
- Reserve vector capacity to avoid reallocations

```cpp
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> frequencySort(vector<int>& nums) {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;

        sort(nums.begin(), nums.end(), [&](int a, int b) {
            if (freq[a] == freq[b])
                return a > b; // higher value first if equal freq
            return freq[a] < freq[b]; // lower freq first
        });
        return nums;
    }
};
```

**Time Complexity:**

- Counting: `O(n)`
- Sorting: `O(n log n)`
  **Space:** `O(k)`

This is the canonical LeetCode-style optimal approach — clear, concise, and efficient.

---

## 7️⃣ Follow-Up Practice Recommendations

| Category                    | Problems                                                                                                     |
| --------------------------- | ------------------------------------------------------------------------------------------------------------ |
| **Hash Map + Sorting**      | LC 451 (“Sort Characters by Frequency”), LC 347 (“Top K Frequent Elements”), LC 692 (“Top K Frequent Words”) |
| **Custom Sort Comparators** | LC 179 (“Largest Number”), LC 937 (“Reorder Log Files”)                                                      |
| **Bucket Sort Variants**    | LC 75 (“Sort Colors”), LC 912 (“Sort an Array”)                                                              |

---

### 🔑 Interview Tip

When asked to _prove correctness_ — always phrase your loop invariant early.
When asked for _optimization_ — mention replacing `map` with `unordered_map` and sorting with a single `sort` pass.
When asked for _scalability_ — mention bucket sort or heap-based top-k approaches.
