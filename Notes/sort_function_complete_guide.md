# C++ sort() 函数完整用法指南

## 基本语法

```cpp
#include <algorithm>
#include <vector>
#include <functional>

// 基本形式
sort(first, last);
sort(first, last, comparator);
```

## 1. 基础用法（默认升序）

```cpp
vector<int> nums = {5, 2, 8, 1, 9};
sort(nums.begin(), nums.end());
// 结果: [1, 2, 5, 8, 9]
```

## 2. 第三个参数：比较器 (Comparator)

### 2.1 内置比较器

```cpp
#include <functional>

// 升序排序（默认）
sort(nums.begin(), nums.end());
sort(nums.begin(), nums.end(), less<int>());

// 降序排序
sort(nums.begin(), nums.end(), greater<int>());
```

### 2.2 自定义比较函数

```cpp
// 方法1: 普通函数
bool compare(int a, int b) {
    return a > b;  // 降序：如果 a > b 返回 true，则 a 排在 b 前面
}
sort(nums.begin(), nums.end(), compare);

// 方法2: Lambda 表达式（推荐）
sort(nums.begin(), nums.end(), [](int a, int b) {
    return a > b;  // 降序
});

// 方法3: 函数对象（仿函数）
struct Compare {
    bool operator()(int a, int b) {
        return a > b;
    }
};
sort(nums.begin(), nums.end(), Compare());
```

## 3. 常用比较器示例

### 3.1 基本数据类型

```cpp
vector<int> nums = {5, 2, 8, 1, 9};

// 升序
sort(nums.begin(), nums.end());
sort(nums.begin(), nums.end(), less<int>());

// 降序
sort(nums.begin(), nums.end(), greater<int>());

// 按绝对值排序
sort(nums.begin(), nums.end(), [](int a, int b) {
    return abs(a) < abs(b);
});
```

### 3.2 字符串排序

```cpp
vector<string> words = {"apple", "pie", "a", "longer"};

// 按字典序
sort(words.begin(), words.end());

// 按长度排序
sort(words.begin(), words.end(), [](const string& a, const string& b) {
    return a.length() < b.length();
});

// 按长度排序，长度相同时按字典序
sort(words.begin(), words.end(), [](const string& a, const string& b) {
    if (a.length() != b.length()) {
        return a.length() < b.length();
    }
    return a < b;
});
```

### 3.3 自定义结构体排序

```cpp
struct Student {
    string name;
    int age;
    double score;
};

vector<Student> students = {
    {"Alice", 20, 85.5},
    {"Bob", 19, 92.0},
    {"Charlie", 21, 78.5}
};

// 按年龄排序
sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    return a.age < b.age;
});

// 按分数降序排序
sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    return a.score > b.score;
});

// 多条件排序：先按分数降序，分数相同按年龄升序
sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    if (a.score != b.score) {
        return a.score > b.score;  // 分数降序
    }
    return a.age < b.age;  // 年龄升序
});
```

### 3.4 pair 和 tuple 排序

```cpp
vector<pair<int, int>> points = {{3, 4}, {1, 2}, {3, 1}, {2, 5}};

// 默认：先按 first 排序，再按 second 排序
sort(points.begin(), points.end());

// 按 second 排序
sort(points.begin(), points.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.second < b.second;
});

// 按距离原点的距离排序
sort(points.begin(), points.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.first * a.first + a.second * a.second <
           b.first * b.first + b.second * b.second;
});
```

## 4. 重要概念理解

### 4.1 比较器的返回值含义

```cpp
// 比较器 comp(a, b) 的返回值：
// - 返回 true：a 应该排在 b 前面
// - 返回 false：a 不应该排在 b 前面（b 排在 a 前面或相等）

// 升序：a < b 时返回 true
sort(nums.begin(), nums.end(), [](int a, int b) {
    return a < b;  // 小的排前面
});

// 降序：a > b 时返回 true
sort(nums.begin(), nums.end(), [](int a, int b) {
    return a > b;  // 大的排前面
});
```

### 4.2 稳定性

```cpp
// sort() 不保证稳定性（相等元素的相对位置可能改变）
// 如果需要稳定排序，使用 stable_sort()
stable_sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    return a.score > b.score;
});
```

## 5. 常见错误与注意事项

### 5.1 错误的比较器

```cpp
// ❌ 错误：不满足严格弱序关系
sort(nums.begin(), nums.end(), [](int a, int b) {
    return a <= b;  // 应该用 < 而不是 <=
});

// ✅ 正确
sort(nums.begin(), nums.end(), [](int a, int b) {
    return a < b;
});
```

### 5.2 引用传参优化

```cpp
// ❌ 对于大对象，按值传递效率低
sort(students.begin(), students.end(), [](Student a, Student b) {
    return a.score > b.score;
});

// ✅ 使用 const 引用
sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    return a.score > b.score;
});
```

## 6. 实际应用场景

### 6.1 LeetCode 常见模式

```cpp
// 1. 数组降序排序
vector<int> citations = {3, 0, 6, 1, 5};
sort(citations.begin(), citations.end(), greater<int>());
// 结果: [6, 5, 3, 1, 0]

// 2. 按绝对值排序
vector<int> nums = {-4, -1, 0, 3, 10};
sort(nums.begin(), nums.end(), [](int a, int b) {
    return abs(a) < abs(b);
});

// 3. 区间排序（按开始时间）
vector<vector<int>> intervals = {{1,3}, {2,6}, {8,10}};
sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];
});

// 4. 字符串按长度排序
vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
    return a.length() < b.length();
});
```

### 6.2 自定义排序优先级

```cpp
// 多条件排序模板
sort(data.begin(), data.end(), [](const DataType& a, const DataType& b) {
    // 第一优先级
    if (a.field1 != b.field1) {
        return a.field1 < b.field1;  // 或 >，取决于需要升序还是降序
    }
    // 第二优先级
    if (a.field2 != b.field2) {
        return a.field2 > b.field2;
    }
    // 第三优先级
    return a.field3 < b.field3;
});
```

## 7. 性能考虑

```cpp
// 时间复杂度: O(n log n)
// 空间复杂度: O(log n) - 递归栈空间

// 对于部分有序的数据，考虑使用：
// - insertion_sort (小数据量)
// - partial_sort (只需要前 k 个元素)
// - nth_element (只需要第 k 个元素)

// 只需要前 k 个最小元素
partial_sort(nums.begin(), nums.begin() + k, nums.end());

// 只需要第 k 个元素的位置
nth_element(nums.begin(), nums.begin() + k, nums.end());
```

## 8. 快速参考

| 需求           | 代码                                                                                                          |
| -------------- | ------------------------------------------------------------------------------------------------------------- |
| 升序           | `sort(v.begin(), v.end())`                                                                                    |
| 降序           | `sort(v.begin(), v.end(), greater<int>())`                                                                    |
| 按绝对值       | `sort(v.begin(), v.end(), [](int a, int b){ return abs(a) < abs(b); })`                                       |
| 字符串按长度   | `sort(v.begin(), v.end(), [](const string& a, const string& b){ return a.length() < b.length(); })`           |
| pair 按 second | `sort(v.begin(), v.end(), [](const pair<int,int>& a, const pair<int,int>& b){ return a.second < b.second; })` |

## 记忆口诀

**比较器返回 true 的含义：第一个参数应该排在第二个参数前面**

- 升序：`a < b` → 小的在前
- 降序：`a > b` → 大的在前
- 自定义：根据需求定义什么"在前"
