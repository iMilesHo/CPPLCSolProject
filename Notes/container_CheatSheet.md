# C++ STL Containers & String 构造方法 + 成员函数 CheatSheet

本文档包含：

1. 各种容器和 string 的构造方法
2. 常用成员函数接口
3. STL 算法函数

---

## 一、容器构造方法 CheatSheet

### 1. `std::string` 构造方法

| 构造方式     | 语法                                     | 说明               | 示例                                 |
| ------------ | ---------------------------------------- | ------------------ | ------------------------------------ |
| 默认构造     | `string s;`                              | 空字符串           | `string s;` → `""`                   |
| C 字符串构造 | `string s(const char* str);`             | 从 C 字符串构造    | `string s("hello");` → `"hello"`     |
| 拷贝构造     | `string s(const string& str);`           | 复制另一个 string  | `string s1("hi"); string s2(s1);`    |
| 子串构造     | `string s(const string& str, pos, len);` | 从指定位置复制子串 | `string s("hello", 1, 3);` → `"ell"` |
| 重复字符     | `string s(n, char);`                     | n 个相同字符       | `string s(5, 'a');` → `"aaaaa"`      |
| 范围构造     | `string s(first, last);`                 | 从迭代器范围构造   | `string s(vec.begin(), vec.end());`  |
| 初始化列表   | `string s{'h','i'};`                     | C++11 列表初始化   | `string s{'h','e','l','l','o'};`     |

```cpp
// 示例代码
string s1;                           // ""
string s2("Hello");                  // "Hello"
string s3(s2);                       // "Hello"
string s4(s2, 1, 3);                 // "ell"
string s5(5, 'x');                   // "xxxxx"
vector<char> v{'w','o','r','l','d'};
string s6(v.begin(), v.end());       // "world"
string s7{'C','+','+'};              // "C++"
```

---

### 2. `std::vector` 构造方法

| 构造方式    | 语法                        | 说明                 | 示例                                   |
| ----------- | --------------------------- | -------------------- | -------------------------------------- |
| 默认构造    | `vector<T> v;`              | 空 vector            | `vector<int> v;` → `{}`                |
| 指定大小    | `vector<T> v(n);`           | n 个默认值元素       | `vector<int> v(5);` → `{0,0,0,0,0}`    |
| 指定大小+值 | `vector<T> v(n, val);`      | n 个值为 val 的元素  | `vector<int> v(5, 3);` → `{3,3,3,3,3}` |
| 拷贝构造    | `vector<T> v(v2);`          | 复制另一个 vector    | `vector<int> v2(v1);`                  |
| 范围构造    | `vector<T> v(first, last);` | 从迭代器范围构造     | `vector<int> v(arr, arr+5);`           |
| 初始化列表  | `vector<T> v{...};`         | C++11 列表初始化     | `vector<int> v{1,2,3,4,5};`            |
| assign 方法 | `v.assign(n, val);`         | 重新分配为 n 个 val  | `v.assign(3, 7);` → `{7,7,7}`          |
| assign 范围 | `v.assign(first, last);`    | 从迭代器范围重新分配 | `v.assign(arr, arr+3);`                |

```cpp
// 示例代码
vector<int> v1;                      // {}
vector<int> v2(5);                   // {0,0,0,0,0}
vector<int> v3(5, 10);               // {10,10,10,10,10}
vector<int> v4(v3);                  // {10,10,10,10,10}
int arr[] = {1,2,3};
vector<int> v5(arr, arr+3);          // {1,2,3}
vector<int> v6{1,2,3,4,5};           // {1,2,3,4,5}
vector<int> v7;
v7.assign(4, 9);                     // {9,9,9,9}

// 二维 vector 构造
vector<vector<int>> matrix(3, vector<int>(4, 0));  // 3×4 矩阵，全为 0
```

---

### 3. `std::deque` 构造方法

