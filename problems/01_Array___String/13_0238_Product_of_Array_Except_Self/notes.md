# Product of Array Except Self

- **ID:** 238
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Prefix Sum
- **Link:** [LeetCode Problem](https://leetcode.com/problems/product-of-array-except-self/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

```cpp
/*
[1,2,3,4]


prefix = 1

*/

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(n, 1);

        int prefix = 1;
        for(int i = 0; i < n; ++i)
        {
            ans[i] *= prefix;
            prefix *= nums[i];
        }

        int suffix = 1;
        for(int i = n - 1; i >= 0; --i)
        {
            ans[i] *= suffix;
            suffix *= nums[i];
        }

        return ans;
    }
};
```

## Complexity

- **Time Complexity:** O(n) - Two passes through the array
- **Space Complexity:** O(1) extra space (not counting output array, which is required)

# Product of Array Except Self - Solution Review

## 1. Time/Space Complexity & Code Quality Analysis

### Your Current Solution

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(n, 1);

        int prefix = 1;
        for(int i = 0; i < n; ++i) {
            ans[i] *= prefix;
            prefix *= nums[i];
        }

        int suffix = 1;
        for(int i = n - 1; i >= 0; --i) {
            ans[i] *= suffix;
            suffix *= nums[i];
        }

        return ans;
    }
};
```

### Complexity Analysis

- **Time Complexity**: O(n) - Two passes through the array
- **Space Complexity**: O(1) extra space (not counting output array, which is required)

### Code Quality Assessment

**Strengths:**

- Clean, readable implementation
- Optimal time and space complexity
- Good variable naming (`prefix`, `suffix`)

**Areas for Improvement:**

- Missing documentation/comments
- Could use more descriptive variable names
- No input validation

## 2. Loop Invariant Analysis

### Invariant Definition

**First Loop (Prefix Products):**

- **Invariant**: At the start of iteration `i`, `ans[j]` contains the product of all elements `nums[0] * nums[1] * ... * nums[j-1]` for all `j < i`
- **Variable**: `prefix` holds the product of all elements from `nums[0]` to `nums[i-1]`

**Second Loop (Suffix Products):**

- **Invariant**: At the start of iteration `i`, `ans[j]` contains the final result (prefix product × suffix product) for all `j > i`
- **Variable**: `suffix` holds the product of all elements from `nums[i+1]` to `nums[n-1]`

### Proof of Correctness

1. **Initialization**: Both loops start with correct initial values (`prefix = 1`, `suffix = 1`)
2. **Maintenance**: Each iteration correctly updates the invariant
3. **Termination**: After both loops, `ans[i]` contains the product of all elements except `nums[i]`

## 3. C++ Style & Idioms

### Current Style Issues

```cpp
// Your current style is mostly good, but consider:
for(int i = 0; i < n; ++i)  // Good: pre-increment
vector<int> ans(n, 1);      // Good: initialization
```

### Suggested Improvements

```cpp
// Use size_t for array indices to avoid signed/unsigned warnings
for (size_t i = 0; i < nums.size(); ++i)

// Consider const-correctness where possible
// Add space after control structures for readability
```

## 4. Common Interview Follow-ups

### Follow-up Questions

1. **"What if we can use division?"**

   - Calculate total product, divide by each element
   - Handle zeros specially (count them)

2. **"What if the array contains zeros?"**

   - Your solution handles this correctly!
   - One zero: only that position has non-zero result
   - Multiple zeros: all results are zero

3. **"Can you do it with only one pass?"**

   - Not possible while maintaining O(1) extra space

4. **"What about integer overflow?"**

   - Use `long long` or check for overflow
   - Consider modular arithmetic if specified

5. **"Implement using recursion?"**
   - Possible but less efficient due to call stack

### Harder Variants

- **2D version**: Product of matrix except current row/column
- **Range queries**: Product of subarray except element at index
- **Streaming version**: Handle dynamic array updates

## 5. Elevator Pitch (30-90 seconds)

_"Imagine you're a cashier calculating discounts. You need to know the total value of all items except one specific item for each customer. Instead of recalculating everything each time, I use a smart two-pass approach:_

_First, I walk left-to-right, collecting the 'running total' of everything I've seen so far - like keeping a running subtotal. Then I walk right-to-left, doing the same thing backwards. When I combine these two totals for each position, I get exactly what I need: the product of everything except that one item._

_This runs in linear time - I only look at each number twice - and uses minimal extra memory. It's elegant because it avoids division entirely, so it naturally handles edge cases like zeros."_

## 6. Production-Ready Solution

```cpp
#include <vector>
#include <stdexcept>

class Solution {
public:
    /**
     * Computes the product of all elements in the array except the element at each index.
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1) extra space (excluding output array)
     *
     * @param nums Input array of integers
     * @return Vector where result[i] = product of all nums[j] where j != i
     * @throws std::invalid_argument if input array is empty
     */
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        if (nums.empty()) {
            throw std::invalid_argument("Input array cannot be empty");
        }

        const size_t n = nums.size();
        std::vector<int> result(n, 1);

        // First pass: compute prefix products
        // result[i] = product of nums[0] through nums[i-1]
        int left_product = 1;
        for (size_t i = 0; i < n; ++i) {
            result[i] = left_product;
            left_product *= nums[i];
        }

        // Second pass: multiply by suffix products
        // result[i] *= product of nums[i+1] through nums[n-1]
        int right_product = 1;
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            result[i] *= right_product;
            right_product *= nums[i];
        }

        return result;
    }
};

// Alternative with overflow protection
class SafeSolution {
public:
    std::vector<long long> productExceptSelfSafe(std::vector<int>& nums) {
        if (nums.empty()) {
            throw std::invalid_argument("Input array cannot be empty");
        }

        const size_t n = nums.size();
        std::vector<long long> result(n, 1LL);

        long long left_product = 1LL;
        for (size_t i = 0; i < n; ++i) {
            result[i] = left_product;
            // Check for potential overflow
            if (left_product > LLONG_MAX / nums[i] && nums[i] != 0) {
                throw std::overflow_error("Integer overflow detected");
            }
            left_product *= nums[i];
        }

        long long right_product = 1LL;
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            if (right_product > LLONG_MAX / nums[i] && nums[i] != 0) {
                throw std::overflow_error("Integer overflow detected");
            }
            result[i] *= right_product;
            right_product *= nums[i];
        }

        return result;
    }
};
```

## 7. Additional Optimizations & Perspectives

### Memory Access Optimization

Your solution is already optimal for memory access patterns (sequential access).

### Alternative Approaches

1. **Divide and Conquer**: O(n log n) but more complex
2. **Using Division**: O(n) but fails with zeros
3. **Recursive**: Same logic but with function call overhead

### Key Insights

- The two-pass approach is actually a form of **dynamic programming**
- It's related to **prefix sums** but with multiplication
- The technique generalizes to other associative operations

## Summary

Your solution is excellent! It demonstrates strong algorithmic thinking and achieves optimal complexity. The main improvements for interview success would be:

1. Add clear comments explaining the invariant
2. Include input validation
3. Practice the elevator pitch
4. Be ready to discuss the zero-handling naturally built into your approach

This is a solution that would impress in most technical interviews.
