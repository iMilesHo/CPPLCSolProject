# Jump Game II

- **ID:** 45
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Dynamic Programming, Greedy
- **Link:** [LeetCode Problem](https://leetcode.com/problems/jump-game-ii/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

My inital idea: use DFS backtracking to explore all possible jump paths, keeping track of the minimum steps found.

```cpp
class Solution {
public:
    void dfs(int i, const vector<int>& nums, int steps, int& minSteps)
    {
        if (i == nums.size() - 1)
        {
            minSteps = min(minSteps, steps);
            return;
        }

        if (steps > minSteps) return;

        int furthestJump = min(i + nums[i], static_cast<int>(nums.size()) - 1);
        for(int k = i+1; k <= furthestJump; k++)
        {
            dfs(k, nums, steps+1, minSteps);
        }

        return;
    }

    int jump(vector<int>& nums) {
        int minSteps = nums.size() + 1;
        dfs(0, nums, 0, minSteps);
        return minSteps;
    }
};
```

Improved version with memoization to avoid re-computing states:

```cpp
class Solution {
public:
    int dfs(int pos, const vector<int>& nums, vector<int>& memo)
    {
        int n = static_cast<int>(nums.size());
        if (pos == n - 1) return 0; // already at the last step

        if (memo[pos] != -1) return memo[pos]; // minJumps from index i already calculated

        int furthestJump = min(pos + nums[pos], n - 1);

        int minJumps = n;
        for(int k = pos+1; k <= furthestJump; k++)
        {
            int subSteps = dfs(k, nums, memo);
            minJumps = min(minJumps, 1 + subSteps);
        }

        return memo[pos] = minJumps;
    }

    int jump(vector<int>& nums) {
        vector<int> memo(nums.size(), -1);

        return dfs(0, nums, memo);
    }
};
```

We can use stack to simulate recursion but it is hard to apply memoization in iterative DFS.

```cpp
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>
using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        int minSteps = n; // large initial value

        stack<pair<int,int>> st; // (index, steps)
        st.push({0, 0});

        while (!st.empty()) {
            auto [i, steps] = st.top();
            st.pop();

            // Reached end
            if (i == n - 1) {
                minSteps = min(minSteps, steps);
                continue;
            }

            // Prune if already worse
            if (steps >= minSteps) continue;

            int furthest = min(i + nums[i], n - 1);
            // Push children (next indices) onto stack
            for (int k = i + 1; k <= furthest; k++) {
                st.push({k, steps + 1});
            }
        }

        return minSteps;
    }
};
```

## Complexity

- **Time Complexity:** O(n^2) in the worst case for the provided DFS solution with memoization.
- **Space Complexity:** O(n) for the memo array + recursion stack depth up to O(n).

## 1. Complexity, Readability, Naming, Documentation

### Complexity

- **Time Complexity**:
  Your code explores **all possible jump paths**. In the worst case, each position can jump to many future positions, creating a branching factor. That makes the complexity **exponential (O(b^n))**, where `b` ≈ average jump length.
  For example, in `[2,3,1,1,4]`, from index 0 you explore 2 branches, from index 1 you explore 3 branches, etc. This grows too fast to handle large `n`.
- **Space Complexity**:
  Stack depth up to `O(n)` (in the case of always jumping 1 step).

This is why the DFS will **TLE** on big inputs (n up to 10⁴).

### Readability & Naming

- `dfs` is fine, but `i` could be `pos` or `index` for clarity.
- `minSteps` as a reference parameter is clear but mutable global state; it could be wrapped in a class field.
- Adding a comment for `furthestJump` would help:

```cpp
// Explore all reachable indices from current position
int furthestJump = min(i + nums[i], n - 1);
```

### Documentation

A brief docstring makes it easier for reviewers:

```cpp
/**
 * Explore all possible jump paths using DFS backtracking.
 * Updates minSteps with the minimal jump count.
 * Time complexity: exponential, not feasible for large n.
 */
```

---

## 2. Loop / Recursion Invariant

**Invariant** for `dfs(i, steps, minSteps)`:

- At the start of each call, `steps` is the number of jumps taken to reach index `i`.
- The recursion ensures that **all paths from index 0 to i** have been considered with their respective step counts.
- The global `minSteps` always stores the best solution found so far.

**Why it holds**:

- Base case updates `minSteps` when reaching the end.
- Pruning (`if steps > minSteps return`) guarantees we never explore worse solutions.

**How it helps**:
It makes correctness clear: at every recursive call, you know the number of steps is valid, and you only improve `minSteps` when a better path is found.

---

## 3. Code Style & C++ Idioms

- `static_cast<int>(nums.size()) - 1` is correct, but you could pre-store `int n = nums.size();` for brevity.
- In modern C++, prefer passing vectors by `const &` when not modifying. You already do this in `dfs`. ✅
- You could consider `std::numeric_limits<int>::max()` instead of `nums.size() + 1` for initial `minSteps`.

---

## 4. Common Interview Follow-ups & Harder Variants

1. **Greedy Optimal Solution**
   This problem has a known greedy solution in O(n) time:

   - Maintain the farthest index reachable within the current jump.
   - When you reach the end of this range, you must make another jump, and update the range to the farthest reachable seen so far.
   - This guarantees the minimum number of jumps.

   Example implementation:

   ```cpp
   int jump(vector<int>& nums) {
       int jumps = 0, currEnd = 0, currFarthest = 0;
       for (int i = 0; i < nums.size() - 1; ++i) {
           currFarthest = max(currFarthest, i + nums[i]);
           if (i == currEnd) {
               jumps++;
               currEnd = currFarthest;
           }
       }
       return jumps;
   }
   ```

   - **Time Complexity**: O(n)
   - **Space Complexity**: O(1)

2. **Variant: Minimum Path**
   Instead of just the number of jumps, sometimes you are asked to return the actual path (indices jumped to). That requires storing predecessors.

3. **Variant: Jump Game I (Reachability)**
   Simpler variant: just decide if reaching the end is possible (LeetCode 55).

---

## 5. Elevator Pitch (30–90 Seconds)

> This problem asks for the minimum number of jumps to reach the last index. My initial DFS solution tries all possible paths, which guarantees correctness but is too slow for large inputs since it explores exponentially many possibilities. The optimal approach uses a greedy strategy: as we traverse the array, we track the farthest index we can reach with the current jump. Whenever we reach the boundary of this range, we increment the jump count and update the range with the farthest reachable index seen so far. This runs in linear time and constant space, making it efficient and scalable for large inputs.

---

## 6. Alternative Perspectives

- **DP solution**:
  `dp[i] = min number of jumps to reach i`. This works but is O(n²), which is better than DFS but worse than greedy.
- **BFS solution**:
  Model the problem as a graph where edges are jumps; then use BFS to find the shortest path. Complexity O(n²).

```cpp
#include <vector>
#include <queue>
#include <utility> // for pair
using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return 0;  // already at last index

        vector<bool> visited(n, false);
        queue<pair<int,int>> q; // (index, steps)
        q.push({0, 0});
        visited[0] = true;

        while (!q.empty()) {
            auto [pos, steps] = q.front();
            q.pop();

            int furthest = min(pos + nums[pos], n - 1);
            for (int next = pos + 1; next <= furthest; next++) {
                if (!visited[next]) {
                    if (next == n - 1) {
                        return steps + 1; // reached last index
                    }
                    visited[next] = true;
                    q.push({next, steps + 1});
                }
            }
        }

        return -1; // shouldn't happen (guaranteed reachable)
    }
};
```

- **Time Complexity**: O(n²) in the worst case.
- **Space Complexity**: O(n) for the queue and visited array.

---

## 7. Production-Ready Solution

```cpp
class Solution {
public:
    int jump(const vector<int>& nums) {
        int jumps = 0, currEnd = 0, currFarthest = 0;
        int n = nums.size();

        for (int i = 0; i < n - 1; ++i) {
            currFarthest = max(currFarthest, i + nums[i]);
            if (i == currEnd) {
                jumps++;
                currEnd = currFarthest;
            }
        }

        return jumps;
    }
};
```

---

✅ Summary:

- Your DFS solution is correct but exponential → useful as a “first attempt”.
- In interviews, recognize this is a **greedy problem** → O(n), O(1).
- Be ready to explain recursion invariant (steps always valid, minSteps always updated optimally).

好问题 ✅！
**Jump Game II**（最少跳跃次数）确实有一个 **DP 解法**，而且和 DFS + 记忆化其实是一回事，只不过 DP 是 **自底向上** 写的。

---

## 1. 思路（状态定义）

- 定义 `dp[i]` = **到达位置 i 的最少跳跃次数**。
- 显然 `dp[0] = 0`（起点不需要跳）。

---

## 2. 状态转移

要到达 `i`，你必须从某个位置 `j` 跳过来，前提是 `j + nums[j] >= i`：

$$
dp[i] = \min_{0 \le j < i \ \wedge \ j + nums[j] \ge i} \{ dp[j] + 1 \}
$$

---

## 3. DP 实现（O(n²)）

```cpp
class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, INT_MAX);
        dp[0] = 0;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (j + nums[j] >= i && dp[j] != INT_MAX) {
                    dp[i] = min(dp[i], dp[j] + 1);
                }
            }
        }

        return dp[n - 1];
    }
};
```

---

## 4. 复杂度

- **时间复杂度**: O(n²) （两层循环）
- **空间复杂度**: O(n)

这比 DFS 好很多，但仍然比不上贪心 O(n)。

---

## 5. 和 DFS + 记忆化的关系

- **DFS + memo**：自顶向下，每次 dfs(i) 代表“从 i 到终点的最少跳数”，递归调用需要子问题结果。
- **DP**：自底向上，每次 dp\[i] 代表“到达 i 的最少跳数”，循环构造结果。

两者其实是同一个递推公式，只是方向相反。

---

## 6. 小结

- **DFS**: 指数级（暴力）。
- **DFS + memo**: O(n²)。
- **DP**: O(n²)。
- **BFS**: O(n²)（按层搜索）。
- **Greedy**: O(n) 最优。
