# Jump Game

- **ID:** 55
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Dynamic Programming, Greedy
- **Link:** [LeetCode Problem](https://leetcode.com/problems/jump-game/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

```cpp
class Solution {
public:
    int dfs(size_t i, const vector<int>& nums, vector<int>& memo)
    {
        if (memo[i] == 1)
        {
            return nums.size() - 1;
        }

        if (i == nums.size() - 1)
        {
            return i;
        }
        else if (i >= nums.size() || nums[i] == 0)
        {
            return i;
        }

        for (size_t k = 1; k <= nums[i]; ++k)
        {
            if (dfs(i + k, nums, memo) == nums.size() - 1)
            {
                memo[i] = 1;
                memo[i+k] = 1;
                return nums.size() - 1;
            }
        }

        return nums.size();
    }
    bool canJump(vector<int>& nums) {
        vector<int> memo(nums.size(), -1);

        if (dfs(0, nums, memo) == nums.size() - 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
```

Improved the readability:

```cpp
class Solution {
public:
    // States for memoization
    enum State { UNKNOWN = -1, BAD = 0, GOOD = 1 };

    bool dfs(int i, const vector<int>& nums, vector<int>& memo) {
        // Base case: last index is always GOOD
        if (i == nums.size() - 1) return true;

        // Return cached result if already computed
        if (memo[i] != UNKNOWN) return memo[i] == GOOD;

        int furthestJump = min(i + nums[i], (int)nums.size() - 1);

        // Try all jumps from 1 to nums[i]
        for (int next = i + 1; next <= furthestJump; ++next) {
            if (dfs(next, nums, memo)) {
                memo[i] = GOOD;
                return true;
            }
        }

        memo[i] = BAD; // No valid path found
        return false;
    }

    bool canJump(vector<int>& nums) {
        vector<int> memo(nums.size(), UNKNOWN);
        return dfs(0, nums, memo);
    }
};
```

## Complexity

- **Time Complexity:** O(n^2) in the worst case for the provided DFS solution with memoization.
- **Space Complexity:** O(n) for the memo array + recursion stack depth up to O(n).

## isn't the time complexity of my solution be like (n^n)?

### 1. **Naïve DFS without memoization**

- At each index `i`, you can branch into up to `nums[i]` recursive calls.
- In the worst case (e.g., `nums = [n, n, n, …]`), branching factor ≈ `n`.
- Depth of recursion ≤ `n`.
- So the upper bound is roughly `O(n^n)` (exponential blow-up).

👉 That’s why a pure DFS/Backtracking solution without memoization quickly times out.

---

### 2. **Your DFS with memoization**

- You keep a `memo` vector. Once you’ve proven that `dfs(i)` can reach the end, you cache it (`memo[i] = 1`).
- That means each index `i` will only be **fully explored once**.
- In each exploration, you try up to `nums[i]` jumps.
- So the true upper bound is about

$$
O\!\left(\sum_{i=0}^{n-1} \text{nums}[i]\right) \;\leq\; O(n^2)
$$

since `nums[i]` can be at most `n` (but problem constraints allow up to 10^5, capped by array length).

👉 Therefore, with memoization your algorithm is **polynomial, not exponential** — specifically **O(n²)** in the worst case.

---

### 3. **Greedy solution**

- Scans once, only updates a single `maxReach` value.
- Time complexity **O(n)**, space **O(1)**.
- That’s why it’s the standard interview solution.

---

✅ **Answer to your question:**

- Without memoization: \~O(n^n) (exponential).
- With your memoization: **O(n²)**.
- Optimal greedy: **O(n)**.

## 1 · Complexity, Readability, and Documentation

### Complexity

- **Time complexity (your code):**
  Worst case ≈ `O(n^2)` because for each index `i`, you may explore up to `nums[i]` recursive calls. With memoization, it avoids re-computation, but the branching factor can still be large before pruning.
  For `n=10^4`, this is too slow in practice.
- **Space complexity:**
  `O(n)` for the memo array + recursion stack depth up to `O(n)`.

👉 The optimal greedy solution runs in **O(n) time, O(1) space** (we’ll cover later).

### Readability

- `dfs` returns indices like `nums.size() - 1` or `nums.size()`, which is confusing.
  Better: return a `bool` (can reach end from here?).
- `memo[i]` values `-1`, `1`, etc. are not clearly documented.
  Suggest using an enum or constants for readability.

### Variable naming

- `dfs`: should be `canReachFrom` or `explore` → improves clarity.
- `k`: could be `step`.
- `memo`: could be `reachable`.

---

## 2 · Loop / Recursion Invariant

For recursive DFS:

- **Invariant:** At each index `i`, the algorithm tries all jumps within `nums[i]` and checks if any path leads to the last index.
- **Why it holds:**
  Each recursive call only explores forward indices, so no cycles. If any path returns success, it propagates back.
- **How it helps:**
  The invariant guarantees correctness: if `dfs(i)` is true, then there exists at least one path from `i` to the last index.

For the **optimal greedy** solution:

- **Invariant:** Keep track of the farthest reachable index `maxReach` as you iterate left-to-right.

  - At index `i`, we must have `i ≤ maxReach` (otherwise we’re stuck).
  - Update `maxReach = max(maxReach, i + nums[i])`.

- **Proof:**
  By induction, all indices ≤ `maxReach` are reachable. If we finish scanning and `maxReach ≥ last index`, then the last index is reachable.

---

## 3 · Code Style & C++ Idioms

- Use `bool` return types instead of encoding success as `size-1`.
- Pass `nums` by reference to avoid copying, mark as `const` where possible.
- Avoid raw `size_t` arithmetic in recursion (risk of underflow if subtraction happens). Use `int` for indices since constraints are ≤ 10^4.
- Prefer early returns for clarity.
- Add comments at function level.

---

## 4 · Common Follow-Ups

- **Greedy solution (O(n)):**
  The interviewer will almost certainly expect this optimization.
- **Variants:**

  - _Jump Game II_ (minimum jumps, LC 45).
  - _Jump Game III_ (can you reach a zero, BFS/DFS).
  - _Jump Game with Costs_ (DP).
  - Weighted graphs generalization.

- **Edge cases to discuss:**

  - Array of length 1 (`[0]` should return true).
  - Leading zero (`[0, …]`).
  - Very large `nums[i]`.

---

## 5 · Elevator Pitch (30–90s)

> “We’re given an array where each position tells us how far we can jump. The brute-force way is to explore all possible paths, but that’s too slow. A more efficient greedy approach is to track the farthest index we can reach as we scan the array. At each step, if the current index is reachable, we extend our maximum reach. If at any point we’re beyond our max reach, we’re stuck. If by the end our max reach covers the last index, we can reach it. This algorithm runs in linear time and constant space, making it efficient even for the largest inputs.”

---

## 6 · Optimizations & Alternative Perspectives

- DFS with memoization is educational but not practical here.
- Greedy scan is both simpler and faster.
- From a graph perspective: treat each index as a node with edges to reachable indices → problem reduces to “is there a path to the last node?”. The greedy method is essentially BFS optimized.

---

## 7 · Production-Ready C++ Solution

```cpp
class Solution {
public:
    bool canJump(const vector<int>& nums) {
        int maxReach = 0;
        int n = nums.size();

        for (int i = 0; i < n; ++i) {
            if (i > maxReach) return false; // stuck
            maxReach = max(maxReach, i + nums[i]);
            if (maxReach >= n - 1) return true; // can reach end
        }
        return true;
    }
};
```

- **Time:** O(n)
- **Space:** O(1)
- Short, clean, efficient. Perfect for interviews and production.

✅ **Suggested practice after this problem:**

1. [Jump Game II](https://leetcode.com/problems/jump-game-ii/) (minimum jumps).
2. [Jump Game III](https://leetcode.com/problems/jump-game-iii/) (reach value zero).
3. [Minimum Number of Refueling Stops](https://leetcode.com/problems/minimum-number-of-refueling-stops/) (greedy with heaps).
4. Try explaining invariants & greedy proofs out loud.

太好了，这题确实和“买股票”那类**最值型 DP（max/min）**很不一样；它更像**可达性（reachability）DP**。下面我按“从 0 到 1 写出 DP”的顺序，把思路一步步搭起来，并顺便指出和股票 DP 的差异。

---

# 一步步推导：从暴力 → 记忆化 → 底向上 DP → 贪心

## 0）先给问题“归类”

- 不是要“赚最多/最少”这种最值问题；
- 也不是“有几种方案”这种计数问题；
- 问的是**能不能到达最后一个下标**（是/否）。
  ➡️ 这就是**可达性问题**：常用 `bool` DP，转移里是逻辑 **OR**（“存在一个可行就行”），而不是 `max`/`min`。

---

## 1）自顶向下（你写的 DFS）——定义子问题

最自然的子问题：

- `canReach(i)`: 从下标 `i` 出发能否到达最后一个下标？

递归关系：

- 从 `i` 能跳到 `[i+1, i+nums[i]]` 的任意位置，只要存在一个 `next` 使得 `canReach(next) == true`，那 `canReach(i) == true`。
- 终止：`canReach(n-1) = true`；越界/卡在 0 的位置返回 `false`。

这就是你 DFS 的语义。由于不同路径会多次访问同一个 `i`，**存在重叠子问题**，所以加 **记忆化** 就得到**自顶向下 DP**。

> 这一步你已经完成了；难点在于它最坏要试很多跳（`O(n^2)`），面试里通常会让你继续优化。

---

## 2）自底向上 DP（两种等价写法）

可达性 DP 常见有两种“等价”的状态设计，选一个即可：

### 写法 A：`dp[i]` = “**起点能否到达 i**”

- **含义**：能否从 0 走到 i。
- **初始化**：`dp[0] = true`。
- **转移**：

  ```
  dp[i] = OR over j in [0..i-1] of ( dp[j] && j + nums[j] >= i )
  ```

- **答案**：`dp[n-1]`。
- **复杂度**：双层循环，最坏 `O(n^2)`。

### 写法 B：`dp[i]` = “**从 i 能否到达终点**”

- **含义**：能否从 i 走到 n-1。
- **初始化**：`dp[n-1] = true`。
- **转移（从右往左）**：

  ```
  dp[i] = OR over step in [1..nums[i]] of dp[i + step]
  ```

- **答案**：`dp[0]`。
- **复杂度**：同样最坏 `O(n^2)`。

> 写法 A 和 B 本质一样，只是方向相反。A 更贴近“前缀可达”，B 更贴近你的 DFS 语义。

---

## 3）从 DP 观察到“单调信息”，压缩到 O(1) 空间 / O(n) 时间（贪心）

观察写法 A 的内层循环：
我们在问——**已可达的所有 j 中，`j + nums[j]` 的最大值是多少？**
如果我们一路维护一个“**能到达的最远下标**” `maxReach`，那么检查 `i` 是否可达只需判断 `i <= maxReach`，并在可达时把 `maxReach = max(maxReach, i + nums[i])`。

这就把 `O(n^2)` 的 DP **退化为线性贪心**（其实是“DP 的等价单调优化”）：

- **循环不变式**：遍历到 `i` 时，`maxReach` 是从 `[0..i]` 这些可达点能达到的最远下标；若 `i > maxReach`，说明被卡住了，直接返回 `false`。
- **结束条件**：若某时刻 `maxReach >= n-1`，可以提前返回 `true`；否则扫完整个数组返回 `true`（长度为 1 等边界）或在中途返回 `false`。

---

# 和“买股票”DP 的不同点

| 维度     | Jump Game              | 买股票（如 121/122/含交易费）            |
| -------- | ---------------------- | ---------------------------------------- |
| 目标     | **可达性**（能不能到） | **最值**（最大收益）                     |
| DP 值域  | 布尔（`true/false`）   | 数值（`int/long long`）                  |
| 转移操作 | 逻辑 OR（存在即为真）  | `max/min`（取最好/最差）                 |
| 优化     | 单调性 → 贪心 O(n)     | 多为**状态机**DP；有的也能贪心（如 122） |
| 直觉模型 | 图的可达 / BFS on DAG  | 有限状态机的最优控制                     |

> 所以虽然都叫 DP，但“本质问题类型”不同，套路也不同：Jump Game 是“是否存在路径”；股票是“怎样操作最优”。

---

# 代码示例

## 底向上 DP（写法 A，`O(n^2)`）

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        vector<bool> dp(n, false);
        dp[0] = true;

        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (dp[j] && j + nums[j] >= i) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n - 1];
    }
};
```

## 底向上 DP（写法 B，`O(n^2)`，更贴近你的 DFS 含义）

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        vector<bool> dp(n, false);
        dp[n - 1] = true;

        for (int i = n - 2; i >= 0; --i) {
            int furthest = min(n - 1, i + nums[i]);
            for (int next = i + 1; next <= furthest; ++next) {
                if (dp[next]) { dp[i] = true; break; }
            }
        }
        return dp[0];
    }
};
```

## 线性“DP → 贪心”的等价实现（`O(n)`）

```cpp
class Solution {
public:
    bool canJump(const vector<int>& nums) {
        int maxReach = 0;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (i > maxReach) return false;            // 不可达，卡住
            maxReach = max(maxReach, i + nums[i]);     // 扩展最远可达
            if (maxReach >= n - 1) return true;        // 已经够到末尾
        }
        return true; // n==1 等边界
    }
};
```

---

# “DP 模板”在这题上的套用（简明版）

1. **问题类型**：可达性（是/否）
2. **状态定义**：`dp[i]` 表示“从 0 是否可达 i”（或“从 i 是否可达末尾”）
3. **初始化**：`dp[0]=true`（或 `dp[n-1]=true`）
4. **转移**：

   - A：`dp[i] = OR_{j<i} ( dp[j] && j+nums[j] >= i )`
   - B：`dp[i] = OR_{1<=k<=nums[i]} dp[i+k]`

5. **答案**：`dp[n-1]`（或 `dp[0]`）
6. **优化**：观察到“存在性 + 单调边界” → 只需维护最远可达 `maxReach` → 线性贪心

---

# 小结与面试话术

- 这题的“DP 味道”和股票不一样，因为它是**可达性** + **存在性**判断，用 **OR** 连接而不是 `max`。
- 从 DFS 的“子问题是下标”出发，你自然会写出记忆化；再改成底向上布尔 DP（`O(n^2)`）；最后抓住“最远可达位置”这个**单调信息**，把 DP 压成 `O(n)` 的贪心。
- 面试里把这条“DFS → DP → 贪心”的演进讲清楚，就很加分。

如果你想，我可以把这三种方案**并排**整理成一页速查表（含复杂度、循环不变式、边界讨论），方便你临门前背诵。
