# Merge and Sort Intervals

https://www.hackerrank.com/contests/software-engineer-prep-kit/challenges/merge-and-sort-intervals/problem

## My solution which has some issues

```c++
vector<vector<int>> mergeHighDefinitionIntervals(vector<vector<int>> intervals) {
    vector<vector<int>> results{};
    if(intervals.empty()) {
        return results;
    }
    sort(intervals.begin(),intervals.end());
    int n = intervals.size();
    vector<int> candidate = intervals[0];
    for(int i = 1; i < n; ++i) {
        // overlapping happened
        if(candidate[1] >= intervals[i][0]) {
            candidate[1] = intervals[i][1];
        } else {
            results.push_back(candidate);
            candidate = intervals[i];
        }
    }
    results.push_back(candidate);

    return results;
}
```

## Notes

### 错误分析

**主要问题：** 在合并重叠区间时，错误地直接赋值 `candidate[1] = intervals[i][1]`

**错误原因：**

- 合并区间应该取两个区间的最大结束位置，而不是直接用后一个区间的结束位置
- 当前一个区间完全包含后一个区间时，会错误地缩小合并后的区间

**具体例子：**

- `candidate = [1, 6]`, `intervals[i] = [2, 4]`
- 错误结果：`[1, 4]` (缩小了)
- 正确结果：`[1, 6]` (应该保持更大的结束值)

**修复方法：** `candidate[1] = max(candidate[1], intervals[i][1]);`

### 如何系统性地找出测试用例

当 HackerRank 不显示测试用例时，需要自己设计全面的测试用例来验证代码：

#### 1. 边界情况 (Edge Cases)

```
- 空数组: []
- 单个区间: [[1,3]]
- 两个不重叠: [[1,2], [3,4]]
- 两个重叠: [[1,3], [2,4]]
```

#### 2. 重叠模式分析

```
A. 完全包含: [1,6] 包含 [2,4] → [1,6]
B. 部分重叠: [1,3] 和 [2,4] → [1,4]
C. 相邻区间: [1,2] 和 [2,3] → [1,3]
D. 完全分离: [1,2] 和 [4,5] → [1,2], [4,5]
```

#### 3. 特殊数值情况

```
- 相同区间: [[1,3], [1,3]]
- 单点区间: [[1,1], [2,2]]
- 负数: [[-2,-1], [0,1]]
- 大数值: [[1000000, 2000000]]
```

#### 4. 排序相关测试

```
- 已排序: [[1,2], [3,4], [5,6]]
- 逆序: [[5,6], [3,4], [1,2]]
- 乱序: [[3,4], [1,2], [5,6]]
```

#### 5. 复杂合并场景

```
- 连续合并: [[1,3], [2,5], [4,7]] → [1,7]
- 多次合并: [[1,4], [2,3], [5,7], [6,8]] → [1,4], [5,8]
```

### 调试技巧

1. **手动追踪**: 在纸上模拟算法执行过程
2. **添加打印**: 在关键步骤输出中间状态
3. **单步测试**: 先测试简单用例，逐步增加复杂度
4. **对比标准**: 找已知正确的解法对比结果

### 测试代码模板

```c++
void testMergeIntervals() {
    // 测试用例1: 基本重叠
    vector<vector<int>> test1 = {{1,3}, {2,6}, {8,10}, {15,18}};
    auto result1 = mergeHighDefinitionIntervals(test1);
    // 期望: {{1,6}, {8,10}, {15,18}}

    // 测试用例2: 完全包含
    vector<vector<int>> test2 = {{1,6}, {2,4}};
    auto result2 = mergeHighDefinitionIntervals(test2);
    // 期望: {{1,6}}

    // 添加更多测试用例...
}
```