| 构造方式    | 语法                        | 说明                | 示例                                 |
| ----------- | --------------------------- | ------------------- | ------------------------------------ |
| 默认构造    | `deque<T> dq;`              | 空 deque            | `deque<int> dq;`                     |
| 指定大小    | `deque<T> dq(n);`           | n 个默认值元素      | `deque<int> dq(5);`                  |
| 指定大小+值 | `deque<T> dq(n, val);`      | n 个值为 val 的元素 | `deque<int> dq(5, 2);`               |
| 拷贝构造    | `deque<T> dq(dq2);`         | 复制另一个 deque    | `deque<int> dq2(dq1);`               |
| 范围构造    | `deque<T> dq(first, last);` | 从迭代器范围构造    | `deque<int> dq(v.begin(), v.end());` |
| 初始化列表  | `deque<T> dq{...};`         | C++11 列表初始化    | `deque<int> dq{1,2,3};`              |

```cpp
// 示例代码
deque<int> dq1;                      // {}
deque<int> dq2(5);                   // {0,0,0,0,0}
deque<int> dq3(3, 7);                // {7,7,7}
deque<int> dq4{1,2,3,4};             // {1,2,3,4}
```

---

### 4. `std::list` (双向链表) 构造方法

| 构造方式    | 语法                        | 说明                | 示例                                 |
| ----------- | --------------------------- | ------------------- | ------------------------------------ |
| 默认构造    | `list<T> lst;`              | 空 list             | `list<int> lst;`                     |
| 指定大小    | `list<T> lst(n);`           | n 个默认值元素      | `list<int> lst(5);`                  |
| 指定大小+值 | `list<T> lst(n, val);`      | n 个值为 val 的元素 | `list<int> lst(5, 1);`               |
| 拷贝构造    | `list<T> lst(lst2);`        | 复制另一个 list     | `list<int> lst2(lst1);`              |
| 范围构造    | `list<T> lst(first, last);` | 从迭代器范围构造    | `list<int> lst(v.begin(), v.end());` |
| 初始化列表  | `list<T> lst{...};`         | C++11 列表初始化    | `list<int> lst{1,2,3};`              |

```cpp
// 示例代码
list<int> lst1;                      // {}
list<int> lst2(4);                   // {0,0,0,0}
list<int> lst3(3, 5);                // {5,5,5}
list<int> lst4{10,20,30};            // {10,20,30}
```

---

### 5. `std::set` / `std::multiset` 构造方法

| 构造方式     | 语法                     | 说明               | 示例                                   |
| ------------ | ------------------------ | ------------------ | -------------------------------------- |
| 默认构造     | `set<T> s;`              | 空 set             | `set<int> s;`                          |
| 范围构造     | `set<T> s(first, last);` | 从迭代器范围构造   | `set<int> s(v.begin(), v.end());`      |
| 拷贝构造     | `set<T> s(s2);`          | 复制另一个 set     | `set<int> s2(s1);`                     |
| 初始化列表   | `set<T> s{...};`         | C++11 列表初始化   | `set<int> s{3,1,4,1,5};` → `{1,3,4,5}` |
| 自定义比较器 | `set<T, Comp> s;`        | 使用自定义比较函数 | `set<int, greater<int>> s;`            |

```cpp
// 示例代码
set<int> s1;                         // {}
vector<int> v{4,2,3,2,1};
set<int> s2(v.begin(), v.end());     // {1,2,3,4} (自动去重排序)
set<int> s3{5,3,8,3,1};              // {1,3,5,8}
set<int, greater<int>> s4{1,3,2};    // {3,2,1} (降序)

// multiset 允许重复
multiset<int> ms{1,2,2,3};           // {1,2,2,3}
```

---

### 6. `std::unordered_set` / `std::unordered_multiset` 构造方法

