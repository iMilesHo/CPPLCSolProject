# Rotate Array

- **ID:** 189
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Math, Two Pointers
- **Link:** [LeetCode Problem](https://leetcode.com/problems/rotate-array/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

My Approach original solution:

```cpp
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        std::vector<int> helpNums = nums;
        std::size_t length = nums.size();
        k = k % static_cast<int>(length);

        // rotate
        for (int i = length-1; i >= k; i--)
        {
            nums[i] = nums[i-k];
        }
        // fill
        for(int i = 0; i < k; i++)
        {
            nums[i] = helpNums[length-k+i];
        }
    }
};
```

My optimized solution:

```cpp
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int length = static_cast<int>(nums.size());
        k = k % length;
        // leave edge case a side
        if (k == 0)
        {
            return;
        }

        int left = 0;
        int right = length-1;
        while(left < right)
        {
            std::swap(nums[left], nums[right]);
            left++;
            right--;
        }

        left = 0;
        right = k-1;
        while(left < right)
        {
            std::swap(nums[left], nums[right]);
            left++;
            right--;
        }

        for(int x: nums)
        {
            std::cout << x << ", ";
        }
        std::cout << std::endl;

        // reverse
        left = k;
        right = length - 1;
        while(left < right)
        {
            std::swap(nums[left], nums[right]);
            left++;
            right--;
        }

        for(int x: nums)
        {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
};
```

## Complexity

- **Time Complexity:** O(n)
- **Space Complexity:** O(1)

# Notes

Awesome problem choice — and your solutions are on the right track. Here’s a tight, interview-ready review and upgrades.

# 1) Complexity, readability, naming, docs

**Your “copy then fill” version**

- Time: **O(n)**
- Space: **O(n)** (extra array)
- Readability: clear intent; good normalization `k %= n`.
- Nit: prefer consistent signed types (avoid mixing `std::size_t` and `int`), and avoid reverse-index loop off-by-one risks.

**Your “three-reversals” version**

- Time: **O(n)**
- Space: **O(1)** ✅
- Readability: solid; but:

  - Remove `std::cout` (debug only).
  - Factor the “reverse two-pointer” into a helper or use `std::reverse`.
  - Early returns for `n == 0` and `k == 0`.
  - Use descriptive names: `n`, `r` (or `rotate_by`), `lo`, `hi`.
  - Keep indices signed (`int`) only if you’re sure `n` fits; otherwise prefer `std::ptrdiff_t` or just rely on STL algorithms.

# 2) Loop invariant (for the reverse-method) — what to say in an interview

We rotate right by `k`. Reverse approach does:

1. Reverse the whole array.
2. Reverse the first `k` elements.
3. Reverse the remaining `n−k` elements.

**Two-pointer reverse routine invariant**
For any call `reverse(a, L, R)`, with pointers `i` from `L` up and `j` from `R` down:

- **Invariant:** after each swap and pointer move, the segment `[L, i)` is the mirror of `(j, R]`, and the segment `(i, j)` is the portion still to be reversed.
- **Initialization:** before the first step, both mirrored segments are empty — invariant holds vacuously.
- **Maintenance:** swapping `a[i]` and `a[j]` preserves “mirror” structure; advancing `i++`, `j--` grows the mirrored, fixed prefix/suffix and shrinks the working middle.
- **Termination:** loop stops when `i >= j`. All elements are in mirrored positions ⇒ segment `[L, R]` is reversed.

**Why three reversals implement rotation**

- Let original be `X = [A | B]` where `B` are the last `k` items. Desired result is `[B | A]`.
- Step 1 reverses `X` → `rev(X) = rev(B) | rev(A)`.
- Step 2 reverses the first `k` elements → `rev(rev(B)) | rev(A) = B | rev(A)`.
- Step 3 reverses the remaining `n−k` → `B | rev(rev(A)) = B | A`, which is the rotated array.

That’s the clean, quick proof interviewers love.

# 3) C++ style & idioms

- Prefer `<algorithm>`: `std::reverse` (and even `std::rotate`, see below).
- Normalize safely: `if (n == 0) return; k %= n; if (k == 0) return;`
- Eliminate debug I/O in library/production code.
- Keep function `noexcept` if you only do swaps and index math.
- If you want genericity, template on iterator / range rather than `std::vector<int>&`.

# 4) Follow-ups & harder variants

- **Use STL:** Right-rotate by `k` ≡ left-rotate by `n−k`:
  `std::rotate(begin, end - (k % n), end);` (O(n), O(1)).
- **Cycle (juggling) method:** O(n) time, O(1) space using `gcd(n,k)` cycles — good to know and derive.
- **Left rotation** (same techniques).
- **Rotate subrange** only (range `[l, r)`).
- **Rotate linked list** (LeetCode 61): two-pointer & ring trick.
- **2D variations:** rotate matrix by k positions row-wise/column-wise or 90° rotations.
- **Streaming / huge arrays:** when random access is expensive, use deque or circular buffer view.
- **Stability question:** rotations are not “stable” by default—worth noting if asked.

# 5) 30–90s elevator pitch (plain-English)

“Rotating an array right by _k_ means taking the last _k_ items and putting them in front. A neat in-place trick does this with three reversals: reverse the whole array, then reverse the first _k_ items, then reverse the rest. Reversal is easy: swap from both ends moving inward. This runs in linear time — we touch each element a constant number of times — and uses constant extra memory. It’s simple, fast, and doesn’t allocate extra space, which is great for production code.”

# 6) Alternative perspectives / optimizations

- **One-liner with STL:** `std::rotate` is concise and correct; favors readability and battle-tested code.
- **GCD cycles:** Avoid three passes; still O(n), O(1), but demonstrates deeper understanding of structure (positions move by `k mod n`).

# 7) Production-grade solutions

### A) Idiomatic STL solution (concise, safe)

```cpp
#include <algorithm>
#include <vector>

class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        const auto n = nums.size();
        if (n == 0) return;
        k %= static_cast<int>(n);
        if (k == 0) return;
        // right-rotate by k == left-rotate by n-k
        std::rotate(nums.begin(), nums.end() - k, nums.end());
    }
};
```

### B) Three-reversal solution using STL

```cpp
#include <algorithm>
#include <vector>

class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        const auto n = nums.size();
        if (n == 0) return;
        k %= static_cast<int>(n);
        if (k == 0) return;

        std::reverse(nums.begin(), nums.end());
        std::reverse(nums.begin(), nums.begin() + k);
        std::reverse(nums.begin() + k, nums.end());
    }
};
```

### C) GCD cycle (juggling) — great for “how else?” follow-ups

```cpp
#include <vector>
#include <numeric> // std::gcd

class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        if (n == 0) return;
        k %= n;
        if (k == 0) return;

        const int cycles = std::gcd(n, k);
        for (int start = 0; start < cycles; ++start) {
            int i = start;
            int carried = nums[i];
            while (true) {
                int j = (i + k) % n;
                if (j == start) { nums[i] = carried; break; }
                nums[i] = nums[j];
                i = j;
            }
        }
    }
};
```

---

## Notes specific to your code

- Replace the custom reverse loops with `std::reverse` for clarity and fewer edge-case risks.
- Remove the two debug print loops — they’re useful locally but noise for interviews/production.
- Add fast exits for `n == 0` and `k == 0`.
- Prefer consistent types (avoid mixing `size_t` and `int`; the STL choices above avoid index arithmetic altogether).

---

## Practice prompts (to rehearse)

1. **Invariant explanation:**
   “State the invariant of your two-pointer reverse and use it to prove correctness.”

2. **STL substitution:**
   “How would you implement the same rotation using the STL in one line?”

3. **Alternative algorithm:**
   “Describe the cycle/juggling method and why the number of cycles is `gcd(n,k)`.”

4. **Edge cases:**
   “What happens when `k > n`, `k == 0`, `n == 0`, or all elements equal?”

5. **Variants:**
   “How to rotate a singly-linked list by k?” (ring, find new tail, break.)

If you want, I can generate a small set of randomized tests (property: rotation preserves multiset; rotating by `n` returns original; rotating by `k` then by `n−k` returns original) to paste into your local harness.
