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

Below is a structured “cover-all-bases” review you can reuse as a study template when you solve other LeetCode questions. I start with a quick correctness check (spoiler: there’s a small bug), then walk through every point you listed.

---

## 1 · Correctness & Complexity

| Aspect          | Your Code                                                                                                                                                                                                                       | Comment / Fix                      |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------- |
| **Time**        | `O(n)`                                                                                                                                                                                                                          | Optimal – you scan the array once. |
| **Space**       | `O(1)`                                                                                                                                                                                                                          | Also optimal.                      |
| **Correctness** | **❌ Off-by-one**. When `k = 0`, the first comparison `x != nums[k]` is always `false`, so the first element is _never_ copied. For an input like `[1,1,2]` you’d return `2` but write `[1,2,…]` starting at `"ghost"` index 1. |                                    |
| **Fix**         | Start scanning from index 1 and keep a separate _write_ pointer.                                                                                                                                                                |                                    |

### Production-ready C++ 17 version

```cpp
int removeDuplicates(std::vector<int>& nums) {
    if (nums.empty()) return 0;              // guard for edge-case []

    std::size_t write = 0;                   // last unique element’s index
    for (std::size_t read = 1; read < nums.size(); ++read) {
        if (nums[read] != nums[write]) {     // found a new value
            nums[++write] = nums[read];      // advance write & copy
        }
    }
    return static_cast<int>(write) + 1;      // length of the unique prefix
}
```

_Idioms used:_ early-return, `std::size_t` for indices, pre-increment, explicit `static_cast`, self-documenting names (`read`, `write`).

---

## 2 · Loop Invariant (what to say in the room)

> **Invariant:** At the start of every iteration with indexes `(read, write)` > `•` `nums[0 … write]` contains **all** unique elements seen so far, each exactly once and in sorted order.
> `•` `write < read`, so the segment right of `write` (`nums[write+1 … read–1]`) is already processed but irrelevant.

### Why it holds

1. **Initialization:** Before the loop we set `write = 0`, so `nums[0]` – the first element – is trivially a unique, sorted prefix.
2. **Maintenance:**
   _If_ `nums[read] == nums[write]`, the set of uniques hasn’t changed ⇒ invariant unchanged.
   _Else_ we discovered a new value, we increment `write` once and copy `nums[read]` there – still unique, still sorted (array is sorted globally).
   Thus the invariant is preserved.
3. **Termination:** When `read` hits `n`, `write + 1` is exactly the number of unique elements, which is what we return.

Having this invariant in your back pocket shows you can reason formally and makes edge--case questions (“what if all numbers are equal?”) trivial to answer.

---

## 3 · Style / C++-specific notes

| Item                   | Recommendation                                                                                                                                                                                    |
| ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Const-correctness**  | Function can’t be `const` because you mutate `nums`, but local vars like `n` (if you store `nums.size()`) should be `const`.                                                                      |
| **`ssize_t` vs `int`** | Use `std::size_t` (unsigned) for indices; cast once when returning `int`. C++23 finally gives `std::ssize` if you prefer signed.                                                                  |
| **Range-for**          | Great for read-only scans; here you need both index & value, so a classic `for` is clearer.                                                                                                       |
| **Algorithm header**   | In production you could simply do: `return std::distance(nums.begin(), std::unique(nums.begin(), nums.end()));` but many interviewers want the manual two-pointer approach to show understanding. |

---

## 4 · Interview follow-ups & harder variants

| Variant                                        | Key idea                                                                                                   |
| ---------------------------------------------- | ---------------------------------------------------------------------------------------------------------- |
| **Allow at most 2 duplicates** (`LeetCode 80`) | Same two-pointer skeleton, but accept a new number if `write < 1` _or_ `nums[read] != nums[write-1]`.      |
| **Unsorted array**                             | Use an `unordered_set` in `O(n)` time & space, or sort first (`O(n log n)`), then apply current algorithm. |
| **Stream of data**                             | Keep only the last unique value; emit/ignore on the fly (useful in embedded systems).                      |
| **Remove duplicates but keep counts**          | Two pointers plus an auxiliary array of `(value, count)` tuples.                                           |
| **Concurrency**                                | Split the array into chunks, dedup inside each chunk, then merge borders (advanced systems question).      |

---

## 5 · 60-second “Elevator Pitch”

> “Because the input is already sorted, every duplicate sits right next to the original. I walk through the list once with two fingers: **read** explores every element, **write** marks the end of the clean, duplicate-free prefix.
> Whenever the value under _read_ differs from the one under _write_, I’ve discovered a new unique number – I bump _write_ forward by one slot and copy the new value there.
> At the end, everything before _write_ is clean, everything after it can be ignored, and the number of unique items is simply `write + 1`.
> I only touch each element once (`O(n)` time) and use two integers of extra storage (`O(1)` space), so it’s as fast and memory-friendly as possible.”

Non-technical version: _“Imagine crossing out repeated names in a sorted guest list by moving them to the back and keeping only the first appearance – I do that in one pass with two pens.”_

---

## 6 · Alternative perspectives (optional optimisations)

- **`std::unique`** (as shown above) – battle-tested, expressive, but less “show-your-work” in interviews.
- **SIMD / vectorisation** – rarely worth it for interview-scale arrays but good in performance-critical production code.
- **Compile-time check** – add `static_assert(std::is_sorted_v<It>, …)` in template utilities to catch misuse.

---

## 7 · “Production-grade” version with documentation

```cpp
/**
 * Removes in-place all *consecutive* duplicates from a **sorted** vector.
 * @param nums Sorted vector of integers (by value).
 * @return     Count of unique elements; the first `count` slots of `nums`
 *             hold those elements in their original order.
 * @complexity Time O(n), Space O(1).
 */
int removeDuplicates(std::vector<int>& nums)
{
    if (nums.empty()) return 0;

    std::size_t write = 0;
    for (std::size_t read = 1; read < nums.size(); ++read)
        if (nums[read] != nums[write])
            nums[++write] = nums[read];

    return static_cast<int>(write) + 1;
}
```

_Unit tests_ would include: empty array, single element, all duplicates, all unique, and alternating duplicates.

---

### What to practise next 🔥

1. **LeetCode 80 – “Remove Duplicates from Sorted Array II”** (allow two copies).
2. **LeetCode 283 – “Move Zeroes”** (same pointer pattern, different predicate).
3. **Implement `vector::eraseDuplicates` as a templated utility** (practice generic programming & `std::unique`).
4. **Explain the loop invariant in writing and out loud**; aim for ≤ 90 seconds.
5. **Rewrite the algorithm in Python and Rust** to cement cross-language fluency.

Good luck – keep polishing both the code _and_ the story you tell about it!
