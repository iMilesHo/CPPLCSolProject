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

# 229. Majority Element II

## My Approach

```cpp
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        const int threshold = static_cast<int>(nums.size()/3);
        std::unordered_map<int, int> freq;
        freq.reserve(nums.size()+1);
        vector<int> result;
        for(const int x: nums)
        {
            freq[x] += 1;
        }

        for(const auto& kv: freq)
        {
            if (kv.second > threshold)
            {
                result.push_back(kv.first);
            }
        }

        return result;
    }
};
```

Great problem choice! Your hashmap solution is correct and quite readable. Below is a structured review and an optimized alternative that’s ideal for interviews and production.

---

# 1) Complexity, readability, naming, docs

### Your approach (frequency map)

- **Correctness:** ✅ finds all elements with count `> ⌊n/3⌋`.
- **Time:** `O(n)` — single pass to count + `O(k)` to scan the map (`k` distinct values, `k ≤ n`).
- **Space:** `O(k)` for the hash map (can be `O(n)` worst-case).
- **Readability & naming:** Clear. Minor nits:

  - Use `size_t` (or `std::size_t`) for sizes to avoid signed/unsigned mismatches.
  - Reserve with an estimate of distinct keys (unknown) rather than `nums.size()+1`. Over-reserving increases memory; you can omit `reserve()` or use a heuristic.
  - Prefer `const auto& [val, cnt] : freq` for clarity.
  - Add a brief comment on the threshold logic.

A lightly polished version of your approach:

```cpp
class Solution {
public:
    std::vector<int> majorityElement(const std::vector<int>& nums) {
        const std::size_t n = nums.size();
        const std::size_t threshold = n / 3;

        std::unordered_map<int, std::size_t> freq;
        // freq.reserve(n); // optional: may over-allocate; safe to omit

        for (int x : nums) {
            ++freq[x];
        }

        std::vector<int> result;
        result.reserve(2); // at most 2 elements can be > n/3
        for (const auto& [val, cnt] : freq) {
            if (cnt > threshold) result.push_back(val);
        }
        return result;
    }
};
```

---

# 2) Loop invariant & proof (Boyer–Moore for n/3)

Interviewers often expect the **O(1) space** solution: a 2-candidate Boyer–Moore majority vote generalization.

### Key idea

- There can be **at most two** numbers that appear more than `n/3` times.
- Maintain two candidates `(c1, c2)` with counts `(k1, k2)`.

### Algorithm (single pass, then verify)

For each `x`:

1. If `x == c1` → `k1++`
2. else if `x == c2` → `k2++`
3. else if `k1 == 0` → `c1 = x, k1 = 1`
4. else if `k2 == 0` → `c2 = x, k2 = 1`
5. else → `k1--, k2--` (cancel one vote from both)

After the pass, `c1`/`c2` are **candidates** only; do one more pass to count and confirm.

### Invariant (what always holds after processing any prefix)

> Among all values in the processed prefix, if we **repeatedly remove triples of distinct elements** (one vote for three different values), the remaining multiset can contain **at most two distinct values**, and those values are exactly the current `(c1, c2)` tracked with counts `(k1, k2)`.

**Why it holds:**

- Step 5 “cancels” one from both counters, equivalent to removing one occurrence from each of three distinct values: `c1`, `c2`, and the new `x` (which differs from both).
- If one counter is 0, we replace that candidate — akin to reintroducing a new value because there’s room among the “survivors”.
- Therefore, after any number of cancellations, survivors can’t exceed 2 distinct values; we store them in `(c1, c2)`.

**How this proves correctness:**

- Any element with global frequency `> n/3` **cannot be fully canceled** by forming triples with other values, so it must survive in the final pair of candidates.
- Final verification counts ensure we return only those truly `> n/3`.

---

# 3) C++ style & idioms