| 构造方式   | 语法                                | 说明                     | 示例                                         |
| ---------- | ----------------------------------- | ------------------------ | -------------------------------------------- |
| 默认构造   | `unordered_set<T> us;`              | 空 unordered_set         | `unordered_set<int> us;`                     |
| 范围构造   | `unordered_set<T> us(first, last);` | 从迭代器范围构造         | `unordered_set<int> us(v.begin(), v.end());` |
| 拷贝构造   | `unordered_set<T> us(us2);`         | 复制另一个 unordered_set | `unordered_set<int> us2(us1);`               |
| 初始化列表 | `unordered_set<T> us{...};`         | C++11 列表初始化         | `unordered_set<int> us{1,2,3};`              |
| 指定桶数量 | `unordered_set<T> us(n);`           | 预留 n 个桶              | `unordered_set<int> us(100);`                |

```cpp
// 示例代码
unordered_set<int> us1;              // {}
unordered_set<int> us2{4,2,3,2,1};   // {1,2,3,4} (无序,去重)
vector<int> v{5,3,5,1};
unordered_set<int> us3(v.begin(), v.end());  // {1,3,5}
```

---

### 7. `std::map` / `std::multimap` 构造方法

| 构造方式     | 语法                           | 说明               | 示例                                  |
| ------------ | ------------------------------ | ------------------ | ------------------------------------- |
| 默认构造     | `map<K,V> m;`                  | 空 map             | `map<string,int> m;`                  |
| 范围构造     | `map<K,V> m(first, last);`     | 从迭代器范围构造   | `map<int,int> m(v.begin(), v.end());` |
| 拷贝构造     | `map<K,V> m(m2);`              | 复制另一个 map     | `map<string,int> m2(m1);`             |
| 初始化列表   | `map<K,V> m{{k1,v1},{k2,v2}};` | C++11 列表初始化   | `map<string,int> m{{"a",1},{"b",2}};` |
| 自定义比较器 | `map<K,V,Comp> m;`             | 使用自定义比较函数 | `map<int,int,greater<int>> m;`        |

```cpp
// 示例代码
map<string, int> m1;                 // {}
map<string, int> m2{{"apple",1}, {"banana",2}};  // {{"apple",1}, {"banana",2}}
map<int, string> m3(m2.begin(), m2.end());
map<int, int, greater<int>> m4;      // 键降序排列

// multimap 允许键重复
multimap<string, int> mm{{"a",1}, {"a",2}};  // 允许重复键
```

---

### 8. `std::unordered_map` / `std::unordered_multimap` 构造方法

| 构造方式   | 语法                                  | 说明                     | 示例                                             |
| ---------- | ------------------------------------- | ------------------------ | ------------------------------------------------ |
| 默认构造   | `unordered_map<K,V> um;`              | 空 unordered_map         | `unordered_map<string,int> um;`                  |
| 范围构造   | `unordered_map<K,V> um(first, last);` | 从迭代器范围构造         | `unordered_map<int,int> um(v.begin(), v.end());` |
| 拷贝构造   | `unordered_map<K,V> um(um2);`         | 复制另一个 unordered_map | `unordered_map<string,int> um2(um1);`            |
| 初始化列表 | `unordered_map<K,V> um{{k1,v1}};`     | C++11 列表初始化         | `unordered_map<string,int> um{{"x",1}};`         |
| 指定桶数量 | `unordered_map<K,V> um(n);`           | 预留 n 个桶              | `unordered_map<string,int> um(100);`             |

```cpp
// 示例代码
unordered_map<string, int> um1;      // {}
unordered_map<string, int> um2{{"key1",10}, {"key2",20}};
unordered_map<int, string> um3(100); // 预留100个桶优化性能
```

---

### 9. `std::stack` 构造方法 (容器适配器)

| 构造方式     | 语法                       | 说明                   | 示例                                 |
| ------------ | -------------------------- | ---------------------- | ------------------------------------ |
| 默认构造     | `stack<T> stk;`            | 空栈（默认基于 deque） | `stack<int> stk;`                    |
| 指定底层容器 | `stack<T, Container> stk;` | 使用指定容器           | `stack<int, vector<int>> stk;`       |
| 从容器构造   | `stack<T> stk(container);` | 从底层容器复制构造     | `stack<int> stk(deque<int>{1,2,3});` |

