## 1. Two Sum

```cpp
std::vector<int> Solution::twoSum(std::vector<int>& nums, int target)
{
    std::unordered_map<int, int> seen;
    for (int i=0; i < nums.size(); i++)
    {
        if (seen.find(target - nums[i]) != seen.end())
        {
            return {seen[target - nums[i]], i};
        }
        else
        {
            seen[nums[i]] = i;
        }
    }
    return {-1, -1};
}
```

## Notes

**Short Review and Suggestions:**

Your solution correctly solves the Two Sum problem in O(n) time using a hash map. To elevate it to a mid-level SDE standard, focus on these areas:

1. **Code Readability and Maintainability**:  
   - Improve variable naming:  
     Instead of `seen`, use `value_to_index` for clarity.  
   - Add explanatory comments and function documentation to help others understand your logic and assumptions.

2. **Complexity and Trade-offs**:  
   - Comment on time complexity (O(n)) and space complexity (O(n)).  
   - Briefly explain why `unordered_map` is chosen (O(1) average lookup) and consider when a sorting-based approach (O(n log n)) might be preferable.

3. **Robustness and Edge Cases**:  
   - Although LeetCode guarantees certain conditions, in a real scenario, address edge cases: empty array, no solution, duplicates, negative numbers.
   - Make sure the code clearly communicates what happens when no pair is found.

4. **Test Coverage and Examples**:  
   - Consider a variety of test cases beyond the provided example.  
   - This shows proactive thinking and ensures code stability for various inputs.

---

**Improved Code Example:**

```cpp
#include <unordered_map>
#include <vector>
#include <iostream>

/**
 * @brief Finds two indices in `nums` whose values sum up to `target`.
 * 
 * If a valid pair is found, returns {index1, index2}.
 * If no pair exists, returns {-1, -1}.
 * 
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 */
std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> value_to_index; 
    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];
        // Check if the complement is already in our map
        if (value_to_index.find(complement) != value_to_index.end()) {
            return {value_to_index[complement], i};
        }
        // Store the current number's index
        value_to_index[nums[i]] = i;
    }
    return {-1, -1}; // No solution found
}

int main() {
    // Test cases to illustrate usage

    // 1. Basic Example
    std::vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;
    auto result1 = twoSum(nums1, target1);
    std::cout << "Result1: [" << result1[0] << ", " << result1[1] << "]\n"; 
    // Expected: [0, 1] because nums[0] + nums[1] = 2 + 7 = 9

    // 2. Example with negative numbers
    std::vector<int> nums2 = {-3, 4, 3, 90};
    int target2 = 0;
    auto result2 = twoSum(nums2, target2);
    std::cout << "Result2: [" << result2[0] << ", " << result2[1] << "]\n"; 
    // Expected: [0, 2] because -3 + 3 = 0

    // 3. No solution example
    std::vector<int> nums3 = {1, 2, 3};
    int target3 = 7;
    auto result3 = twoSum(nums3, target3);
    std::cout << "Result3: [" << result3[0] << ", " << result3[1] << "]\n"; 
    // Expected: [-1, -1]

    // 4. Duplicate numbers
    std::vector<int> nums4 = {3, 3};
    int target4 = 6;
    auto result4 = twoSum(nums4, target4);
    std::cout << "Result4: [" << result4[0] << ", " << result4[1] << "]\n";
    // Expected: [0, 1]

    return 0;
}
```

---

**Detailed Notes and Examples**:

- **Why `unordered_map`?**  
  `std::unordered_map` provides average O(1) lookup and insertion. This is ideal for quickly checking if the complement of the current number exists. If `target - nums[i]` is in the map, you’ve instantly found a solution.

- **When to Avoid Hash Maps?**  
  If memory is constrained or if deterministic O(n log n) performance is preferred, consider a sorting + two-pointer approach. That uses less overhead and more predictable memory usage at the cost of slower O(n log n) performance.

```cpp
#include <vector>
#include <algorithm>
#include <utility>

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    // Create a vector of pairs (value, original_index)
    std::vector<std::pair<int,int>> arr;
    for (int i = 0; i < (int)nums.size(); ++i) {
        arr.push_back({nums[i], i});
    }
    
    // Sort by value
    std::sort(arr.begin(), arr.end(), [](auto &a, auto &b) {
        return a.first < b.first;
    });
    
    // Two-pointer approach
    int left = 0;
    int right = (int)nums.size() - 1;
    while (left < right) {
        int sum = arr[left].first + arr[right].first;
        if (sum == target) {
            return {arr[left].second, arr[right].second};
        } else if (sum < target) {
            left++;
        } else {
            right--;
        }
    }
    
    return {-1, -1};
}
```

- **Edge Cases**:
  - **Empty Array**: `nums = []`, any `target`. Result: `[-1, -1]`.
  - **Single Element**: `nums = [5]`, `target = 5`. No pair exists, return `[-1, -1]`.
  - **Duplicates**: `nums = [3, 3]`, `target = 6`. Correctly returns `[0, 1]`.
  - **Negative Values**: `nums = [-1, -2, -3, -4, -5]`, `target = -8`. Should return `[2, 4]` because `-3 + (-5) = -8`.


## Topic
- Array
- Hash Table