- Use `std::size_t` for sizes; avoid narrowing casts.
- Reserve realistic capacities (`result.reserve(2)` for this problem).
- Structured bindings for clarity.
- Prefer early `const` and narrow scopes.
- Keep the counting pass & verification pass clearly separated.

---

# 4) Common follow-ups & harder variants

- **Follow-up 1:** Achieve **O(1) extra space** → Boyer–Moore (below).
- **Follow-up 2:** Generalize to **elements > n/k** → keep `k-1` candidates and counts; time `O(nk)` space `O(k)`, plus verification.
- **Follow-up 3:** **Streaming / online** data → Boyer–Moore works in one pass; you can checkpoint and periodically verify on a sample.
- **Follow-up 4:** **Distributed** setting → run Boyer–Moore per shard to get local candidates; merge/summarize and run verification on the union.
- **Follow-up 5:** Stability to adversarial inputs → verification pass is mandatory; counters alone are insufficient.
- **Follow-up 6:** If `nums` is **read-only** or memory constrained → Boyer–Moore minimizes auxiliary memory and keeps cache-friendly scans.

---

# 5) 30–90 second elevator pitch (non-technical friendly)

> We’re looking for numbers that appear really often—more than a third of the list. It turns out there can be at most two such numbers.
> Instead of counting everything, I keep track of just two “candidates” and their tallies while scanning the list once. When a new value doesn’t match either candidate and both tallies are positive, I reduce both tallies—this mimics canceling out a balanced set of different numbers.
> After one pass, only the truly frequent values can survive as candidates. I then do a quick second pass to confirm their actual counts.
> This runs in linear time, uses constant extra memory, and returns up to two results.

---

# 6) Production-grade, optimized solution (Boyer–Moore + verification)

```cpp
#include <vector>
#include <limits>
#include <cstddef>

class Solution {
public:
    std::vector<int> majorityElement(const std::vector<int>& nums) {
        // Phase 1: Find up to two candidates.
        int c1 = 0, c2 = 0;
        int k1 = 0, k2 = 0;

        for (int x : nums) {
            if (x == c1) {
                ++k1;
            } else if (x == c2) {
                ++k2;
            } else if (k1 == 0) {
                c1 = x; k1 = 1;
            } else if (k2 == 0) {
                c2 = x; k2 = 1;
            } else {
                --k1; --k2;
            }
        }

        // Phase 2: Verify counts.
        std::size_t n = nums.size();
        std::size_t cnt1 = 0, cnt2 = 0;
        for (int x : nums) {
            if (x == c1) ++cnt1;
            else if (x == c2) ++cnt2;
        }

        const std::size_t threshold = n / 3;
        std::vector<int> ans;
        ans.reserve(2);
        if (cnt1 > threshold) ans.push_back(c1);
        if (cnt2 > threshold && c2 != c1) ans.push_back(c2);
        return ans;
    }
};
```

**Why this is production-ready**

- Clear two-phase structure (candidates → verify).
- Minimal allocations (`reserve(2)`).
- No undefined behavior or integer-sign pitfalls.
- Linear time, constant extra space.
- Works for negative numbers and duplicates; no assumptions beyond `int`.

---

# 7) Invariants you can say out loud (quick script)

- **Invariant:** After processing any prefix, if you repeatedly remove triples of distinct values, the survivors are at most two distinct values; those are exactly my `(c1, c2)` with positive counts.
- **Why it holds:** Every time I meet a third different value while both counts are positive, I decrement both counts—this simulates deleting one of each of three distinct values. If a counter hits zero, I “free” that slot to track a new potential survivor.
- **How it helps:** Since elements occurring more than `n/3` times cannot be fully canceled by triples, any real majority-by-third must be one of the two survivors. A final count confirms it.

---

## Bonus: When to use which

- **Interview / large data / low memory:** Boyer–Moore (above).
- **Simple & quick to implement, memory okay:** Your hashmap solution.

If you’d like, I can also provide the generalized `> n/k` version template.
