# Remove Duplicates from Sorted Array II

- **ID:** 80
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Two Pointers
- **Link:** [LeetCode Problem](https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

```cpp
class Solution {
public:
	// Use two pointers
	// write: tracks position to write next valide element
	// read: scan the input array, allow each element at most twice
    int removeDuplicates(vector<int>& nums) {
        size_t write = 0;
        size_t count = 1;
        for (auto read = 0; read < nums.size(); read++)
        {
            if (write == read)
            {
                continue;
            }
		    
		    // first duplicate
            if (nums[write] == nums[read] && count == 1)
            {
                count++;
                nums[++write] = nums[read];
            }
            // new element
            else if (nums[write] != nums[read])
            {
                nums[++write] = nums[read];
                count = 1;
            }
        }

        return static_cast<int>(write) + 1;
    }
};
```

## Complexity

- **Time Complexity:** (O(n))
- **Space Complexity:** (O(1))

**1. Time & Space Complexity, Readability, Naming, Documentation**

- **Time Complexity**: O(n), where n is nums.size(). We make a single pass with two indices (read, write), doing only constant-time work per element.
    
- **Space Complexity**: O(1) extra space, as required (modifies nums in place).
    
- **Readability & Naming**
    
    - read / write are clear two-pointer names.
        
    - count is slightly ambiguous—better: dupeCount or currentCount.
        
    - Using size_t everywhere is fine, but mixing size_t and signed int on return can be confusing. Consider returning write as size_t or casting explicitly in one place.
        
    
- **Documentation**
    
    - A brief comment outlining the overall two-pointer approach at function top would help, e.g.:
        
    

```
// Use two pointers: `write` tracks position to write next valid element;
// `read` scans input. Allow each value at most twice.
```

- -   
        
    - Inline comments on the two if-branches would clarify purpose (first duplicate vs new value).
        
    

---

**2. Loop Invariant**

  

> **Invariant:** At the start of each iteration (for a given read),

1. > All elements in nums[0..write] form the correct “deduped” prefix for nums[0..read-1], with each value appearing at most twice.
    
2. > write < read (except initially, where both are 0).
    

  

- **Initialization**: Before the first iteration (read = 0), we haven’t written anything yet (write = 0), but we treat nums[0] as the first valid element (prefix of length 1 with no duplicates).
    
- **Maintenance**:
    
    - If nums[read] == nums[write] and dupeCount == 1, we accept the second copy by incrementing write and dupeCount. That keeps the prefix correct.
        
    - If it’s a new value, we reset dupeCount = 1, increment write, and copy. That extends the prefix correctly.
        
    - Otherwise (third+ copy), we skip without modifying write, leaving the prefix unchanged.
        
    
- **Termination**: After read = nums.size(), the invariant guarantees nums[0..write] is the final array with each value ≤2 times. Returning write+1 is the correct length.
    

---

**3. C++ Style & Idioms**

- **Use of auto** for loop indices (especially in modern C++17+):
    

```
for (auto read = size_t{0}; read < nums.size(); ++read) { … }
```

-   
    
- **Prefer prefix increment** (++read, ++write).
    
- **Structured binding** is irrelevant here, but consider const auto& val = nums[read]; to avoid repeated indexing.
    
- **Early continue** on read == write is okay, but you could initialize write = 1 and start read = 1, setting dupeCount = 1 to simplify.
    

---

**4. Common Follow-Ups & Harder Variants**

- **Generalize “at most k duplicates”**: make k a parameter:
    

```
int removeDuplicates(vector<int>& A, int k) { … }
```

-   
    
- **Unsorted input**: must use hash + sliding window or sort first (trade-offs).
    
- **Return new container** vs in-place.
    
- **Linked list variant**: similar two-pointer logic but with node pointers.
    
- **Streaming/infinite** input: keep a small buffer or count with a map.
    

---

**5. 30–90s Elevator Pitch**

  

> “We use a **two-pointer** method on the sorted array: one pointer (read) scans every element, and the other (write) builds the ‘clean’ prefix in place. We also track how many times the current value has appeared (dupeCount). When we see a new value, or the **second** occurrence, we copy it to write+1 and advance write. We skip any further duplicates. This runs in linear time and constant extra space, since we only walk the array once and overwrite it in place. It’s efficient because sorted order groups duplicates together, letting us enforce the “at most twice” rule with simple counters.”

---

**6. Additional Optimizations & Perspectives**

- **Simplified two-pointer** without explicit count variable: compare nums[read] to nums[write - k] (for k=2):
    

```cpp
int removeDuplicates(vector<int>& A) {
    int write = 0;
    for (int x : A) {
        if (write < 2 || x != A[write - 2])
            A[write++] = x;
    }
    return write;
}
```

- This handles the “≤2” rule by looking two positions back.
    
- **Template** for any k: replace 2 with k.
    
- **Branchless** style (more advanced): use std::copy_if with a custom predicate, though it may allocate or be less clear.
    

---

**7. Production-Ready Solution**

```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        const int k = 2;
        int write = 0;
        for (int read = 0, n = nums.size(); read < n; ++read) {
            // Allow if we have written fewer than k elements,
            // or current != element k positions before.
            if (write < k || nums[read] != nums[write - k]) {
                nums[write++] = nums[read];
            }
        }
        return write;
    }
};
```

- **Why it’s production-grade**:
    
    - **Concise** and **easy to verify**.
        
    - **Extensible**: change k at one place.
        
    - **Zero extra memory**, single pass, no separate counters.
        
    - **Handles edge cases** (small arrays) naturally.
        
    

---

**Follow-up Practice**

1. **Generalize** to allow at most _k_ duplicates; implement and test with k=1, k=3.
    
2. **Unsorted version**: discuss trade-offs if nums isn’t sorted (e.g. use an unordered_map).
    
3. **Linked-list variant**: write an in-place removal for a ListNode*.
    
4. **Proof exercise**: formally state and prove the loop invariant for your simplified solution.
    
5. **Edge cases**: test empty array, all identical elements, strictly increasing array, array of size 1 and 2.
    

  

These exercises will deepen your understanding and prepare you for related interview questions. Good luck!