```cpp
// 示例代码
stack<int> stk1;                     // 默认 deque 作底层
stack<int, vector<int>> stk2;        // vector 作底层
deque<int> dq{1,2,3};
stack<int> stk3(dq);                 // 从 deque 构造
```

---

### 10. `std::queue` 构造方法 (容器适配器)

| 构造方式     | 语法                     | 说明                     | 示例                               |
| ------------ | ------------------------ | ------------------------ | ---------------------------------- |
| 默认构造     | `queue<T> q;`            | 空队列（默认基于 deque） | `queue<int> q;`                    |
| 指定底层容器 | `queue<T, Container> q;` | 使用指定容器             | `queue<int, list<int>> q;`         |
| 从容器构造   | `queue<T> q(container);` | 从底层容器复制构造       | `queue<int> q(deque<int>{1,2,3});` |

```cpp
// 示例代码
queue<int> q1;                       // 默认 deque 作底层
queue<int, list<int>> q2;            // list 作底层
```

---

### 11. `std::priority_queue` 构造方法 (容器适配器)

| 构造方式     | 语法                                           | 说明                      | 示例                                                 |
| ------------ | ---------------------------------------------- | ------------------------- | ---------------------------------------------------- |
| 默认构造     | `priority_queue<T> pq;`                        | 最大堆（默认基于 vector） | `priority_queue<int> pq;`                            |
| 范围构造     | `priority_queue<T> pq(first, last);`           | 从迭代器范围构造          | `priority_queue<int> pq(v.begin(), v.end());`        |
| 最小堆       | `priority_queue<T, vector<T>, greater<T>> pq;` | 使用 greater 构造最小堆   | `priority_queue<int, vector<int>, greater<int>> pq;` |
| 自定义比较器 | `priority_queue<T, Container, Comp> pq;`       | 自定义比较函数            | 见下方示例                                           |

```cpp
// 示例代码
priority_queue<int> pq1;             // 最大堆
pq1.push(3); pq1.push(1); pq1.push(5);
pq1.top();                           // 5

// 最小堆
priority_queue<int, vector<int>, greater<int>> pq2;
pq2.push(3); pq2.push(1); pq2.push(5);
pq2.top();                           // 1

// 从范围构造
vector<int> v{4,2,5,1};
priority_queue<int> pq3(v.begin(), v.end());  // 最大堆

// 自定义比较器
auto cmp = [](int a, int b) { return a > b; }; // 最小堆
priority_queue<int, vector<int>, decltype(cmp)> pq4(cmp);
```

---

### 12. `std::array` 构造方法 (C++11)

| 构造方式   | 语法                             | 说明                             | 示例                           |
| ---------- | -------------------------------- | -------------------------------- | ------------------------------ |
| 默认构造   | `array<T, N> arr;`               | 未初始化（基本类型）或默认初始化 | `array<int, 5> arr;`           |
| 聚合初始化 | `array<T, N> arr = {v1,v2,...};` | 初始化列表                       | `array<int, 3> arr = {1,2,3};` |
| 统一初始化 | `array<T, N> arr{v1,v2,...};`    | C++11 花括号初始化               | `array<int, 3> arr{1,2,3};`    |
| fill 填充  | `arr.fill(val);`                 | 填充所有元素为 val               | `arr.fill(0);`                 |

```cpp
// 示例代码
array<int, 5> arr1;                  // 未初始化
array<int, 5> arr2 = {1,2,3,4,5};    // {1,2,3,4,5}
array<int, 3> arr3{10,20,30};        // {10,20,30}
array<int, 4> arr4{};                // {0,0,0,0} (值初始化)
arr4.fill(7);                        // {7,7,7,7}
```

---

### 13. `std::pair` 和 `std::tuple` 构造方法

#### `std::pair` 构造

