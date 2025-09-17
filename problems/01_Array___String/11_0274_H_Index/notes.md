# H-Index

- **ID:** 274
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Sorting, Counting Sort
- **Link:** [LeetCode Problem](https://leetcode.com/problems/h-index/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

My solutoin:

```cpp
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        int maxCitation = INT_MIN;
        for(auto x: citations)
        {
            maxCitation = max(maxCitation, x);
        }

        int h_index = 0;
        int furthestSearch = min(maxCitation, n);
        for (int i = 0; i <= furthestSearch; ++i)
        {
            int count = 0;
            for(int x: citations)
            {
                if (x >= i) count++;
            }

            if(count >= i)
            {
                h_index = max(h_index, i);
            }
        }

        return h_index;
    }
};
```

## Complexity

- **Time Complexity:** O(n²) in the worst case (for each h, we count citations).
- **Space Complexity:** O(1) extra space.

## 1. Time / Space Complexity, Readability, Naming, Docs

### Complexity

- **Your solution:**

  - Outer loop: up to `min(maxCitation, n)` iterations.
  - Inner loop: `O(n)` per iteration.
  - Worst case: `O(n²)` if `maxCitation ≥ n`.

- **Space:** `O(1)` extra space.

This is fine for small `n`, but too slow if `n` ≈ 10⁵ (LeetCode constraints).

### Readability & Naming

- `maxCitation` → clear.
- `furthestSearch` → good but could be `upperBound`.
- `h_index` → conventionally `hIndex`.
- Use `const int` or `auto` for loop variables.
- Comment the key invariant (see below).

### Documentation

You could add a short docstring:

```cpp
// h-index definition: max h such that at least h papers have ≥ h citations.
// Approach: try all possible h values up to min(maxCitation, n).
// For each h, count how many papers qualify. Keep the largest valid h.
```

---

## 2. Loop Invariant & Proof

**Invariant for outer loop (for each `i` from 0..furthestSearch):**

- Before iteration `i`, we have already verified all smaller candidate h-values (`< i`) and stored the largest feasible one in `h_index`.

**Why it holds:**

- Each iteration counts how many papers have ≥ `i` citations.
- If that count ≥ `i`, then `i` is a valid candidate.
- By updating `h_index = max(h_index, i)`, we ensure `h_index` always stores the largest feasible h found so far.

**Proof of correctness:**

- Exhaustive search over all possible `h`.
- At the end, `h_index` equals the maximum feasible h, which matches the definition of the h-index.

This is a good talking point in interviews: the invariant ensures you are not missing any candidate h and are monotonically improving toward the maximum.

---

## 3. Code Style & C++ Idioms

- Prefer `std::max_element` to find `maxCitation`:

```cpp
int maxCitation = *max_element(citations.begin(), citations.end());
```

- Use range-based loops with `const auto& x` to avoid copies:

```cpp
for (const auto& x : citations) { ... }
```

- Use `std::size_t` for indices (unsigned, matches `vector::size()`).

- Keep style consistent: use camelCase for variables (`hIndex` instead of `h_index`).

---

## 4. Common Follow-Ups & Harder Variants

- **Follow-ups:**

  1. Optimize from `O(n²)` to `O(n log n)` (by sorting) or `O(n)` (by counting sort–like approach).
  2. Handle **very large citation counts** (avoid scanning up to `maxCitation`).
  3. Streaming / online version: what if citations arrive one by one?
  4. **H-index II** (LeetCode 275): citations array is sorted — solve in `O(log n)` via binary search.

- **Harder variants:**

  - “G-index” (sum of top g papers ≥ g²).
  - Multi-author metrics (combine multiple researchers).
  - Dynamic updates (support adding/removing citations efficiently).

---

## 5. Elevator Pitch (30–90s)

_"The h-index is the largest number h such that at least h of a researcher’s papers have been cited h or more times. My solution checks every possible h from 0 up to n, counts how many papers meet that threshold, and tracks the maximum valid h. It’s simple and always correct. This brute-force approach runs in O(n²) time, which works for small datasets but can be optimized. In practice, sorting the citations array gives an O(n log n) solution, or we can use a counting approach for O(n). The idea is intuitive: we’re balancing the number of papers with the depth of their citations."_

---

## 6. Optimizations / Alternative Perspectives

- **Sorting approach (O(n log n)):**

```cpp
class Solution {
public:
    int hIndex(vector<int>& citations) {
        sort(citations.begin(), citations.end());
        int n = citations.size();

        int hIndex = 0;
        for(int i = 0; i < n; i++)
        {
            if(citations[i] >= n - i)
            {
                hIndex = n - i;
                break;
            }
        }
        return hIndex;
    }
};
```

- **Counting approach (O(n)):**

  - Count how many papers have each citation count (capped at n).
  - Walk backwards to find the largest h.

```cpp
int hIndex(vector<int>& citations) {
    int n = citations.size();
    vector<int> count(n+1, 0);
    for (int c : citations) {
        count[min(c, n)]++;
    }
    int total = 0;
    for (int i = n; i >= 0; i--) {
        total += count[i];
        if (total >= i) return i;
    }
    return 0;
}
```

This is **production-grade**: optimal `O(n)` time, `O(n)` space, handles large citation values safely.

---

## 7. Production-Grade Solution (Final)

Here’s a clean, efficient, documented version:

```cpp
class Solution {
public:
    // O(n) time, O(n) space solution using counting approach
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        vector<int> count(n+1, 0);

        // Cap citation counts at n, since h cannot exceed n
        for (int c : citations) {
            count[min(c, n)]++;
        }

        int total = 0;
        // Traverse backwards: find largest i with at least i papers
        for (int i = n; i >= 0; --i) {
            total += count[i];
            if (total >= i) return i;
        }
        return 0;
    }
};
```

---

✅ Summary:

- Your code works but is `O(n²)`.
- Add loop invariants and consistent style.
- Learn the `O(n log n)` sort-based and `O(n)` counting approaches — these are what interviewers expect.
- Practice follow-ups (sorted input, online updates).
- Be able to **explain both the brute-force intuition and the optimized production version**.
