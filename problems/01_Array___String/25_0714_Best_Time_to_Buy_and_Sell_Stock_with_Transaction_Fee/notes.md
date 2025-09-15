# Best Time to Buy and Sell Stock with Transaction Fee

- **ID:** 714
- **Difficulty:** Medium
- **Topic Tags:** Array, Dynamic Programming, Greedy
- **Link:** [LeetCode Problem](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/description/)

## Problem Analysis

At each step, we have two choices: to hold a stock or not to hold a stock. It is different from the previous problems because we have a transaction fee when we sell a stock. So we need to consider to make the number of transactions as small as possible while maximizing the profit.

So we can use a recursive approach to solve this problem. Or we can use dynamic programming to optimize the recursive approach.

## Approach

My solution:

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();
        vector<vector<int>> dp(n + 1, vector<int>(2, 0));
        dp[0][1] = -prices[0] - fee;

        for (int i = 1; i < n; ++i) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][1] + prices[i]);
            dp[i][1] = max(dp[i - 1][1], dp[i - 1][0] - prices[i] - fee);
        }

        return dp[n - 1][0];
    }
};
```

Optimized space complexity:
We can see that the current state only depends on the previous state, so we can optimize the space complexity to O(1) by using two variables to store the previous state.

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int n = static_cast<int>(prices.size());

        // at the end of the first day we don't hold the stock
        int tempNotHold = 0;
        // at the end of the first day we keep the stock
        int tempHold = - fee - prices[0];

        for (int i = 1; i < prices.size(); ++i)
        {
            // before the i_th day, we have two scenarios
            // and at the end of the day we could have two directions as well
            tempNotHold = std::max(tempNotHold, tempHold + prices[i]);
            tempHold = std::max(tempHold, tempNotHold - prices[i] - fee);
        }

        return tempNotHold;
    }
};
```

## Alternative Approach (DFS + Memoization)

```cpp
class Solution {
public:
    int dfs(int i, bool isHolding, const vector<int>& prices, int fee, vector<vector<int>>& memo)
    {
        if(i == prices.size()) return 0;

        if(memo[i][isHolding] != -1) return memo[i][isHolding];

        // do nothing at this step
        int skip = dfs(i+1, isHolding, prices, fee, memo);

        int bestProfit = skip;
        if(isHolding)
        {
            // if isHolding is true, we try to sell it at this step
            bestProfit = std::max(bestProfit,
                                  prices[i] + dfs(i+1, false, prices, fee, memo));
        }
        else
        {
            // if isHolding is false, we try to buy stocks at this step
            bestProfit = std::max(bestProfit,
                                  -prices[i] - fee + dfs(i+1, true, prices, fee, memo));
        }

        // memoize the result
        memo[i][isHolding] = bestProfit;

        return bestProfit;
    }

    int maxProfit(vector<int>& prices, int fee) {
      vector<vector<int>> memo(prices.size(), vector<int>(2, -1));
        return dfs(0, false, prices, fee, memo);
    }
};
```

## Complexity

- **Time Complexity:** O(n)
- **Space Complexity:** O(n)
