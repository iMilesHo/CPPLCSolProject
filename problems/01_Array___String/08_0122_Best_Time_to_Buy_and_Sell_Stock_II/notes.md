# Best Time to Buy and Sell Stock II

- **ID:** 122
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Dynamic Programming, Greedy
- **Link:** [LeetCode Problem](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

My solution:

```cpp
class Solution
{
public:
    int maxProfit(vector<int>& prices)
    {
        int totalProfit = 0;
        for(size_t i = 1; i < prices.size(); ++i)
        {
            int profit = prices[i] - prices[i-1];
            // Add only positive profits
            // which means we are selling at a peak
            if (profit > 0)
            {
                totalProfit += profit;
            }
        }

        return totalProfit;
    }
};
```

## Complexity

- **Time Complexity:** O(n)
- **Space Complexity:** O(1)

## 1. Complexity, Readability, Naming, and Documentation

### Complexity

- **Time Complexity**:
  The loop goes through the array once → **O(n)**.
- **Space Complexity**:
  Uses only a few variables → **O(1)**.

This is optimal: you cannot do better than O(n) because you must read all prices.

### Readability & Naming

- `maxProfit` (function name) and `maxProfit` (variable) overlap in naming. It’s better to rename the local variable to `totalProfit` or `profitSum` to avoid confusion.
- A short inline comment before the loop would help clarify the greedy reasoning.

Example:

```cpp
// We add every positive price difference (uphill movement) to maximize profit.
```

### Documentation

A brief doc comment would help in production code:

```cpp
/**
 * Computes maximum profit from unlimited transactions.
 * Greedy: sum all positive differences between consecutive days.
 * @param prices: daily stock prices
 * @return maximum achievable profit
 */
```

---

## 2. Loop Invariant & Proof

### Invariant

At iteration `i` (1 ≤ i < n):
**`totalProfit` equals the maximum profit achievable from days `[0..i]`.**

### Why it Holds

- If `prices[i] > prices[i-1]`, we add `(prices[i] - prices[i-1])`.
  → This simulates buying on day `i-1` and selling on day `i`.
- If `prices[i] <= prices[i-1]`, no profit is possible, so we skip.
- At each step, the decision is local and greedy, but it’s safe:
  Every “uphill” contributes profit, and summing them is equivalent to capturing the global maxima.

### How it Helps

In interviews, saying **“my loop invariant is that at each index, the profit tracked so far is optimal up to that point”** shows that:

- You understand correctness, not just code.
- You can explain why greedy works (local optimal → global optimal).

---

## 3. Code Style & C++ Idioms

- Use `int totalProfit = 0;` (clearer than `maxProfit` as a local var).
- Use `auto` in modern C++ if desired:

  ```cpp
  for (size_t i = 1; i < prices.size(); ++i)
  ```

  is fine, but

  ```cpp
  for (size_t i = 1; i < prices.size(); ++i) {
      const int diff = prices[i] - prices[i - 1];
      if (diff > 0) totalProfit += diff;
  }
  ```

  with `const` adds clarity.

- Avoid `[1]` placeholder comment at the top unless it refers to something.

---

## 4. Common Interview Follow-ups & Harder Variants

1. **Transaction Fee Variant** (LeetCode 714):
   Each transaction costs a fee. You can solve with DP:

   ```
   hold[i] = max(hold[i-1], notHold[i-1] - prices[i])
   notHold[i] = max(notHold[i-1], hold[i-1] + prices[i] - fee)
   ```

2. **Cooldown Variant** (LeetCode 309):
   After selling, you must cooldown 1 day. Again solved with DP states.
3. **At Most k Transactions** (LeetCode 188):
   Requires DP with transaction count dimension.
4. **Single Transaction Only** (LeetCode 121):
   Classic min-so-far approach.

👉 Interviewers often pivot from the greedy “sum profits” to these DP variants.

---

## 5. Elevator Pitch (30–90 seconds)

> The problem asks for maximum profit with unlimited transactions. The key observation is that every rising edge — when today’s price is higher than yesterday’s — represents a chance for profit. Instead of trying to time exact valleys and peaks, we simply add up all positive differences between consecutive days. This greedy approach works because the sum of small climbs equals the total climb between any valley and peak. We process prices in one pass, so the algorithm runs in linear time and constant space. It’s efficient and scalable even for large inputs.

---

## 6. Alternative Perspectives / Optimizations

- Mathematically, the greedy solution is equivalent to summing all `(peak - valley)` profits. Instead of explicitly finding peaks/valleys, we add consecutive gains. This reduces implementation complexity.
- No further runtime optimization possible — O(n) is best.
- If memory efficiency is critical, your current solution is already O(1).

---

## 7. Production-Ready Version

```cpp
#include <vector>
using std::vector;

/**
 * @brief Calculate max profit with unlimited transactions
 *
 * Greedy insight: profit is accumulated by summing every
 * positive difference between consecutive days. This is
 * equivalent to capturing all rises from valleys to peaks.
 *
 * @param prices Vector of daily stock prices
 * @return int Maximum achievable profit
 */
class Solution {
public:
    int maxProfit(const vector<int>& prices) {
        int totalProfit = 0;
        for (size_t i = 1; i < prices.size(); ++i) {
            const int diff = prices[i] - prices[i - 1];
            if (diff > 0) totalProfit += diff;
        }
        return totalProfit;
    }
};
```

---

✅ You already have the optimal solution. Improvements are mostly around:

- **naming** (`totalProfit` vs `maxProfit`),
- **explanation** (loop invariant, greedy correctness),
- **style/documentation** for interviews or production.
