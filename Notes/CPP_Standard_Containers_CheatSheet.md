# C++ Standard Containers Complete Guide (C++ 标准容器完整指南)

## 1. 序列容器 (Sequence Containers)

### std::vector - 动态数组 (Dynamic Array)

```cpp
#include <vector>

// 特点 (Features)
std::vector<int> vec = {1, 2, 3, 4, 5};

// ✅ 优点 (Pros):
// - 动态大小 (Dynamic size)
// - 随机访问 O(1) (Random access O(1))
// - 在末尾插入/删除很快 O(1) (Fast push_back/pop_back O(1))
// - 内存连续 (Contiguous memory) - 缓存友好

// ❌ 缺点 (Cons):
// - 在中间插入/删除慢 O(n) (Slow insert/erase in middle O(n))
// - 重新分配时可能拷贝所有元素 (May copy all elements when reallocating)

// 常用操作 (Common Operations)
vec.push_back(6);           // 末尾添加
vec.pop_back();             // 末尾删除
vec.insert(vec.begin(), 0); // 指定位置插入
vec.erase(vec.begin());     // 指定位置删除
vec.size();                 // 大小
vec.capacity();             // 容量
vec.reserve(100);           // 预分配空间
vec[2];                     // 随机访问，如果越界未定义行为
vec.at(2);                  // 安全访问（边界检查）如果越界抛异常

// Move 语义 (Move Semantics)
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = std::move(v1);  // ✅ 高效：只转移指针，O(1)
// v1 现在是空的，v2 获得了所有元素
```

### std::array - 固定大小数组 (Fixed-Size Array)

```cpp
#include <array>

// 特点 (Features) - 编译时确定大小
std::array<int, 5> arr = {1, 2, 3, 4, 5};

// ✅ 优点 (Pros):
// - 零开销抽象 (Zero overhead abstraction)
// - 编译时大小检查 (Compile-time size checking)
// - 没有动态分配 (No dynamic allocation)
// - 随机访问 O(1) (Random access O(1))
// - 栈上分配，速度极快 (Stack allocation, very fast)

// ❌ 缺点 (Cons):
// - 大小固定，不能改变 (Fixed size, cannot change)
// - 大对象可能栈溢出 (Large objects may cause stack overflow)

// 常用操作 (Common Operations)
arr[2] = 10;                // 随机访问
arr.at(2);                  // 安全访问
arr.size();                 // 大小（编译时常量）
arr.fill(0);                // 填充所有元素
arr.front();                // 第一个元素
arr.back();                 // 最后一个元素

// ❗ 为什么 Move 还是需要 Copy？
std::array<int, 1000> big_arr;
std::array<int, 1000> moved_arr = std::move(big_arr);  // ⚠️ 仍然需要拷贝所有元素！

// 原因解释：
// std::array 的数据是直接存储在对象内部的，不是通过指针指向堆内存
// 移动时没有"指针"可以转移，只能逐个元素拷贝
class ArrayInternals {
    int data[5];  // 数据直接存储在对象内部
    // 没有 int* data; 这样的指针可以转移
};
```

### std::list - 双向链表 (Doubly Linked List)

```cpp
#include <list>

// 特点 (Features)
std::list<int> lst = {1, 2, 3, 4, 5};

// ✅ 优点 (Pros):
// - 在任意位置插入/删除都很快 O(1) (Fast insert/erase anywhere O(1))
// - 不会使迭代器失效 (Iterators remain valid)
// - 双向遍历 (Bidirectional traversal)

// ❌ 缺点 (Cons):
// - 没有随机访问 (No random access)
// - 内存不连续，缓存不友好 (Non-contiguous memory, cache unfriendly)
// - 额外的指针开销 (Extra pointer overhead)

// 常用操作 (Common Operations)
lst.push_back(6);           // 末尾添加
lst.push_front(0);          // 开头添加
lst.pop_back();             // 末尾删除
lst.pop_front();            // 开头删除
lst.insert(lst.begin(), 10); // 指定位置插入
lst.erase(lst.begin());     // 指定位置删除
lst.sort();                 // 排序
lst.reverse();              // 反转

// Move 语义 (Move Semantics)
std::list<int> l1 = {1, 2, 3};
std::list<int> l2 = std::move(l1);  // ✅ 高效：只转移内部指针，O(1)
```

