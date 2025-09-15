#include <gtest/gtest.h>
// #include "Solution.h"
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
using namespace std;

struct Action {
    int day;
    string op; // "buy" or "sell"
};

int dfs(int i, bool holding, const vector<int>& prices, int fee,
        vector<vector<int>>& memo, vector<vector<vector<Action>>>& choicePath,
        vector<Action>& path) {
    if (i == prices.size()) return 0;

    if (memo[i][holding] != INT_MIN) {
        // 把缓存的路径加入
        for (auto &a : choicePath[i][holding]) {
            path.push_back(a);
        }
        return memo[i][holding];
    }

    // 跳过今天
    vector<Action> skipPath;
    int skipProfit = dfs(i+1, holding, prices, fee, memo, choicePath, skipPath);

    int bestProfit = skipProfit;
    vector<Action> bestPath = skipPath;

    if (holding) {
        // 卖出
        vector<Action> sellPath;
        int sellProfit = prices[i] - fee + dfs(i+1, false, prices, fee, memo, choicePath, sellPath);
        if (sellProfit > bestProfit) {
            bestProfit = sellProfit;
            bestPath = {{i, "sell"}};
            bestPath.insert(bestPath.end(), sellPath.begin(), sellPath.end());
        }
    } else {
        // 买入
        vector<Action> buyPath;
        int buyProfit = -prices[i] + dfs(i+1, true, prices, fee, memo, choicePath, buyPath);
        if (buyProfit > bestProfit) {
            bestProfit = buyProfit;
            bestPath = {{i, "buy"}};
            bestPath.insert(bestPath.end(), buyPath.begin(), buyPath.end());
        }
    }

    // 缓存结果和路径
    memo[i][holding] = bestProfit;
    choicePath[i][holding] = bestPath;

    for (auto &a : bestPath) path.push_back(a);
    return bestProfit;
}

int maxProfitWithPath(vector<int>& prices, int fee, vector<Action>& resultPath) {
    int n = prices.size();
    vector<vector<int>> memo(n, vector<int>(2, INT_MIN));
    vector<vector<vector<Action>>> choicePath(n, vector<vector<Action>>(2));
    int result = dfs(0, false, prices, fee, memo, choicePath, resultPath);
    // print the memo table
    cout << "Memo Table:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Day " << i << ": Not Holding = " << memo[i][0] << ", Holding = " << memo[i][1] << "\n";
    }
    return result;
}


TEST(Problem122Test, BasicTest) {
    // Solution sol;
    // TODO: Add your basic tests here
    vector<int> prices = {1, 3, 2, 8, 4, 9};
    int fee = 2;

    vector<Action> path;
    int profit = maxProfitWithPath(prices, fee, path);

    cout << "Max Profit = " << profit << "\n";
    for (auto &a : path) {
        cout << "Day " << a.day << ": " << a.op << " (price=" << prices[a.day] << ")\n";
    }
}
