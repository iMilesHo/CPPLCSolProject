# Trapping Rain Water

- **ID:** 42
- **Difficulty:** HARD
- **Topic Tags:** Array, Two Pointers, Dynamic Programming, Stack, Monotonic Stack
- **Link:** [LeetCode Problem](https://leetcode.com/problems/trapping-rain-water/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

```cpp
class Solution
{
  public:
    int trap(vector<int>& height)
    {                           // edge cases
        int n = height.size();  //
        int leftMin = 0, rightMax = 0;
        for (int i = 0; i < n; i++)
        {
            int cI = i;
            // center index
            // search left
            int lI = cI;
            while (lI >= 0)
            {
                if (height[lI - 1] <= height[lI] &&
                    height[lI] >= 1)  // search right
                    {
                        leftMin = height[lI];
                        break;
                    }
            }
        }
    }
};
```

## Complexity

- **Time Complexity:** (TODO)
- **Space Complexity:** (TODO)

Great question — this is one of the most important “hard” problems in LeetCode, and interviewers love to test how well you can reason about invariants, tradeoffs, and clean C++ implementation. Let’s break down a systematic review for you.

---

## 1. Time & Space Complexity, Code Readability, Variable Naming

- Your current code is incomplete and tries to “search left and right” per index. That would become **O(n²)** in the worst case (scanning left/right for each bar). This is not acceptable for `n = 20,000`.
- The optimal approaches run in **O(n)** time:

  - **Two-pointer approach**: scanning from both ends.
  - **Prefix/suffix precomputation**: computing leftMax/rightMax arrays, then one pass.

- Space:

  - Two-pointer approach → **O(1)** extra space.
  - Prefix/suffix arrays → **O(n)** space.

- Variable naming: Avoid cryptic names like `cI`, `lI`, `rI`. Prefer `left`, `right`, `leftMax`, `rightMax`. This matters in interviews.

---

## 2. Loop Invariant (Two-Pointer Approach)

**Invariant:** At any step, whichever side (`left` or `right`) has the smaller boundary determines the water trapped at that position.

- Why? Water is bounded by the _minimum_ of leftMax and rightMax. If `leftMax < rightMax`, then the trapped water at `left` depends only on `leftMax` (since the right boundary is guaranteed to be higher).
- This invariant ensures that you only move one pointer at a time and compute trapped water safely.
- Proof idea:

  - If `leftMax <= rightMax`, then no matter what is on the right, the right boundary is higher. So the water at `left` is `leftMax - height[left]`. You can safely move `left++`.
  - Symmetric reasoning applies for `right`.

---

## 3. C++ Idioms & Style

- Use **`std::vector<int>&`** with `const` when possible (`const std::vector<int>& height`) if you don’t modify it.
- Prefer **structured loops** (`for` or `while`) with clear conditions.
- Document logic inline with short comments.
- Don’t leave half-finished while loops — clarity matters in interviews.

---

## 4. Common Follow-ups & Harder Variants

- **Follow-ups:**

  1. Implement with prefix/suffix arrays (space O(n)).
  2. Optimize to two pointers (space O(1)).
  3. Ask about edge cases: empty array, all zeros, monotonic increasing/decreasing.

- **Variants:**

  - Trapping Rain Water II (3D version on a grid) — uses a priority queue (min-heap).
  - Skyline problems (building outline, etc.).

---

## 5. Elevator Pitch (30–90s)

> The problem is about finding trapped rainwater between bars. The key insight is that water at any index is bounded by the minimum of the tallest bar to its left and right. A brute-force approach checks both sides for every index, which is O(n²). Instead, we can use two pointers: keep track of the maximum height seen so far from the left and right. At each step, the smaller boundary determines the water trapped at that side. This runs in O(n) time and O(1) space. In simple terms: we “squeeze” from both ends, always filling from the lower wall, until the two pointers meet. It’s efficient and works in a single pass.

---

## 6. Production-Ready C++ Solution

```cpp
class Solution {
public:
    int trap(const vector<int>& height) {
        int n = height.size();
        if (n == 0) return 0;

        int left = 0, right = n - 1;
        int leftMax = 0, rightMax = 0;
        int water = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= leftMax) {
                    leftMax = height[left];
                } else {
                    water += leftMax - height[left];
                }
                ++left;
            } else {
                if (height[right] >= rightMax) {
                    rightMax = height[right];
                } else {
                    water += rightMax - height[right];
                }
                --right;
            }
        }
        return water;
    }
};
```

- **Time Complexity:** O(n) (one pass).
- **Space Complexity:** O(1) (constant extra memory).
- **Readability:** clean variable names, short explanatory comments possible.

---

## 7. Next-Step Practice

- **Implement the prefix/suffix approach** (easier to explain, though less optimal).
- **Practice Trapping Rain Water II** (3D, heap-based).
- **Try explaining the loop invariant** aloud — interviewers love when you reason about correctness, not just write code.
- **Coding exercise**: implement both solutions and unit test them with edge cases.

---

## Other Solutions

- Brute-force (O(n²)) — for completeness, but not optimal.

```cpp
class Solution
{
  public:
    // brute force solution:
    // for each point, we find the left max, and right max
    // the water at this point it can hole is
    // min(leftMax, rightMax) - height[i] (only if it > 0)
    int trap(const vector<int>& height)
    {
        int n = height.size();
        if (n < 2) return 0;  // can't hold water when less than 3
        int water = 0;

        for (int i = 0; i < n; i++)
        {
            // search left max
            int leftMax = 0;
            for (int j = 0; j < i; ++j)
            {
                leftMax = max(height[j], leftMax);
            }

            // search right max
            int rightMax = 0;
            for (int j = n - 1; j > i; --j)
            {
                rightMax = max(height[j], rightMax);
            }

            int tmp = min(leftMax, rightMax) - height[i];

            if (tmp > 0)
            {
                water += tmp;
            }
        }
        return water;
    }
};
```

- **DP Prefix/Suffix Arrays** (O(n) time, O(n) space):

```cpp
class Solution
{
  public:
    int trap(const vector<int>& height)
    {
        int n = height.size();
        if (n < 2) return 0;

        vector<int> leftMax(n, 0);
        vector<int> rightMax(n, 0);

        leftMax[0] = height[0];
        for (int i = 1; i < n; ++i)
        {
            leftMax[i] = max(leftMax[i - 1], height[i]);
        }

        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; --i)
        {
            rightMax[i] = max(rightMax[i + 1], height[i]);
        }

        int water = 0;
        for (int i = 0; i < n; ++i)
        {
            water += min(leftMax[i], rightMax[i]) - height[i];
        }

        return water;
    }
};
```

- Monotonic Stack

```cpp
class Solution
{
  public:
    int trap(const vector<int>& height)
    {
        int n = height.size();
        if (n < 2) return 0;

        stack<int> st;  // store indices
        int water = 0;

        for (int i = 0; i < n; ++i)
        {
            while (!st.empty() && height[i] > height[st.top()])
            {
                int top = st.top();
                st.pop();
                if (st.empty()) break;

                int distance = i - st.top() - 1;
                int boundedHeight = min(height[i], height[st.top()]) - height[top];
                water += distance * boundedHeight;
            }
            st.push(i);
        }
        return water;
    }
};
```

-
