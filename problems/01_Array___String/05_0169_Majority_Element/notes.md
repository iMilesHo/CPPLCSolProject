# Majority Element

- **ID:** 169
- **Difficulty:** EASY
- **Topic Tags:** Array, Hash Table, Divide and Conquer, Sorting, Counting
- **Link:** [LeetCode Problem](https://leetcode.com/problems/majority-element/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

My solution:

```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        // map
        int halfLength = nums.size() / 2;
        map<int, int> elementCount{};
        for(auto i: nums)
        {
            elementCount[i] += 1;
        } //
        for(auto pair: elementCount)
        {
            if(pair.second > halfLength)
            {
                return pair.first;
            }
        }
        return 0;
    }
};
```

Great problem to practice! Here’s a structured, interview-ready review of your current solution plus a polished alternative, proofs you can speak to, and follow-ups.

---

# 1) Quick review of your solution

### What you wrote (map counting)

- Uses `std::map<int,int>` to count frequencies, then scans for `> n/2`.

### Complexity

- Time: `O(n log u)` where `u` is #distinct values (each map op is `log u`).
- Space: `O(u)` for the counts.

### Readability & naming

- `halfLength` → `threshold` is clearer.
- `elementCount` → `freq` is the standard term.
- Use `const` where possible; avoid copying pairs in the second loop.

### C++ idioms

- Prefer `std::unordered_map` over `std::map` for average `O(1)` updates.
- Early-exit while counting once `freq[x] > threshold` (saves a pass).
- Range-for with `const int x : nums` and `const auto& kv : freq`.

### Minimal edit of your approach (faster & cleaner)

```cpp
class Solution {
public:
    int majorityElement(const std::vector<int>& nums) {
        const int threshold = static_cast<int>(nums.size() / 2);
        std::unordered_map<int,int> freq;
        freq.reserve(threshold + 1);                // tiny optimization
        for (const int x : nums) {
            if (++freq[x] > threshold) return x;    // early exit
        }
        return nums[0]; // LeetCode 169 guarantees existence
    }
};
```

---

# 2) Loop invariant(s) & proof (what to say in an interview)

## A. For the counting approach

**Invariant:** After processing the first `k` elements, for every value `v`, `freq[v]` equals the number of occurrences of `v` in `nums[0..k-1]`.

**Why it holds:** Base case `k=0`: all 0. Inductive step: when you see `nums[k] = t`, you increment `freq[t]` by 1 and leave others unchanged; therefore the counts remain exact.

**How it helps:** At the end (`k=n`), a value with `freq[v] > n/2` must be the majority element, and if the problem guarantees such an element exists, returning it is correct.

## B. For Boyer–Moore Majority Vote (the `O(1)` space star)

**Algorithm (one pass):** Maintain `candidate` and `count`. For each `x`:

- If `count == 0`, set `candidate = x`.
- If `x == candidate` increment `count`, else decrement `count`.

**Key invariant (pair-cancellation view):** At any point, the processed prefix can be partitioned into:

- Some number of _canceled pairs_ of different values, plus
- The _uncanceled remainder_, which are all equal to the current `candidate`, counted by `count`.

**Why it holds:** Every time we see a non-candidate and `count > 0`, we “pair off” one candidate with that different element (netting `count--`). When `count==0`, there’s no unmatched candidate left; the next element starts a new remainder.

**Why it returns the majority:** A true majority element `M` appears more than all other values combined. Pair-cancellation can never eliminate all copies of `M`; thus after all cancellations, the leftover (if any) must be `M`, making it the final `candidate`.

---

# 3) Code style & C++ specifics

- Prefer `const` correctness: take `const std::vector<int>&` when you don’t modify the vector.
- Use fixed types (`int` is fine for LeetCode; consider `std::int32_t` in strict codebases).
- Avoid magic values in returns: LeetCode guarantees existence, but in production, verify first (see below).
- Keep functions small and name variables semantically: `candidate`, `count`, `threshold`, `freq`.

---

# 4) Common follow-ups & harder variants

1. **Can you do `O(1)` extra space?**
   → Boyer–Moore. Time `O(n)`, space `O(1)`.

2. **What if a majority element may _not_ exist?**
   → Run Boyer–Moore to get a candidate, then **verify** by a second pass counting it; if `count > n/2`, return it; otherwise signal “none”.

3. **More than one majority?**
   → Impossible for `> n/2`.

4. **Majority Element II (LeetCode 229, `> n/3`)**
   → Generalized Boyer–Moore tracking two candidates and two counts; then verify.

5. **Streaming data / large data**
   → Boyer–Moore is online: you can feed items incrementally; keep a rolling candidate.

6. **Randomized approach**
   → Sample random indices and verify; high probability in `O(n)` expected time.

7. **Range majority queries**
   → Much harder: use segment trees with Boyer–Moore summaries per node + verification.

---

# 5) Elevator pitch (≈60 seconds)

“We need the number that appears more than half the time. The neat trick is **Boyer–Moore’s majority vote**: scan once, keep just a `candidate` and a `count`. When you see the candidate, increase the count; when you see something else, decrease it. Think of it as canceling out pairs of different values. Because the real majority occurs more than everything else combined, it can’t be fully canceled—it survives as the final candidate. This runs in linear time with constant memory. If the input might not have a true majority, we just do a quick second pass to verify the candidate’s frequency.”

---

# 6) Alternative perspectives / optimizations (quick)

- **Sorting:** `O(n log n)`, middle element works; simple but not optimal.
- **Bit counting:** For each of 32 bits, count ones; reconstruct the majority. `O(32n)` time, `O(1)` space; nice when values are wide but still integer.
- **Divide & conquer:** Recurse left/right to find local majorities and combine. `O(n log n)`.

---

# 7) Production-grade solutions (robust + clear)

### A) Boyer–Moore with verification (safe for real systems)

```cpp
#include <vector>
#include <optional>
#include <algorithm>

std::optional<int> majority_element_verified(const std::vector<int>& nums) {
    if (nums.empty()) return std::nullopt;

    int candidate = 0, count = 0;
    for (const int x : nums) {
        if (count == 0) candidate = x;
        count += (x == candidate) ? 1 : -1;
    }

    // Verify the candidate
    const int freq = static_cast<int>(std::count(nums.begin(), nums.end(), candidate));
    if (freq > static_cast<int>(nums.size() / 2)) return candidate;
    return std::nullopt;
}
```

### B) LeetCode-style (guaranteed existence) — O(1) space, O(n) time

```cpp
class Solution {
public:
    int majorityElement(const std::vector<int>& nums) {
        int candidate = 0, count = 0;
        for (const int x : nums) {
            if (count == 0) candidate = x;
            count += (x == candidate) ? 1 : -1;
        }
        return candidate; // guaranteed to exist
    }
};
```

---

## Summary of feedback on your original code

- Switch to `unordered_map` or, better, Boyer–Moore for `O(1)` space.
- Add early exit if you keep counting.
- Tighten naming (`threshold`, `freq`), use `const`, avoid unnecessary copies.
- Be ready to state the **invariants and the cancellation proof** out loud.

If you want, I can turn this into a one-page “cheat sheet” you can rehearse before interviews.