```cpp
// 方式1: 默认构造
pair<int, string> p1;                // {0, ""}

// 方式2: 直接初始化
pair<int, string> p2(1, "hello");    // {1, "hello"}

// 方式3: make_pair
auto p3 = make_pair(42, "world");    // {42, "world"}

// 方式4: 列表初始化
pair<int, string> p4{100, "test"};   // {100, "test"}

// 访问
p4.first;   // 100
p4.second;  // "test"
```

#### `std::tuple` 构造

```cpp
// 方式1: 默认构造
tuple<int, double, string> t1;       // {0, 0.0, ""}

// 方式2: 直接初始化
tuple<int, double, string> t2(1, 3.14, "pi");

// 方式3: make_tuple
auto t3 = make_tuple(42, 2.71, "e");

// 方式4: 列表初始化
tuple<int, double, string> t4{100, 1.5, "test"};

// 访问
get<0>(t4);  // 100
get<1>(t4);  // 1.5
get<2>(t4);  // "test"

// C++17 结构化绑定
auto [a, b, c] = t4;  // a=100, b=1.5, c="test"
```

---

## 二、容器成员函数 CheatSheet

---

## Cheat Sheet: Common Interfaces (Member Functions)

| Function   | Return Type                    | Description                            | Example                       |
| ---------- | ------------------------------ | -------------------------------------- | ----------------------------- |
| `size()`   | `size_type` (usually `size_t`) | Number of elements in the container    | `myVec.size();` → `3`         |
| `empty()`  | `bool`                         | Checks if the container is empty       | `myMap.empty();` → `true`     |
| `clear()`  | `void`                         | Removes all elements from container    | `mySet.clear();`              |
| `begin()`  | Iterator                       | Returns iterator to the first element  | `auto it = myVec.begin();`    |
| `end()`    | Iterator                       | Returns iterator past the last element | `auto it = myVec.end();`      |
| `insert()` | Iterator or pair               | Inserts elements into the container    | `mySet.insert(3);`            |
| `erase()`  | Iterator                       | Removes elements from container        | `myVec.erase(myVec.begin());` |

---

## Unique Interface: `std::string`

| Function               | Return Type   | Description                        | Example                                       |
| ---------------------- | ------------- | ---------------------------------- | --------------------------------------------- |
| `length()` / `size()`  | `size_t`      | Number of characters in the string | `"Hello".length();` → `5`                     |
| `substr(pos, len)`     | `std::string` | Returns substring                  | `"Hello".substr(1, 3);` → `"ell"`             |
| `find(str)`            | `size_t`      | Finds substring position or `npos` | `"Hello".find("ell");` → `1`                  |
| `c_str()`              | `const char*` | Returns C-style string             | `printf("%s", myStr.c_str());`                |
| `append(str)`          | `string&`     | Adds characters to end             | `"Hello".append(" World");` → `"Hello World"` |
| `replace(pos,len,str)` | `string&`     | Replace substring                  | `"Hello".replace(1,3,"a");` → `"Hao"`         |

---

## Unique Interface: `std::vector`

| Function         | Return Type          | Description                                | Example             |
| ---------------- | -------------------- | ------------------------------------------ | ------------------- |
| `push_back(val)` | `void`               | Adds element at the end                    | `vec.push_back(3);` |
| `pop_back()`     | `void`               | Removes the last element                   | `vec.pop_back();`   |
| `reserve(n)`     | `void`               | Allocates memory for at least `n` elements | `vec.reserve(10);`  |
| `capacity()`     | `size_t`             | Returns the total allocated memory         | `vec.capacity();`   |
| `resize(n,val)`  | `void`               | Resizes vector to contain `n` elements     | `vec.resize(5, 0);` |
| `at(i)`          | Reference to element | Access element with bounds check           | `vec.at(2);`        |

---

## Unique Interface: `std::list`

