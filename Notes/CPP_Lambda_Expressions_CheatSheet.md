# C++ Lambda Expressions Cheat Sheet for Competitive Programming

## Table of Contents

1. [Basic Lambda Syntax](#basic-lambda-syntax)
2. [Capture Modes](#capture-modes)
3. [Parameter Types](#parameter-types)
4. [Return Types](#return-types)
5. [Common Use Cases in Algorithms](#common-use-cases-in-algorithms)
6. [Lambda with STL Algorithms](#lambda-with-stl-algorithms)
7. [Advanced Lambda Patterns](#advanced-lambda-patterns)
8. [Performance Considerations](#performance-considerations)
9. [Common Pitfalls](#common-pitfalls)
10. [Quick Reference](#quick-reference)

---

## Basic Lambda Syntax

### Lambda Structure

```cpp
[capture](parameters) -> return_type { body }
//   ↑         ↑           ↑          ↑
//   |         |           |          └── Function body
//   |         |           └─────────── Return type (optional)
//   |         └─────────────────────── Parameters (optional)
//   └───────────────────────────────── Capture clause
```

### Simple Examples

```cpp
// Basic lambda
auto add = [](int a, int b) { return a + b; };
int result = add(3, 4);  // 7

// Lambda with explicit return type
auto divide = [](double a, double b) -> double {
    return a / b;
};

// Lambda without parameters
auto get_random = []() { return rand() % 100; };

// Lambda without return (void)
auto print_hello = []() { cout << "Hello!" << endl; };
```

---

## Capture Modes

### 1. No Capture `[]`

```cpp
// Cannot access outside variables
int x = 10;
auto lambda1 = []() {
    // cout << x;  // ❌ Error! Cannot access x
    return 42;
};
```

### 2. Capture by Value `[var]`

```cpp
int x = 10;
auto lambda2 = [x]() {
    return x * 2;  // ✅ OK, x is copied
    // x++;        // ❌ Error! x is const by default
};

// Mutable capture by value
auto lambda2_mut = [x]() mutable {
    x++;           // ✅ OK with mutable
    return x;
};
cout << x;  // Still 10, original unchanged
```

### 3. Capture by Reference `[&var]`

```cpp
int x = 10;
auto lambda3 = [&x]() {
    x++;           // ✅ OK, modifies original x
    return x;
};
lambda3();  // x is now 11
cout << x;  // 11
```

### 4. Capture All by Value `[=]`

```cpp
int x = 10, y = 20;
auto lambda4 = [=]() {
    return x + y;  // ✅ Both x and y are copied
};
```

### 5. Capture All by Reference `[&]`

```cpp
int x = 10, y = 20;
auto lambda5 = [&]() {
    x++; y++;      // ✅ Both x and y are modified
    return x + y;
};
```

### 6. Mixed Capture `[=, &var]` or `[&, var]`

```cpp
int x = 10, y = 20, z = 30;

// Capture all by value, but z by reference
auto lambda6 = [=, &z]() {
    z++;                    // ✅ z modified (by reference)
    return x + y + z;       // ✅ x, y copied (by value)
};

// Capture all by reference, but x by value
auto lambda7 = [&, x]() {
    y++; z++;               // ✅ y, z modified (by reference)
    return x + y + z;       // ✅ x copied (by value)
};
```

### 7. Specific Variable Capture

```cpp
int a = 1, b = 2, c = 3, d = 4;

// Capture specific variables
auto lambda8 = [a, &b, c]() {
    // a, c by value; b by reference
    b++;
    return a + b + c;
};
```

---

## Parameter Types

### Auto Parameters (C++14+)

```cpp
// Generic lambda
auto generic = [](auto a, auto b) {
    return a + b;
};

int result1 = generic(3, 4);        // int + int
double result2 = generic(3.14, 2.0); // double + double
string result3 = generic(string("Hello"), string(" World")); // string + string
```

### Template Parameters (C++20+)

```cpp
// Template lambda
auto template_lambda = []<typename T>(T a, T b) {
    return a + b;
};
```

### Variadic Parameters (C++14+)

```cpp
// Variadic lambda
auto print_all = [](auto... args) {
    ((cout << args << " "), ...);  // C++17 fold expression
    cout << endl;
};

print_all(1, 2.5, "hello", 'c');  // 1 2.5 hello c
```

---

## Return Types

### Automatic Return Type Deduction

```cpp
auto lambda1 = [](int x) { return x * 2; };        // Returns int
auto lambda2 = [](int x) { return x * 2.0; };      // Returns double
```

### Explicit Return Type

```cpp
auto lambda3 = [](int x) -> double { return x; };  // Explicit double return
auto lambda4 = [](bool flag) -> int {
    if (flag) return 1;
    return 0;
};
```

### Multiple Return Statements

```cpp
auto lambda5 = [](int x) -> int {  // Explicit return type needed
    if (x > 0) return x;
    if (x < 0) return -x;
    return 1;
};
```

---

## Common Use Cases in Algorithms

### 1. Custom Comparators for Sorting

```cpp
vector<pair<int, int>> points = {{1, 3}, {2, 1}, {0, 4}};

// Sort by x-coordinate
sort(points.begin(), points.end(), [](const auto& a, const auto& b) {
    return a.first < b.first;
});

// Sort by distance from origin
sort(points.begin(), points.end(), [](const auto& a, const auto& b) {
    return a.first*a.first + a.second*a.second < b.first*b.first + b.second*b.second;
});

// Sort strings by length
vector<string> words = {"apple", "pie", "a", "banana"};
sort(words.begin(), words.end(), [](const string& a, const string& b) {
    return a.length() < b.length();
});
```

### 2. Custom Comparators for Priority Queue

```cpp
// Min heap for pairs (by second element)
priority_queue<pair<int, int>, vector<pair<int, int>>,
               decltype([](const auto& a, const auto& b) {
                   return a.second > b.second;  // Min heap
               })> pq;

// Or using auto (C++20)
auto cmp = [](const auto& a, const auto& b) { return a.second > b.second; };
priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq2(cmp);
```

### 3. Filtering with `remove_if`

```cpp
vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Remove even numbers
nums.erase(remove_if(nums.begin(), nums.end(), [](int x) {
    return x % 2 == 0;
}), nums.end());
// nums = {1, 3, 5, 7, 9}
```

### 4. Transforming with `transform`

```cpp
vector<int> nums = {1, 2, 3, 4, 5};
vector<int> squares(nums.size());

transform(nums.begin(), nums.end(), squares.begin(), [](int x) {
    return x * x;
});
// squares = {1, 4, 9, 16, 25}
```

### 5. Accumulation with Custom Operation

```cpp
vector<int> nums = {1, 2, 3, 4, 5};

// Product of all elements
int product = accumulate(nums.begin(), nums.end(), 1, [](int acc, int val) {
    return acc * val;
});

// Sum of squares
int sum_squares = accumulate(nums.begin(), nums.end(), 0, [](int acc, int val) {
    return acc + val * val;
});
```

---

## Lambda with STL Algorithms

### Searching and Finding

```cpp
vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Find first even number
auto it = find_if(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });

// Count odd numbers
int odd_count = count_if(nums.begin(), nums.end(), [](int x) { return x % 2 == 1; });

// Check if all numbers are positive
bool all_positive = all_of(nums.begin(), nums.end(), [](int x) { return x > 0; });

// Check if any number is greater than 5
bool any_gt_5 = any_of(nums.begin(), nums.end(), [](int x) { return x > 5; });
```

### Generation and Filling

```cpp
vector<int> nums(10);

// Fill with random numbers
mt19937 gen(42);
uniform_int_distribution<int> dis(1, 100);
generate(nums.begin(), nums.end(), [&gen, &dis]() { return dis(gen); });

// Fill with fibonacci numbers
int a = 0, b = 1;
generate(nums.begin(), nums.end(), [&a, &b]() mutable {
    int temp = a + b;
    a = b;
    b = temp;
    return a;
});
```

### Binary Search with Custom Comparator

```cpp
vector<pair<int, string>> data = {{1, "one"}, {3, "three"}, {5, "five"}};

// Binary search by first element
auto it = lower_bound(data.begin(), data.end(), make_pair(3, ""),
    [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
```

---

## Advanced Lambda Patterns

### 1. Capturing `this` in Member Functions

```cpp
class MyClass {
    int value = 42;
public:
    void process(vector<int>& nums) {
        // Capture this to access member variables
        transform(nums.begin(), nums.end(), nums.begin(), [this](int x) {
            return x + value;  // Access member variable
        });

        // C++17: Capture *this by value
        auto lambda = [*this](int x) { return x + value; };
    }
};
```

### 2. Recursive Lambda

```cpp
// C++14 way (using std::function)
function<int(int)> factorial = [&factorial](int n) -> int {
    return n <= 1 ? 1 : n * factorial(n - 1);
};

// C++17 way (better)
auto factorial_v2 = [](int n) {
    auto impl = [](int n, auto& self) -> int {
        return n <= 1 ? 1 : n * self(n - 1, self);
    };
    return impl(n, impl);
};
```

### 3. Lambda Factories

```cpp
// Factory function returning lambdas
auto make_multiplier(int factor) {
    return [factor](int x) { return x * factor; };
}

auto times_2 = make_multiplier(2);
auto times_10 = make_multiplier(10);

cout << times_2(5);   // 10
cout << times_10(3);  // 30
```

### 4. Stateful Lambdas

```cpp
// Counter lambda
auto counter = [count = 0]() mutable { return ++count; };

cout << counter();  // 1
cout << counter();  // 2
cout << counter();  // 3

// Accumulator lambda
auto accumulator = [sum = 0](int value) mutable {
    sum += value;
    return sum;
};

cout << accumulator(10);  // 10
cout << accumulator(5);   // 15
cout << accumulator(3);   // 18
```

---

## Performance Considerations

### 1. Capture Efficiency

```cpp
// ❌ Expensive: copying large objects
struct LargeObject {
    vector<int> data = vector<int>(1000000);
};

LargeObject large;
auto bad_lambda = [large]() {  // Copies entire large object
    return large.data.size();
};

// ✅ Efficient: capture by reference
auto good_lambda = [&large]() {  // Only stores reference
    return large.data.size();
};

// ✅ Efficient: capture only what you need
auto better_lambda = [size = large.data.size()]() {  // C++14 init capture
    return size;
};
```

### 2. Generic vs Specific Lambdas

```cpp
// Generic lambda (may be slower due to template instantiation)
auto generic = [](auto a, auto b) { return a + b; };

// Specific lambda (faster)
auto specific = [](int a, int b) { return a + b; };
```

### 3. Inline vs Function Objects

```cpp
// Lambda (usually inlined)
sort(vec.begin(), vec.end(), [](int a, int b) { return a < b; });

// Function pointer (not inlined)
bool compare(int a, int b) { return a < b; }
sort(vec.begin(), vec.end(), compare);  // Slower

// Function object (inlined)
struct Compare {
    bool operator()(int a, int b) const { return a < b; }
};
sort(vec.begin(), vec.end(), Compare{});  // Fast
```

---

## Common Pitfalls

### 1. Capturing Local Variables by Reference

```cpp
function<int()> bad_function() {
    int local = 42;
    return [&local]() { return local; };  // ❌ Dangling reference!
}

function<int()> good_function() {
    int local = 42;
    return [local]() { return local; };   // ✅ Capture by value
}
```

### 2. Modifying Captured Variables

```cpp
int x = 10;
auto lambda = [x]() {
    x++;          // ❌ Error! x is const by default
    return x;
};

// ✅ Solution 1: Use mutable
auto lambda_mut = [x]() mutable {
    x++;
    return x;
};

// ✅ Solution 2: Capture by reference
auto lambda_ref = [&x]() {
    x++;
    return x;
};
```

### 3. Forgetting Return Type for Complex Lambdas

```cpp
auto bad_lambda = [](bool flag) {
    if (flag) return 1;      // int
    return 2.5;              // ❌ Error! Different return types
};

auto good_lambda = [](bool flag) -> double {  // ✅ Explicit return type
    if (flag) return 1;
    return 2.5;
};
```

---

## Quick Reference

### Capture Syntax Quick Guide

| Capture   | Meaning                                     |
| --------- | ------------------------------------------- |
| `[]`      | No capture                                  |
| `[x]`     | Capture x by value                          |
| `[&x]`    | Capture x by reference                      |
| `[=]`     | Capture all used variables by value         |
| `[&]`     | Capture all used variables by reference     |
| `[=, &x]` | Capture all by value, except x by reference |
| `[&, x]`  | Capture all by reference, except x by value |
| `[this]`  | Capture this pointer                        |
| `[*this]` | Capture this object by value (C++17)        |

### Common Lambda Patterns for LeetCode

#### 1. Sorting 2D Arrays

```cpp
// Sort by first element, then by second
sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
    return a[0] != b[0] ? a[0] < b[0] : a[1] < b[1];
});
```

#### 2. Custom Priority Queue

```cpp
// Max heap by absolute value
auto cmp = [](int a, int b) { return abs(a) < abs(b); };
priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
```

#### 3. Graph DFS with Lambda

```cpp
function<void(int)> dfs = [&](int node) {
    visited[node] = true;
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
    }
};
```

#### 4. Binary Search with Lambda

```cpp
// Find first element >= target
auto it = lower_bound(nums.begin(), nums.end(), target, [](int a, int b) {
    return a < b;
});
```

#### 5. Two-Pointer Technique

```cpp
// Remove elements that satisfy condition
nums.erase(remove_if(nums.begin(), nums.end(), [target](int x) {
    return x == target;
}), nums.end());
```

This comprehensive guide covers everything you need to know about lambdas for competitive programming! 🚀
