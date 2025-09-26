#include <gtest/gtest.h>

#include <vector>
using namespace std;

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
                int boundedHeight =
                    min(height[i], height[st.top()]) - height[top];
                water += distance * boundedHeight;
            }
            st.push(i);
        }
        return water;
    }
};

TEST(Problem42Test, BasicTest)
{
    Solution sol;

    // 测试用例1: [0,1,0,2,1,0,1,3,2,1,2,1]
    // 预期结果: 6
    vector<int> height1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    EXPECT_EQ(sol.trap(height1), 6);

    // 测试用例2: [4,2,0,3,2,5]
    // 预期结果: 9
    vector<int> height2 = {4, 2, 0, 3, 2, 5};
    EXPECT_EQ(sol.trap(height2), 9);

    // 边界测试：空数组
    vector<int> empty = {};
    EXPECT_EQ(sol.trap(empty), 0);

    // 边界测试：只有一个元素
    vector<int> single = {5};
    EXPECT_EQ(sol.trap(single), 0);

    // 边界测试：只有两个元素
    vector<int> two = {3, 1};
    EXPECT_EQ(sol.trap(two), 0);

    // 测试递增序列
    vector<int> increasing = {1, 2, 3, 4, 5};
    EXPECT_EQ(sol.trap(increasing), 0);

    // 测试递减序列
    vector<int> decreasing = {5, 4, 3, 2, 1};
    EXPECT_EQ(sol.trap(decreasing), 0);
}