| Function          | Return Type | Description                     | Example                  |
| ----------------- | ----------- | ------------------------------- | ------------------------ |
| `push_front(val)` | `void`      | Adds element at the front       | `lst.push_front(5);`     |
| `pop_front()`     | `void`      | Removes the front element       | `lst.pop_front();`       |
| `splice(pos,lst)` | `void`      | Transfers elements from list    | `lst1.splice(it, lst2);` |
| `remove(val)`     | `void`      | Removes elements equal to `val` | `lst.remove(3);`         |

---

## Unique Interface: `std::deque`

| Function          | Return Type | Description               | Example             |
| ----------------- | ----------- | ------------------------- | ------------------- |
| `push_front(val)` | `void`      | Adds element to front     | `dq.push_front(3);` |
| `pop_front()`     | `void`      | Removes the front element | `dq.pop_front();`   |
| `push_back(val)`  | `void`      | Adds element to back      | `dq.push_back(3);`  |
| `pop_back()`      | `void`      | Removes the back element  | `dq.pop_back();`    |

---

## Unique Interface: `std::set` and `std::unordered_set`

| Function       | Return Type           | Description                     | Example                 |
| -------------- | --------------------- | ------------------------------- | ----------------------- |
| `count(val)`   | `size_t`              | Counts occurrences (`0` or `1`) | `mySet.count(5);` → `1` |
| `find(val)`    | Iterator              | Finds element                   | `mySet.find(5);`        |
| `emplace(val)` | Pair (iterator, bool) | Constructs element in-place     | `mySet.emplace(3);`     |

---

## Unique Interface: `std::map` and `std::unordered_map`

| Function          | Return Type               | Description                                      | Example                     |
| ----------------- | ------------------------- | ------------------------------------------------ | --------------------------- |
| `operator[](key)` | Reference to mapped value | Access or insert mapped value by key             | `myMap["key"] = 5;`         |
| `find(key)`       | Iterator                  | Finds element by key                             | `myMap.find("key");`        |
| `count(key)`      | `size_t`                  | Returns number of elements with key (`0` or `1`) | `myMap.count("key");` → `1` |
| `emplace(k,v)`    | Pair (iterator, bool)     | Constructs pair in-place                         | `myMap.emplace("key",5);`   |

---

## Unique Interface: Container Adaptors (`std::stack`, `std::queue`, `std::priority_queue`)

### `std::stack`

| Function  | Return Type          | Description          | Example        |
| --------- | -------------------- | -------------------- | -------------- |
| `push(v)` | `void`               | Adds element on top  | `stk.push(5);` |
| `pop()`   | `void`               | Removes top element  | `stk.pop();`   |
| `top()`   | Reference to element | Accesses top element | `stk.top();`   |

### `std::queue`

| Function  | Return Type          | Description            | Example      |
| --------- | -------------------- | ---------------------- | ------------ |
| `push(v)` | `void`               | Adds element to end    | `q.push(3);` |
| `pop()`   | `void`               | Removes front element  | `q.pop();`   |
| `front()` | Reference to element | Accesses front element | `q.front();` |

### `std::priority_queue`

| Function  | Return Type          | Description                       | Example       |
| --------- | -------------------- | --------------------------------- | ------------- |
| `push(v)` | `void`               | Adds element                      | `pq.push(3);` |
| `pop()`   | `void`               | Removes highest priority element  | `pq.pop();`   |
| `top()`   | Reference to element | Accesses highest priority element | `pq.top();`   |

---

**Note on `emplace`:**

- Constructs elements directly within the container, reducing unnecessary copy/move operations, beneficial when copying/moving objects is costly.

---

**Note on Iterators:**

- Use iterators (`begin()`, `end()`) to navigate containers efficiently.

---

## STL Algorithms Cheat Sheet

### Sorting & Permutation