### std::deque - 双端队列 (Double-Ended Queue)

```cpp
#include <deque>

// 特点 (Features) - vector 和 list 的折中
std::deque<int> dq = {1, 2, 3, 4, 5};

// ✅ 优点 (Pros):
// - 两端插入/删除都很快 O(1) (Fast insert/erase at both ends O(1))
// - 随机访问 O(1) (Random access O(1))
// - 不会因为插入而使所有迭代器失效

// ❌ 缺点 (Cons):
// - 内存不完全连续 (Not fully contiguous memory)
// - 比 vector 稍慢 (Slightly slower than vector)

// 常用操作 (Common Operations)
dq.push_back(6);            // 末尾添加
dq.push_front(0);           // 开头添加
dq.pop_back();              // 末尾删除
dq.pop_front();             // 开头删除
dq[2];                      // 随机访问

// Move 语义 (Move Semantics)
std::deque<int> d1 = {1, 2, 3};
std::deque<int> d2 = std::move(d1);  // ✅ 高效：转移内部数据结构，O(1)
```

## 2. 关联容器 (Associative Containers)

### std::map - 有序映射 (Ordered Map)

```cpp
#include <map>

// 特点 (Features) - 基于红黑树，自动排序
std::map<std::string, int> scores = {
    {"Alice", 95},
    {"Bob", 87},
    {"Charlie", 92}
};

// ✅ 优点 (Pros):
// - 自动按键排序 (Automatically sorted by key)
// - 查找、插入、删除都是 O(log n)
// - 迭代器遍历是有序的 (Iteration is ordered)

// ❌ 缺点 (Cons):
// - 比哈希表慢 (Slower than hash table)
// - 内存开销较大 (Higher memory overhead)

// 常用操作 (Common Operations)
scores["David"] = 88;       // 插入或更新
scores.erase("Bob");        // 删除
auto it = scores.find("Alice");  // 查找
if (it != scores.end()) {
    std::cout << it->second;  // 输出值
}

// Move 语义 (Move Semantics)
std::map<std::string, int> m1 = {{"a", 1}, {"b", 2}};
std::map<std::string, int> m2 = std::move(m1);  // ✅ 高效：转移整个树结构，O(1)
```

### std::unordered_map - 哈希映射 (Hash Map)

```cpp
#include <unordered_map>

// 特点 (Features) - 基于哈希表
std::unordered_map<std::string, int> hash_scores = {
    {"Alice", 95},
    {"Bob", 87}
};

// ✅ 优点 (Pros):
// - 平均 O(1) 查找、插入、删除 (Average O(1) operations)
// - 最快的关联容器 (Fastest associative container)

// ❌ 缺点 (Cons):
// - 最坏情况 O(n) (Worst case O(n))
// - 迭代顺序不确定 (Iteration order undefined)
// - 需要好的哈希函数 (Requires good hash function)

// 常用操作 (Common Operations)
hash_scores["Charlie"] = 92;  // 插入
auto count = hash_scores.count("Alice");  // 检查是否存在
```

## 3. 容器适配器 (Container Adapters)

### std::stack - 栈 (Stack - LIFO)

```cpp
#include <stack>

// 特点 (Features) - 后进先出
std::stack<int> stk;

stk.push(1);               // 入栈
stk.push(2);               // 入栈
int top = stk.top();       // 查看栈顶（不删除）
stk.pop();                 // 出栈
bool empty = stk.empty();  // 检查是否为空
size_t size = stk.size();  // 大小

// 底层容器可以选择 (Can choose underlying container)
std::stack<int, std::vector<int>> vector_stack;
std::stack<int, std::deque<int>> deque_stack;   // 默认
std::stack<int, std::list<int>> list_stack;
```

