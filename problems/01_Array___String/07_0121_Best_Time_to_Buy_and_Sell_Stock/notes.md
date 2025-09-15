# Best Time to Buy and Sell Stock

- **ID:** 121
- **Difficulty:** EASY
- **Topic Tags:** Array, Dynamic Programming
- **Link:** [LeetCode Problem](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

My initial solution:

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int maxProfit = 0;
        for (size_t buy = 0; buy < prices.size(); ++buy)
        {
            // std::cout << "buy = " << buy << ", ";
            for (size_t sell = buy + 1; sell < prices.size(); ++sell)
            {
                maxProfit = std::max(maxProfit, prices[sell] - prices[buy]);
                // std::cout << "sell = " << sell << ", max = " << maxProfit << std::endl;
            }
        }
        return maxProfit;
    }
};
```

## Complexity

- **Time Complexity:** O(n^2)
- **Space Complexity:** O(1)

## 1. Complexity, Readability, Naming

### Complexity

- **Time complexity:**
  Your solution uses two nested loops → $O(n^2)$.
  For $n = 10^5$ (LeetCode constraint), this is too slow.
- **Space complexity:**
  $O(1)$ — no extra memory used beyond a few integers.

### Readability

- `maxProfit` (variable) has the same name as the function — avoid shadowing/confusion.
- Loop variables `buy` and `sell` are descriptive, but since they represent **indices**, consider naming them `i` and `j` or adding comments.

### Documentation

- No comments: in interviews, you should add at least one line explaining the algorithm idea.

---

## 2. Loop Invariant

For the **optimized solution** (single pass), the key invariant is:

- **Invariant:** At index `i`, we always know the **minimum price so far** (`minPrice`) before day `i`.
- **Why it holds:** Each iteration compares `prices[i]` with `minPrice`. If smaller, update `minPrice`. This guarantees `minPrice` is always the lowest price before or at day `i`.
- **How it helps:** Profit at day `i` = `prices[i] - minPrice`. We can compute the best possible profit ending at day `i` using only this invariant.
- **Termination:** After scanning all days, `maxProfit` holds the global maximum.

For your brute-force solution:

- **Invariant:** At iteration `(buy, sell)`, you’ve already checked **all previous pairs** and stored the maximum profit seen.
- This is valid, but much less efficient.

---

## 3. C++ Style and Idioms

- Prefer **range-based for loops** when not using indices (not ideal here since you need indices).
- Use `int` for indices when safe, but if you’re working with large data, `size_t` is fine.
- `std::max` is good; alternatively, you can maintain `maxProfit` with a ternary operator for readability.
- Avoid shadowing names (e.g., rename `maxProfit` variable → `max_profit`).

---

## 4. Common Follow-ups

1. **Multiple transactions allowed** (LeetCode 122).

   - Greedy: sum all increasing segments.

2. **At most two transactions** (LeetCode 123).

   - DP with split states.

3. **At most k transactions** (LeetCode 188).

   - Generalized DP.

4. **With transaction fee** (LeetCode 714).

   - Adjust profit when selling.

5. **Cooldown period** (LeetCode 309).

   - DP with states (hold, sell, cooldown).

---

## 5. Elevator Pitch (30–90 sec)

_"We want to buy low and sell high. A brute-force approach checks every possible buy-sell pair, but that’s too slow. Instead, as we scan prices from left to right, we keep track of the lowest price we’ve seen so far. At each step, we compute the profit if we sold today and update the maximum profit. This way, we only need one pass, making the algorithm run in linear time $O(n)$ with constant space $O(1)$. In simple terms, we’re always asking: ‘What if I had bought at the cheapest time so far — how much could I make by selling today?’ That’s why it’s efficient and elegant."_

---

## 6. Alternative Perspectives / Optimizations

- **Kadane’s Algorithm Analogy:**
  You can think of it as finding the maximum subarray sum of consecutive daily profit differences (`prices[i+1]-prices[i]`).
- **Production-readiness:**

  - Add input validation (empty array → return 0).
  - Use descriptive variable names.
  - Write unit tests for edge cases (`[]`, `[1]`, `[2,1]`, strictly decreasing).

---

## 7. Optimized Production-Ready Solution

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;

        int minPrice = prices[0];
        int maxProfit = 0;

        for (size_t i = 1; i < prices.size(); ++i) {
            // Profit if selling today
            int profit = prices[i] - minPrice;
            maxProfit = std::max(maxProfit, profit);

            // Update the lowest price seen so far
            minPrice = std::min(minPrice, prices[i]);
        }

        return maxProfit;
    }
};
```

- **Time complexity:** $O(n)$
- **Space complexity:** $O(1)$
- **Invariant:** `minPrice` always holds the minimum price so far.
- **Style:** clear, descriptive, safe for production.
