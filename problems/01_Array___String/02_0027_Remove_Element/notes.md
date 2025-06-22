# Remove Element

- **ID:** 27
- **Difficulty:** EASY
- **Topic Tags:** Array, Two Pointers
- **Link:** [LeetCode Problem](https://leetcode.com/problems/remove-element/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

```cpp
class Solution
{
public:
    int removeElement(vector<int> &nums, int val)
    {
        // edge cases
        if (nums.size() == 0)
        {
            return 0;
        }

        const int length = static_cast<int>(nums.size());
        int idx_l = 0, idx_r = length - 1;


        while (idx_l < length && idx_r >= 0 && idx_l < idx_r)
        {
            // search from the left
            if (nums[idx_l] != val)
            {
                idx_l++;
                continue;
            }

            if (nums[idx_r] == val)
            {
                idx_r--;
                continue;
            }

            swap(nums[idx_l++], nums[idx_r--]);
        }

        if (nums[idx_l] == val)
        {
            return idx_l;
        }
        else
        {
            return ++idx_l;
        }
    }
};
```

```cpp
int k = 0;
for (int x : nums)
  if (x != val) nums[k++] = x;
return k;
```

## Complexity

- **Time Complexity:**
  O(n)
- **Space Complexity:** (TODO)
  O(1). The algorithm uses a constant amount of space, as it only uses a few variables for indexing and does not require any additional data structures.

## Edge Cases

- If the input array is empty, the function should return 0.
- If all elements in the array are equal to `val`, the function should return 0
- If no elements in the array are equal to `val`, the function should return the length of the array.
- If the array contains only one element, the function should return 0 if that element is equal to `val`, or 1 if it is not.

## Alternative Approaches

```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        size_t left = 0;
        size_t right = nums.size();
        // Shrink right until it points past the last element
        while (left < right) {
            if (nums[left] == val) {
                // Overwrite current 'val' with the last unchecked element
                nums[left] = nums[--right];
            } else {
                ++left;
            }
        }
        return static_cast<int>(left);
    }
};
```

## production method (may not be met the question requirements)

```cpp
#include <vector>
#include <algorithm>

namespace mylib {
class Solution {
public:
    // Removes all occurrences of 'val' in-place and returns new logical length.
    // Complexity: O(n) time, O(1) extra space.
    int removeElement(std::vector<int>& nums, int val) {
        // std::remove shifts non-'val' elements forward
        auto newEnd = std::remove(nums.begin(), nums.end(), val);
        nums.erase(newEnd, nums.end());
        return static_cast<int>(nums.size());
    }
};
}  // namespace mylib
```

## 30–90-Second Elevator Pitch

“We run two indices from the front and back of the array. Whenever the front index lands on the unwanted value, we swap it with a good value at the back index, and move both indices inward. This way, by the time they meet, all unwanted values are shuffled to the end in O(n) time and O(1) space. It’s efficient because each element is touched at most once, and it does in-place partitioning without extra storage.
