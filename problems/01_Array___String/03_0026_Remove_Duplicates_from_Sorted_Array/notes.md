# Remove Duplicates from Sorted Array

- **ID:** 26
- **Difficulty:** EASY
- **Topic Tags:** Array, Two Pointers
- **Link:** [LeetCode Problem](https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/)

## Problem Description

```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int k = 0;
        for (int x: nums)
        {
            if (x != nums[k])
            {
                nums[++k] = x;
            }
        }
        return ++k;
    }
};
```

## Approach

(TODO: Outline your approach here)

## Complexity

- **Time Complexity:** (TODO)
- **Space Complexity:** (TODO)
