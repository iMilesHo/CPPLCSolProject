# Gas Station

- **ID:** 134
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Greedy
- **Link:** [LeetCode Problem](https://leetcode.com/problems/gas-station/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

```cpp
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        // edge case
        int n = gas.size();

        int i = 0, prefixSum = 0, candidateStart = 0;
        int totalPrefixSum = 0;
        while (i < n)
        {
            prefixSum += gas[i] - cost[i];
            totalPrefixSum += gas[i] - cost[i];
            // std::cout << "prefixSum = " << prefixSum << std::endl;
            if (prefixSum < 0)
            {
                candidateStart = ++i;
                prefixSum = 0;
            }
            else
            {
                ++i;
            }
        }
        // std::cout << "prefixSum = " << prefixSum << std::endl;
        if (totalPrefixSum < 0) return -1;
        return candidateStart;
    }
};
```

## Complexity

- **Time Complexity:** O(n) - Single pass through the array
- **Space Complexity:** O(1) extra space

# Gas Station - Solution Review

## 1. Correctness and Complexity

- **Time Complexity:**
  Your loop runs once through `n` stations. Inside, only constant-time operations are done. → **O(n)** time.
- **Space Complexity:**
  You only use a handful of variables (`prefixSum`, `totalPrefixSum`, etc.), so **O(1)** extra space.

This is optimal — no solution can do better than O(n) because you must at least examine each station once.

---

## 2. Code Review (Readability & Style)

### Issues

- **Variable names:**

  - `prefixSum` is misleading. It’s actually your _current tank balance_.
  - `totalPrefixSum` is not really a "prefix sum"; it’s the _net balance_ across all stations.

- **Clarity:**
  The logic of "reset candidate start when prefixSum < 0" is standard, but not obvious at first glance. Needs a docstring.
- **Edge cases:**
  You handle them implicitly, but adding an early check when `n == 0` makes it more robust.

### Suggested Rewrite

```cpp
class Solution {
public:
    // Returns the index of the starting gas station if possible, otherwise -1.
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int n = gas.size();
        if (n == 0) return -1;

        int start = 0;        // candidate start index
        int tank = 0;         // current fuel in tank
        int total = 0;        // net gas balance over entire trip

        for (int i = 0; i < n; i++) {
            int balance = gas[i] - cost[i];
            tank += balance;
            total += balance;

            // If tank is negative, can't reach i+1 from current start
            if (tank < 0) {
                start = i + 1; // try next station as new start
                tank = 0;      // reset tank
            }
        }

        return (total >= 0) ? start : -1;
    }
};
```

- **Tank vs. Total**:

  - `tank` checks local feasibility.
  - `total` checks global feasibility.

Much clearer.

---

## 3. Loop Invariant and Proof

**Invariant:** At the start of each iteration `i`, if the running `tank` is non-negative, then it is possible to travel from the current candidate `start` to station `i`.

- **Why it holds:**

  - Each step adds `(gas[i] - cost[i])`.
  - If at some `i` the `tank` drops below 0, then no station between `start` and `i` can be the valid start, because all would inherit a deficit before reaching `i+1`. So we must reset `start = i+1`.

- **Termination:**
  At the end of the loop:

  - If `total < 0`, then the total gas < total cost → impossible.
  - If `total >= 0`, then the current `start` is valid (guaranteed uniqueness).

This invariant is the heart of the greedy solution.

---

## 4. C++ Idioms and Style

- Use `int n = static_cast<int>(gas.size());` in production for type-safety.
- Prefer descriptive variable names over terse ones in interview settings.
- Keep `const` where possible (e.g., pass by const reference if no modification is needed).

---

## 5. Common Interview Follow-ups

1. **Harder Variants**

   - **Multiple laps allowed?** → Then it’s always -1 if total < 0, otherwise any station works.
   - **What if you can drive backward?** → More complex, requires two-pointer or prefix sum min check.
   - **Find _all_ valid start indices.** → Use prefix sum min-suffix trick (O(n)).

2. **Data Structure twist**

   - If stations change dynamically (gas/cost updates), use segment tree or Fenwick tree to maintain prefix sums.

---

## 6. Elevator Pitch (30–90 seconds)

_"We’re trying to see if we can drive around a circular set of gas stations. The key observation is that if the total gas is less than the total cost, it’s impossible. Otherwise, there’s exactly one valid start. We scan through once, tracking the current fuel balance. If the tank ever drops below zero, it means none of the stations we passed can work as a start, so we reset the start point to the next station and continue. By the end, if the global balance is non-negative, the last recorded start is the solution. This runs in O(n) time and O(1) space, which is optimal."_

---

## 7. Non-technical Analogy

Imagine each station gives you money (gas) but charges you a toll (cost). If your total income is less than total tolls, you’ll go broke. If income ≥ tolls, there’s always a place where you can start without going bankrupt — we just find that place by moving forward and restarting when we’re in debt.

---

✅ With this explanation, you can **(a)** walk through the proof rigorously in an interview, **(b)** explain it to a non-technical person, and **(c)** deliver production-ready C++ code.