| Function                            | Header        | Description                                  | Example                                                |
| ----------------------------------- | ------------- | -------------------------------------------- | ------------------------------------------------------ |
| `sort(first, last)`                 | `<algorithm>` | Sorts elements in ascending order            | `sort(vec.begin(), vec.end());`                        |
| `sort(first, last, comp)`           | `<algorithm>` | Sorts with custom comparator                 | `sort(vec.begin(), vec.end(), greater<int>());`        |
| `stable_sort(first, last)`          | `<algorithm>` | Stable sort (preserves equal elements order) | `stable_sort(vec.begin(), vec.end());`                 |
| `partial_sort(first, middle, last)` | `<algorithm>` | Partially sorts first n elements             | `partial_sort(vec.begin(), vec.begin()+3, vec.end());` |
| `nth_element(first, nth, last)`     | `<algorithm>` | Puts nth element in correct position         | `nth_element(vec.begin(), vec.begin()+2, vec.end());`  |
| `reverse(first, last)`              | `<algorithm>` | Reverses elements in range                   | `reverse(vec.begin(), vec.end());`                     |
| `next_permutation(first, last)`     | `<algorithm>` | Generates next lexicographical permutation   | `next_permutation(vec.begin(), vec.end());`            |

### Searching & Finding

| Function                          | Header        | Return Type  | Description                              | Example                                                     |
| --------------------------------- | ------------- | ------------ | ---------------------------------------- | ----------------------------------------------------------- |
| `find(first, last, val)`          | `<algorithm>` | Iterator     | Finds first occurrence of value          | `find(vec.begin(), vec.end(), 5);`                          |
| `find_if(first, last, pred)`      | `<algorithm>` | Iterator     | Finds first element satisfying predicate | `find_if(vec.begin(), vec.end(), [](int x){return x>5;});`  |
| `binary_search(first, last, val)` | `<algorithm>` | `bool`       | Checks if value exists (sorted range)    | `binary_search(vec.begin(), vec.end(), 5);`                 |
| `lower_bound(first, last, val)`   | `<algorithm>` | Iterator     | First position where val can be inserted | `lower_bound(vec.begin(), vec.end(), 5);`                   |
| `upper_bound(first, last, val)`   | `<algorithm>` | Iterator     | Last position where val can be inserted  | `upper_bound(vec.begin(), vec.end(), 5);`                   |
| `equal_range(first, last, val)`   | `<algorithm>` | Pair of iter | Returns range of equal elements          | `equal_range(vec.begin(), vec.end(), 5);`                   |
| `count(first, last, val)`         | `<algorithm>` | `size_t`     | Counts occurrences of value              | `count(vec.begin(), vec.end(), 5);`                         |
| `count_if(first, last, pred)`     | `<algorithm>` | `size_t`     | Counts elements satisfying predicate     | `count_if(vec.begin(), vec.end(), [](int x){return x>5;});` |

### Min/Max Operations

| Function                      | Header        | Return Type | Description                           | Example                                   |
| ----------------------------- | ------------- | ----------- | ------------------------------------- | ----------------------------------------- |
| `min_element(first, last)`    | `<algorithm>` | Iterator    | Finds minimum element                 | `min_element(vec.begin(), vec.end());`    |
| `max_element(first, last)`    | `<algorithm>` | Iterator    | Finds maximum element                 | `max_element(vec.begin(), vec.end());`    |
| `minmax_element(first, last)` | `<algorithm>` | Pair        | Finds both min and max elements       | `minmax_element(vec.begin(), vec.end());` |
| `min(a, b)` / `max(a, b)`     | `<algorithm>` | Value type  | Returns minimum/maximum of two values | `min(5, 3);` → `3`                        |

### Modifying Operations