### std::queue - 队列 (Queue - FIFO)

```cpp
#include <queue>

// 特点 (Features) - 先进先出
std::queue<int> q;

q.push(1);                 // 入队
q.push(2);                 // 入队
int front = q.front();     // 查看队首
int back = q.back();       // 查看队尾
q.pop();                   // 出队
```

### std::priority_queue - 优先队列 (Priority Queue - Heap)

```cpp
#include <queue>

// 特点 (Features) - 最大堆（默认）
std::priority_queue<int> pq;

pq.push(3);                // 插入
pq.push(1);                // 插入
pq.push(4);                // 插入
int max_val = pq.top();    // 查看最大值（4）
pq.pop();                  // 删除最大值

// 最小堆 (Min heap)
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
```

## 4. 实用工具 (Utility Types)

### std::pair - 二元组 (Pair)

```cpp
#include <utility>

// 特点 (Features) - 存储两个值
std::pair<std::string, int> student = {"Alice", 20};
std::pair<int, int> point = std::make_pair(10, 20);

// 访问元素 (Access elements)
std::cout << student.first;   // "Alice"
std::cout << student.second;  // 20

// C++17 结构化绑定 (Structured binding)
auto [name, age] = student;
std::cout << name << " is " << age << " years old\n";

// 比较 (Comparison) - 先比较 first，再比较 second
std::pair<int, int> p1 = {1, 2};
std::pair<int, int> p2 = {1, 3};
bool result = p1 < p2;  // true

// Move 语义 (Move Semantics)
std::pair<std::string, std::vector<int>> p = {"data", {1,2,3}};
auto moved_p = std::move(p);  // ✅ 移动两个成员
```

### std::tuple - 多元组 (Tuple)

```cpp
#include <tuple>

// 特点 (Features) - 存储多个不同类型的值
std::tuple<std::string, int, double> person = {"Bob", 25, 175.5};
auto person2 = std::make_tuple("Charlie", 30, 180.0);

// 访问元素 (Access elements)
std::cout << std::get<0>(person);  // "Bob"
std::cout << std::get<1>(person);  // 25
std::cout << std::get<2>(person);  // 175.5

// C++17 结构化绑定 (Structured binding)
auto [name, age, height] = person;

// 获取元素个数 (Get size)
constexpr size_t size = std::tuple_size_v<decltype(person)>;  // 3

// tie - 解包到现有变量 (Unpack to existing variables)
std::string n;
int a;
double h;
std::tie(n, a, h) = person;

// 忽略某些值 (Ignore some values)
std::tie(n, std::ignore, h) = person;  // 忽略年龄

// Move 语义 (Move Semantics)
std::tuple<std::string, std::vector<int>> data = {"info", {1,2,3}};
auto moved_data = std::move(data);  // ✅ 移动所有成员
```

## 5. 性能对比和选择指南 (Performance Comparison & Selection Guide)

### 容器选择决策树 (Container Selection Decision Tree)

```cpp
// 1. 需要随机访问？(Need random access?)
// ├─ 是 → 需要动态大小？(Need dynamic size?)
// │  ├─ 是 → std::vector (最常用)
// │  ├─ 否 → std::array (固定大小，最快)
// │  └─ 需要两端操作？(Need operations at both ends?) → std::deque
// └─ 否 → 需要频繁插入/删除？(Need frequent insert/delete?)
//    ├─ 是 → std::list
//    └─ 否 → std::vector (仍然推荐)

// 2. 需要键值对？(Need key-value pairs?)
// ├─ 需要排序？(Need sorted order?) → std::map
// └─ 需要最快查找？(Need fastest lookup?) → std::unordered_map

// 3. 需要特殊访问模式？(Need special access pattern?)
// ├─ 后进先出 (LIFO) → std::stack
// ├─ 先进先出 (FIFO) → std::queue
// └─ 优先级排序 (Priority) → std::priority_queue

// 4. 需要组合多个值？(Need to combine multiple values?)
// ├─ 两个值 → std::pair
// └─ 多个值 → std::tuple
```

