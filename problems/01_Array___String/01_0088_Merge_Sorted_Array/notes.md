# Merge Sorted Array

- **ID:** 88
- **Difficulty:** EASY
- **Topic Tags:** Array, Two Pointers, Sorting
- **Link:** [LeetCode Problem](https://leetcode.com/problems/merge-sorted-array/description/)

## Problem Description

You are given two integer arrays nums1 and nums2, sorted in non-decreasing order, and two integers m and n, representing the number of elements in nums1 and nums2 respectively.

Merge nums1 and nums2 into a single array sorted in non-decreasing order.

The final sorted array should not be returned by the function, but instead be stored inside the array nums1. To accommodate this, nums1 has a length of m + n, where the first m elements denote the elements that should be merged, and the last n elements are set to 0 and should be ignored. nums2 has a length of n.

## Approach

(TODO: Outline your approach here)

## Complexity

- **Time Complexity:** (TODO)
- **Space Complexity:** (TODO)

# Review Notes

### 1 · Quick “elevator‑pitch” you can give the interviewer (≈ 20 s)

> “I merge **in‑place, from the back**.
> Three indices: `i = m‑1`, `j = n‑1`, `k = m+n‑1`.
> While `j ≥ 0`, write the larger of `nums1[i]` and `nums2[j]` to `nums1[k]`, moving the corresponding pointer left.
> Complexity O(m + n) time, **O(1)** extra space, covers all edge cases (`m = 0`, `n = 0`).”

If you can recite that cleanly, half the battle is already won.

---

### 2 · Algorithmic review of **your** code

| Aspect                      | What you did                                                                                                                               | Suggested tightening                                                                                                                                                                                                                                                                                                                                           |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Correctness & invariant** | Maintains:_“indices to the right of `i`,`j` are already placed in non‑decreasing order and are the largest `k‑(i+j)` elements”_ – correct. | Make that invariant explicit when you speak; it convinces the interviewer you’re reasoning formally.                                                                                                                                                                                                                                                           |
| **Control flow**            | Nested `if…else if…else if` inside a `for` loop.                                                                                           | A single `while (j >= 0)` loop is simpler and drops two branches:<br>`while (j >= 0) nums1[k--] = (i >= 0 && nums1[i] > nums2[j]) ? nums1[i--] : nums2[j--];`                                                                                                                                                                                                  |
| **Edge cases**              | You explicitly guarded both exhaustion scenarios – good.                                                                                   | In interviews, call out `m = 0`, `n = 0`, duplicates, already merged, all identical numbers.                                                                                                                                                                                                                                                                   |
| **Complexity**              | O(m + n) / O(1) – the bar for this question.                                                                                               | Mention that forward copying would cause O(m × n) shifts; your reverse pass avoids it.                                                                                                                                                                                                                                                                         |
| **Safety**                  | Indices are `int`; fine for LeetCode but **mention** that `size_t` would be safer in production on 64‑bit data.                            | Also note the potential under‑flow when `m=0` (`i=m‑1` → ‑1). You already guard with `i >= 0`.                                                                                                                                                                                                                                                                 |
| **C++ specifics**           | Pass‑by‑reference, good.                                                                                                                   | _Const‑correctness_: the inputs are logically mutating only `nums1`; you can mark `nums2` as `const vector<int>&`. <br>_Qualifier avoidance_: remove the comment `// TODO: Define your methods here` – looks unfinished in a real interview. <br>_Namespace_: in short snippets `using std::vector;` or `using namespace std;` is acceptable if you ask first. |

---

#### A polished interview‑friendly implementation

```cpp
class Solution {
public:
    void merge(vector<int>& nums1, int m,
               const vector<int>& nums2, int n) {

        int i = m - 1;          // last valid element in nums1
        int j = n - 1;          // last element in nums2
        int k = m + n - 1;      // write position

        while (j >= 0) {        // only need to copy if nums2 remains
            nums1[k--] = (i >= 0 && nums1[i] > nums2[j])
                        ? nums1[i--]
                        : nums2[j--];
        }
    }
};
```

Why it’s crisper:

- No redundant checks once `nums2` is empty – you’re done.
- `const` protects against accidental writes to `nums2`.
- Explicit indices convey intent without comments.

---

### 3 · How to **talk through** this solution

1. **Pattern identification**
   “We have two sorted sources → classic _two‑pointer_ merge problem.”

2. **Naïve idea & its cost**
   “If I copy all of `nums1` to a temp buffer and merge forward, time O(m+n) but space O(m). Goal is O(1) extra.”

3. **Key observation**
   “The free space is at the **end** of `nums1`. If we write from back to front we never overwrite unread data.”

4. **Invariant statement**

   > After each iteration, `nums1[k+1…]` contains the largest `m+n-(k+1)` elements in sorted order.

5. **Complexity**
   O(m+n) time, O(1) space, stable with respect to `nums2`.

6. **Edge cases**
   Enumerate them clearly: empty arrays, single element, duplicates, all elements equal.

---

### 4 · C++‑centric discussion points you can raise if time allows

| Topic                             | One‑liner you can volunteer                                                                                                                                                                                                                |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **STL alternatives**              | “`std::inplace_merge` requires contiguous halves within a single array; our halves live in _separate_ arrays, so we can’t use it directly. `std::merge` copies into a third range, so space O(m+n). That’s why I implemented it manually.” |
| **Move semantics**                | “If these were `vector<T>` with expensive copies, we’d still be fine because we only _move‑assign_ elements — trivial for `int`, but important for object types.”                                                                          |
| **Iterator stability**            | “Because we overwrite from the back, we never invalidate the `i` and `j` indices we haven’t used yet.”                                                                                                                                     |
| **Template generalisation**       | “We can turn the function into `template<class RandomIt1, class RandomIt2>` and use `std::iter_value_t` to accept any random‑access range, enabling reuse across projects.”                                                                |
| **constexpr / concepts (C++20+)** | “With concepts we could constrain the template to `std::totally_ordered` types for stronger compile‑time guarantees.”                                                                                                                      |

Showing awareness of these lifts you above the typical LeetCode‑only preparation.

---

### 5 · Self‑checks before declaring “done” in the interview

1. **Ran through the smallest failing case aloud** (_e.g._ `m=0, n=1` → `[0]`, `[1]`).
2. **Confirmed no signed/unsigned mix‑ups** (`int` vs `size_t`).
3. **Named an alternative** (copy to buffer or forward merge) and why it’s worse.
4. **Stated complexities** and space trade‑off.
5. **Added 1‑2 lines of explanatory comments if code is very dense.**

---

### 6 · Follow‑up questions the interviewer may throw

| Question                                                    | Why they ask                                                | How to answer in one breath                                                                |
| ----------------------------------------------------------- | ----------------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| “Can you do it if both arrays are singly linked lists?”     | Tests pointer reasoning.                                    | Use dummy head, forward merge, space O(1), can’t traverse backward, so no back‑fill trick. |
| “What if `nums1` isn’t large enough?”                       | Tests adaptability.                                         | Allocate a third array or resize `nums1` first (costs O(m+n) extra mem).                   |
| “Now find the median of two sorted arrays.”                 | Probes depth.                                               | Pivot to O(log (min(m,n))) binary‑search partition technique.                              |
| “What changes if elements are objects with expensive copy?” | Sees if you consider move semantics / swap to avoid copies. | Mention move‑assignment or merging indices only, then re‑ordering pointers.                |

---

## TL;DR

Your solution is **already correct and interview‑ready**; trim the control flow, add `const`, and practise the 20‑second overview + invariant explanation. Pair that with the STL/complexity talking points and you’ll present as a polished, confident C++ candidate. Good luck — see you on the whiteboard!