| Function                                 | Header        | Return Type | Description                       | Example                                                                   |
| ---------------------------------------- | ------------- | ----------- | --------------------------------- | ------------------------------------------------------------------------- |
| `fill(first, last, val)`                 | `<algorithm>` | `void`      | Fills range with value            | `fill(vec.begin(), vec.end(), 0);`                                        |
| `copy(first, last, result)`              | `<algorithm>` | Iterator    | Copies elements to another range  | `copy(vec1.begin(), vec1.end(), vec2.begin());`                           |
| `transform(first, last, result, op)`     | `<algorithm>` | Iterator    | Applies operation to each element | `transform(vec.begin(), vec.end(), vec.begin(), [](int x){return x*2;});` |
| `replace(first, last, old_val, new_val)` | `<algorithm>` | `void`      | Replaces all occurrences of value | `replace(vec.begin(), vec.end(), 0, -1);`                                 |
| `remove(first, last, val)`               | `<algorithm>` | Iterator    | Removes elements (doesn't resize) | `vec.erase(remove(vec.begin(), vec.end(), 0), vec.end());`                |
| `unique(first, last)`                    | `<algorithm>` | Iterator    | Removes consecutive duplicates    | `vec.erase(unique(vec.begin(), vec.end()), vec.end());`                   |

### Set Operations (on sorted ranges)

| Function                                                 | Header        | Return Type | Description                   | Example                                                                                |
| -------------------------------------------------------- | ------------- | ----------- | ----------------------------- | -------------------------------------------------------------------------------------- |
| `merge(first1, last1, first2, last2, result)`            | `<algorithm>` | Iterator    | Merges two sorted ranges      | `merge(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());`                   |
| `set_union(first1, last1, first2, last2, result)`        | `<algorithm>` | Iterator    | Union of two sorted ranges    | `set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));`        |
| `set_intersection(first1, last1, first2, last2, result)` | `<algorithm>` | Iterator    | Intersection of sorted ranges | `set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));` |

### Heap Operations

| Function                 | Header        | Return Type | Description                  | Example                              |
| ------------------------ | ------------- | ----------- | ---------------------------- | ------------------------------------ |
| `make_heap(first, last)` | `<algorithm>` | `void`      | Creates heap from range      | `make_heap(vec.begin(), vec.end());` |
| `push_heap(first, last)` | `<algorithm>` | `void`      | Adds element to heap         | `push_heap(vec.begin(), vec.end());` |
| `pop_heap(first, last)`  | `<algorithm>` | `void`      | Removes largest element      | `pop_heap(vec.begin(), vec.end());`  |
| `sort_heap(first, last)` | `<algorithm>` | `void`      | Sorts heap into sorted range | `sort_heap(vec.begin(), vec.end());` |

### Numeric Operations

| Function                                     | Header      | Return Type | Description                      | Example                                                     |
| -------------------------------------------- | ----------- | ----------- | -------------------------------- | ----------------------------------------------------------- |
| `accumulate(first, last, init)`              | `<numeric>` | Value type  | Sums elements with initial value | `accumulate(vec.begin(), vec.end(), 0);`                    |
| `accumulate(first, last, init, op)`          | `<numeric>` | Value type  | Applies operation cumulatively   | `accumulate(vec.begin(), vec.end(), 1, multiplies<int>());` |
| `inner_product(first1, last1, first2, init)` | `<numeric>` | Value type  | Dot product of two ranges        | `inner_product(v1.begin(), v1.end(), v2.begin(), 0);`       |
| `partial_sum(first, last, result)`           | `<numeric>` | Iterator    | Computes partial sums            | `partial_sum(vec.begin(), vec.end(), result.begin());`      |

### Common Usage Patterns

```cpp
// Sort vector in descending order
sort(vec.begin(), vec.end(), greater<int>());

// Find and erase all zeros
vec.erase(remove(vec.begin(), vec.end(), 0), vec.end());

// Check if vector is sorted
bool is_sorted = std::is_sorted(vec.begin(), vec.end());

// Find intersection of two sorted vectors
vector<int> intersection;
set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(intersection));

// Sum all elements
int sum = accumulate(vec.begin(), vec.end(), 0);

// Find element greater than 5
auto it = find_if(vec.begin(), vec.end(), [](int x) { return x > 5; });
```

---

With these charts, you have quick references to frequently used STL containers and algorithms, aiding efficient coding practice and effective preparation for interviews.
