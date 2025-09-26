# Candy

- **ID:** 135
- **Difficulty:** HARD
- **Topic Tags:** Array, Greedy
- **Link:** [LeetCode Problem](https://leetcode.com/problems/candy/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

````cpp
class Solution
{ public:
  int candy(vector<int>& ratings) {
    int n = ratings.size();
    vector<int> record(n,1);
    int miniValue = record[0]; int sum = record[0];
    for (int i = 1; i < n; i++)
    {
      if (ratings[i]>ratings[i-1])
      {
        record[i] = record[i-1] + 1;
      }
      else if (ratings[i]<ratings[i-1])
      {
        record[i] = record[i-1] - 1;
      }
      sum += record[i];
      miniValue = min(record[i], miniValue);
      }
      if (miniValue <= 0) { sum += (1 - miniValue)*n;
    }
      return sum;
  }
};
```

## Complexity

- **Time Complexity:** (TODO)
- **Space Complexity:** (TODO)

Great choice of problem — **LeetCode 135. Candy** is a famous hard-tagged greedy problem. Let’s go step by step to review your attempt, then I’ll give you a correct production-ready solution, an invariant explanation, and interview-style follow-ups.

---

## 1. Review of Your Code

### Correctness

- ❌ Your code does **not always satisfy the problem conditions**.
  Example: `ratings = [1,0,2]`.

  - Your logic produces `record = [1,0,1]` → sum = 2, then adjusts by shifting to positive → result = 5 (correct output, but coincidental).
  - For other inputs like `[1,3,2,2,1]`, it breaks. The decrement approach doesn’t ensure local minima get exactly one candy and may assign invalid distributions.

### Complexity

- **Time Complexity**: O(n).
- **Space Complexity**: O(n) due to the `record` vector.
  That’s fine — optimal solutions are O(n) time and space, though O(1) space is possible with careful tricks.

### Code Readability & Naming

- `record` → better as `candies` (meaningful).
- `miniValue` → better as `minCandies` or `minAssigned`.
- Lacks **comments** about the greedy reasoning.

### Documentation

- No explanation of algorithm steps, invariants, or why post-processing (adding `(1 - min) * n`) is valid.
- For production or interviews, you must clarify why the logic ensures constraints.

---

## 2. Loop Invariant (Greedy Method)

The **correct greedy algorithm** uses two passes:

1. **Left-to-right pass**:
   Invariant → After processing index `i`, if `ratings[i] > ratings[i-1]`, then `candies[i] = candies[i-1] + 1`; else `candies[i] ≥ 1`.
   This guarantees left neighbor condition.

2. **Right-to-left pass**:
   Invariant → After processing index `i`, if `ratings[i] > ratings[i+1]`, then `candies[i] = max(candies[i], candies[i+1] + 1)`; else unchanged.
   This guarantees right neighbor condition.

Together, both invariants ensure every child has at least 1 candy, and higher ratings get more than neighbors.

---

## 3. Production-Ready Solution (C++)

```cpp
class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        vector<int> candies(n, 1);

        // Pass 1: left to right
        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i-1]) {
                candies[i] = candies[i-1] + 1;
            }
        }

        // Pass 2: right to left
        for (int i = n - 2; i >= 0; i--) {
            if (ratings[i] > ratings[i+1]) {
                candies[i] = max(candies[i], candies[i+1] + 1);
            }
        }

        return accumulate(candies.begin(), candies.end(), 0);
    }
};
````

✅ Correct, simple, and widely accepted.

---

## 4. Code Style & C++ Idioms

- Use `std::vector<int> candies(n, 1)` for clarity.
- Use `std::accumulate` instead of manual sum loop.
- Prefer `const` references (`const vector<int>& ratings`) when not modifying.
- Add **comments** for each pass.

---

## 5. Elevator Pitch (30–90 seconds)

_"We need to give candies to children based on ratings, with two rules: at least one candy each, and higher ratings get more than neighbors. A greedy approach works: we scan left-to-right, ensuring each child has more than the left neighbor if rated higher. Then we scan right-to-left, fixing the right neighbor condition. This takes linear time and space, O(n), which is efficient even for 20k children. The result is the minimum total because each pass only adds candies when absolutely necessary."_

---

## 6. Common Interview Follow-Ups

1. **Space optimization**: Can we solve it in O(1) space? (Yes, but tricky — usually interviewer doesn’t expect it; the O(n) solution is standard.)
2. **Variants**:

   - What if children sit in a circle instead of a line?
   - What if equal ratings must also have equal candies?
   - What if we want lexicographically smallest distribution?

3. **Proof of correctness**: Why two passes are enough? (Because neighbor relation is local, and fixing one direction may break the other, but two passes cover both.)

---

## 7. Alternative Perspective

This problem is essentially about **local slopes**:

- Increasing slope → increment candies.
- Decreasing slope → adjust in backward pass.
- Flat slope → reset to 1.

Some people call this the **mountain/valley method**.

---

✅ **Summary**:
Your attempt was creative but not correct in general. The standard greedy two-pass solution is provably correct, readable, and production-ready. Focus in interviews on stating the **invariants** and why two passes are necessary.

---

Would you like me to also show you how to **prove optimality formally** (exchange argument + greedy proof), so you can defend your solution in a hard interview?