### Move 语义效率对比 (Move Semantics Efficiency Comparison)

```cpp
// Move 效率排行榜 (Move Efficiency Ranking)

// 🥇 超快 (Ultra Fast) - O(1)，只转移指针
std::vector<int> v;
std::list<int> l;
std::map<int, int> m;
std::unordered_map<int, int> um;
std::string s;
// auto moved = std::move(container);  // 只转移内部指针

// 🥈 中等 (Medium) - O(1)，但转移多个成员
std::pair<std::string, std::vector<int>> p;
std::tuple<std::string, int, std::vector<int>> t;
// auto moved = std::move(utility);   // 转移每个成员

// 🥉 慢 (Slow) - O(n)，必须拷贝所有元素
std::array<int, 1000> arr;
// auto moved = std::move(arr);       // 拷贝所有元素！

// 为什么 std::array Move 慢？
// 因为数据存储方式不同：
class VectorLike {
    int* data;        // 指针 → Move 时转移指针（快）
    size_t size;
};

class ArrayLike {
    int data[1000];   // 直接存储 → Move 时必须逐个拷贝（慢）
};
```

### 内存布局对比 (Memory Layout Comparison)

```cpp
// 内存连续性排行榜 (Memory Contiguity Ranking)

// 🥇 完全连续 (Fully Contiguous) - 缓存最友好
std::vector<int> v;     // 所有元素在连续内存中
std::array<int, 10> a;  // 所有元素在连续内存中
std::string s;          // 字符连续存储

// 🥈 部分连续 (Partially Contiguous)
std::deque<int> d;      // 分块存储，块内连续

// 🥉 不连续 (Non-Contiguous) - 随机访问慢
std::list<int> l;       // 链表，节点分散在内存中
std::map<int, int> m;   // 树结构，节点分散
std::unordered_map<int, int> um; // 哈希表，桶分散

```

## 6. 实际使用建议 (Practical Usage Recommendations)

### 默认选择 (Default Choices)

```cpp
// 90% 的情况下，这些就够了：
std::vector<T>              // 序列容器首选
std::unordered_map<K, V>    // 关联容器首选
std::string                 // 字符串
std::pair<T1, T2>          // 简单组合
```

### 特殊场景选择 (Special Scenario Choices)

```cpp
// 固定大小，性能关键 → std::array
std::array<double, 4> matrix_row;

// 频繁头尾操作 → std::deque
std::deque<Task> task_queue;

// 需要保持插入顺序的映射 → 考虑 std::vector<std::pair>
std::vector<std::pair<std::string, int>> ordered_map;

// 需要排序的映射 → std::map
std::map<std::string, Student> student_records;

// 算法需要 → std::stack, std::queue, std::priority_queue
std::stack<int> dfs_stack;
std::queue<Node*> bfs_queue;
std::priority_queue<Task> task_scheduler;
```

### 性能优化技巧 (Performance Optimization Tips)

```cpp
// 1. 预分配空间 (Pre-allocate space)
std::vector<int> v;
v.reserve(1000);  // 避免多次重新分配

// 2. 使用 emplace 而不是 push (Use emplace instead of push)
std::vector<std::string> strings;
strings.emplace_back("hello");  // 直接构造，避免临时对象
// strings.push_back("hello");   // 创建临时对象再移动

// 3. 移动而不是拷贝 (Move instead of copy)
std::vector<std::string> source = {"a", "b", "c"};
std::vector<std::string> dest = std::move(source);  // 移动

// 4. 使用合适的容器 (Use appropriate container)
// 如果只需要查找，不需要排序：
std::unordered_set<int> fast_lookup;  // 而不是 std::set<int>
```

总结：每种容器都有其最适合的使用场景，选择合适的容器比优化算法更重要！记住 `std::array` 的 move 需要拷贝是因为它的数据直接存储在对象内部，而不是通过指针指向堆内存。
