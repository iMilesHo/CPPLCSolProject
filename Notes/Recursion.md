# 📚 递归解题六步走

## 1. **状态是什么？**

- 递归函数要传什么参数？
- 这些参数是否能唯一描述「此时的局面」？
  👉 在股票题里，我们的状态就是 `(i, holding)` ——今天是第几天、手里有没有股票。

---

## 2. **终止条件（什么时候到底？）**

- 到哪一天就不能再继续递归？
- 通常是「走到数组末尾」或者「满足某个边界条件」。
  👉 在股票题里，就是 `i == prices.size()`，没有更多天可操作，收益为 0。

---

## 3. **可以走的方向（递归分支）**

- 在当前状态下，有哪些合法的选择？
- 每个选择会带来怎样的状态转移？
  👉 在股票题里：

  - 如果手里有股票 → 可以 **卖** 或 **跳过**
  - 如果手里没股票 → 可以 **买** 或 **跳过**

---

## 4. **递归返回值（子问题的含义）**

- 递归调用返回的是什么？
- 我们是要返回 true/false、还是要返回最大值、最小值、路径等？
  👉 在股票题里，返回的是「从第 i 天开始，在 holding 状态下，最多能赚多少钱」。

---

## 5. **合并子结果（递推关系）**

- 子问题结果回来以后，要怎么取最优？
- 常见是 `max` / `min` / `sum` / `count`。
  👉 在股票题里，就是 `max(skip, buy/sell)`。

---

## 6. **复杂度 & 优化**

- 当前递归会不会爆炸（指数级）？
- 能否用 **memoization**（记忆化）或 **DP 表** 降低复杂度？

---

### 总结口诀 🎯

递归四步走：

1. 定义状态（函数参数是什么）
2. 写清出口（什么时候停下来）
3. 列出选择（可以往哪递归）
4. 合并答案（返回值怎么决定）

---

好 👌，下面我给你整理一个通用的 **递归模板框架**，带详细注释，你可以套用到大部分题目里（DFS、回溯、搜索、动态规划）。

---

# 📝 通用递归模板

```cpp
// 定义递归函数：参数表示当前状态，返回值表示子问题的答案
ReturnType dfs(State parameters...) {
    // 1. 递归出口（base case）
    //    判断什么时候到头，比如数组走完、树节点为空、目标达成
    if (到头条件) {
        return 最终答案; // 例如 0 / true / false / 空集合
    }

    // 2. 检查缓存（可选，针对DP优化）
    //    如果这个状态已经算过，就直接返回
    if (memo.count(parameters)) {
        return memo[parameters];
    }

    // 3. 列出所有可能的选择（往下递归）
    //    每个选择会产生一个新的子状态
    ResultType res;
    for (每个可能的选择) {
        // 递归进入子问题
        auto sub = dfs(新的状态);

        // 4. 根据题目需要，合并子结果
        res = combine(res, sub); // 比如取 max/min/累加/拼接
    }

    // 5. 保存结果到缓存（可选）
    memo[parameters] = res;

    // 返回最终结果
    return res;
}
```

---

# 📌 举例：股票买卖（714 带手续费）

```cpp
int dfs(int i, bool holding, const vector<int>& prices, int fee,
        unordered_map<long long,int>& memo) {
    // 1. 递归出口
    if (i == prices.size()) return 0;

    // 2. 状态哈希 (i, holding)
    long long key = (long long)i << 1 | holding;
    if (memo.count(key)) return memo[key];

    // 3. 可选操作
    // 跳过今天
    int skip = dfs(i+1, holding, prices, fee, memo);

    int doSomething;
    if (holding) {
        // 卖出
        doSomething = prices[i] - fee + dfs(i+1, false, prices, fee, memo);
    } else {
        // 买入
        doSomething = -prices[i] + dfs(i+1, true, prices, fee, memo);
    }

    // 4. 合并答案（取最大收益）
    int res = max(skip, doSomething);

    // 5. 记忆化
    return memo[key] = res;
}
```

调用：

```cpp
int maxProfit(vector<int>& prices, int fee) {
    unordered_map<long long,int> memo;
    return dfs(0, false, prices, fee, memo);
}
```

---

# 🚀 总结口诀

- **状态**：递归函数要表达“我现在在哪儿”。
- **出口**：什么时候停。
- **选择**：这一层能往哪走。
- **合并**：怎么整合子结果。
- **优化**：记忆化 or DP。
