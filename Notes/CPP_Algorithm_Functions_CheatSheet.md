# C++ Algorithm Functions Cheat Sheet for Competitive Programming

## Table of Contents

1. [Numeric Operations](#numeric-operations)
2. [Min/Max Operations](#minmax-operations)
3. [Sorting and Searching](#sorting-and-searching)
4. [Set Operations](#set-operations)
5. [Permutation and Combination](#permutation-and-combination)
6. [Binary Operations](#binary-operations)
7. [String Operations](#string-operations)
8. [Container Utilities](#container-utilities)
9. [Iterator Utilities](#iterator-utilities)
10. [Quick Reference Table](#quick-reference-table)

---

## Numeric Operations

### `std::accumulate` - Sum/Reduce Operations

```cpp
#include <numeric>

vector<int> nums = {1, 2, 3, 4, 5};

// Basic sum
int sum = accumulate(nums.begin(), nums.end(), 0);  // 15

// Sum with different initial value
int sum_plus_10 = accumulate(nums.begin(), nums.end(), 10);  // 25

// Product
int product = accumulate(nums.begin(), nums.end(), 1, multiplies<int>());  // 120

// Custom operation (concatenate strings)
vector<string> words = {"Hello", " ", "World"};
string result = accumulate(words.begin(), words.end(), string(""));  // "Hello World"

// Lambda operation
int sum_of_squares = accumulate(nums.begin(), nums.end(), 0,
    [](int sum, int val) { return sum + val * val; });  // 55
```

### `std::partial_sum` - Prefix Sum

```cpp
#include <numeric>

vector<int> nums = {1, 2, 3, 4, 5};
vector<int> prefix_sum(nums.size());

partial_sum(nums.begin(), nums.end(), prefix_sum.begin());
// prefix_sum = {1, 3, 6, 10, 15}

// Custom operation (prefix product)
vector<int> prefix_product(nums.size());
partial_sum(nums.begin(), nums.end(), prefix_product.begin(), multiplies<int>());
// prefix_product = {1, 2, 6, 24, 120}
```

### `std::adjacent_difference` - Differences Between Adjacent Elements

```cpp
#include <numeric>

vector<int> nums = {1, 3, 6, 10, 15};
vector<int> diff(nums.size());

adjacent_difference(nums.begin(), nums.end(), diff.begin());
// diff = {1, 2, 3, 4, 5}  // First element unchanged, rest are differences
```

### `std::inner_product` - Dot Product

```cpp
#include <numeric>

vector<int> a = {1, 2, 3};
vector<int> b = {4, 5, 6};

int dot_product = inner_product(a.begin(), a.end(), b.begin(), 0);  // 32
// 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
```

---

## Min/Max Operations

### `std::min_element` / `std::max_element`

```cpp
#include <algorithm>

vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};

// Find iterators
auto min_it = min_element(nums.begin(), nums.end());
auto max_it = max_element(nums.begin(), nums.end());

// Get values
int min_val = *min_it;  // 1
int max_val = *max_it;  // 9

// Get indices
int min_idx = min_it - nums.begin();  // 1
int max_idx = max_it - nums.begin();  // 5

// Custom comparator
auto max_abs_it = max_element(nums.begin(), nums.end(),
    [](int a, int b) { return abs(a) < abs(b); });
```

### `std::minmax_element` - Find Both Min and Max

```cpp
#include <algorithm>

vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};

auto [min_it, max_it] = minmax_element(nums.begin(), nums.end());  // C++17
// Or C++11: auto result = minmax_element(nums.begin(), nums.end());
//          auto min_it = result.first, max_it = result.second;

int min_val = *min_it;  // 1
int max_val = *max_it;  // 9
```

### `std::min` / `std::max` - Compare Values

```cpp
#include <algorithm>

int a = 5, b = 3;
int minimum = min(a, b);      // 3
int maximum = max(a, b);      // 5

// Multiple values (C++11)
int min_of_many = min({5, 3, 8, 1, 9});  // 1
int max_of_many = max({5, 3, 8, 1, 9});  // 9

// With custom comparator
string s1 = "hello", s2 = "world";
string shorter = min(s1, s2, [](const string& a, const string& b) {
    return a.length() < b.length();
});
```

---

## Sorting and Searching

### `std::sort` - Sorting

```cpp
#include <algorithm>

vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};

// Basic sort (ascending)
sort(nums.begin(), nums.end());
// nums = {1, 1, 2, 3, 4, 5, 6, 9}

// Descending sort
sort(nums.begin(), nums.end(), greater<int>());
// nums = {9, 6, 5, 4, 3, 2, 1, 1}

// Custom comparator
vector<string> words = {"apple", "pie", "a", "longer"};
sort(words.begin(), words.end(), [](const string& a, const string& b) {
    return a.length() < b.length();  // Sort by length
});
// words = {"a", "pie", "apple", "longer"}
```

### `std::partial_sort` - Sort Only First K Elements

```cpp
#include <algorithm>

vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};

// Sort only first 3 elements
partial_sort(nums.begin(), nums.begin() + 3, nums.end());
// nums = {1, 1, 2, ...} (first 3 are the 3 smallest, rest unspecified order)
```

### `std::nth_element` - Find Kth Element

```cpp
#include <algorithm>

vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};

// Find 3rd smallest element (0-indexed)
nth_element(nums.begin(), nums.begin() + 2, nums.end());
int kth_element = nums[2];  // 2 (3rd smallest)
// Elements before index 2 are <= nums[2], elements after are >= nums[2]
```

### `std::binary_search` / `std::lower_bound` / `std::upper_bound`

```cpp
#include <algorithm>

vector<int> nums = {1, 1, 2, 3, 4, 5, 6, 9};  // Must be sorted

// Binary search
bool found = binary_search(nums.begin(), nums.end(), 4);  // true

// Lower bound (first position where element could be inserted)
auto lb = lower_bound(nums.begin(), nums.end(), 4);
int lb_idx = lb - nums.begin();  // 4

// Upper bound (last position where element could be inserted)
auto ub = upper_bound(nums.begin(), nums.end(), 1);
int ub_idx = ub - nums.begin();  // 2

// Equal range (range of equal elements)
auto [first, last] = equal_range(nums.begin(), nums.end(), 1);
int count = last - first;  // 2 (number of 1's)
```

---

## Set Operations

### Set Operations on Sorted Ranges

```cpp
#include <algorithm>

vector<int> a = {1, 2, 3, 4, 5};
vector<int> b = {3, 4, 5, 6, 7};
vector<int> result;

// Union
set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// result = {1, 2, 3, 4, 5, 6, 7}

result.clear();
// Intersection
set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// result = {3, 4, 5}

result.clear();
// Difference (a - b)
set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// result = {1, 2}

result.clear();
// Symmetric difference (elements in either but not both)
set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// result = {1, 2, 6, 7}
```

### `std::includes` - Check Subset

```cpp
#include <algorithm>

vector<int> superset = {1, 2, 3, 4, 5, 6};
vector<int> subset = {2, 4, 6};

bool is_subset = includes(superset.begin(), superset.end(),
                         subset.begin(), subset.end());  // true
```

---

## Permutation and Combination

### `std::next_permutation` / `std::prev_permutation`

```cpp
#include <algorithm>

vector<int> nums = {1, 2, 3};

// Generate all permutations
do {
    for (int x : nums) cout << x << " ";
    cout << endl;
} while (next_permutation(nums.begin(), nums.end()));
// Output: 1 2 3, 1 3 2, 2 1 3, 2 3 1, 3 1 2, 3 2 1

// Previous permutation
vector<int> desc = {3, 2, 1};
prev_permutation(desc.begin(), desc.end());
// desc = {3, 1, 2}
```

### `std::random_shuffle` / `std::shuffle`

```cpp
#include <algorithm>
#include <random>

vector<int> nums = {1, 2, 3, 4, 5};

// Modern way (C++11)
random_device rd;
mt19937 g(rd());
shuffle(nums.begin(), nums.end(), g);

// Legacy way (deprecated in C++14, removed in C++17)
// random_shuffle(nums.begin(), nums.end());
```

---

## Binary Operations

### `std::count` / `std::count_if`

```cpp
#include <algorithm>

vector<int> nums = {1, 2, 3, 2, 4, 2, 5};

// Count specific value
int count_2 = count(nums.begin(), nums.end(), 2);  // 3

// Count with condition
int count_even = count_if(nums.begin(), nums.end(),
    [](int x) { return x % 2 == 0; });  // 4
```

### `std::find` / `std::find_if`

```cpp
#include <algorithm>

vector<int> nums = {1, 2, 3, 4, 5};

// Find specific value
auto it = find(nums.begin(), nums.end(), 3);
if (it != nums.end()) {
    int index = it - nums.begin();  // 2
}

// Find with condition
auto it2 = find_if(nums.begin(), nums.end(),
    [](int x) { return x > 3; });  // Points to 4
```

### `std::all_of` / `std::any_of` / `std::none_of`

```cpp
#include <algorithm>

vector<int> nums = {2, 4, 6, 8};

bool all_even = all_of(nums.begin(), nums.end(),
    [](int x) { return x % 2 == 0; });  // true

bool any_greater_5 = any_of(nums.begin(), nums.end(),
    [](int x) { return x > 5; });  // true

bool none_negative = none_of(nums.begin(), nums.end(),
    [](int x) { return x < 0; });  // true
```

---

## String Operations

### String-Specific Algorithms

```cpp
#include <algorithm>
#include <string>

string s = "hello world";

// Transform to uppercase
transform(s.begin(), s.end(), s.begin(), ::toupper);
// s = "HELLO WORLD"

// Reverse
reverse(s.begin(), s.end());
// s = "DLROW OLLEH"

// Remove spaces
s.erase(remove(s.begin(), s.end(), ' '), s.end());

// Check if palindrome
string original = "racecar";
string reversed = original;
reverse(reversed.begin(), reversed.end());
bool is_palindrome = (original == reversed);
```

---

## Container Utilities

### `std::fill` / `std::fill_n`

```cpp
#include <algorithm>

vector<int> nums(5);

// Fill entire container
fill(nums.begin(), nums.end(), 42);
// nums = {42, 42, 42, 42, 42}

// Fill first n elements
fill_n(nums.begin(), 3, 99);
// nums = {99, 99, 99, 42, 42}
```

### `std::generate` / `std::generate_n`

```cpp
#include <algorithm>
#include <random>

vector<int> nums(5);
mt19937 gen(42);
uniform_int_distribution<int> dis(1, 100);

// Generate using function
generate(nums.begin(), nums.end(), [&]() { return dis(gen); });

// Generate first n elements
generate_n(nums.begin(), 3, [&]() { return dis(gen); });
```

### `std::iota` - Fill with Sequential Values

```cpp
#include <numeric>

vector<int> nums(5);
iota(nums.begin(), nums.end(), 1);
// nums = {1, 2, 3, 4, 5}

// Useful for creating index arrays
vector<int> indices(n);
iota(indices.begin(), indices.end(), 0);  // {0, 1, 2, ..., n-1}
```

---

## Iterator Utilities

### `std::advance` / `std::distance`

```cpp
#include <iterator>

vector<int> nums = {1, 2, 3, 4, 5};
auto it = nums.begin();

// Advance iterator
advance(it, 3);  // it now points to nums[3]

// Calculate distance
auto dist = distance(nums.begin(), it);  // 3
```

### `std::next` / `std::prev`

```cpp
#include <iterator>

vector<int> nums = {1, 2, 3, 4, 5};
auto it = nums.begin() + 2;  // Points to 3

auto next_it = next(it);     // Points to 4
auto prev_it = prev(it);     // Points to 2
auto next_2 = next(it, 2);   // Points to 5
```

---

## Quick Reference Table

| Function           | Header        | Purpose                      | Time Complexity |
| ------------------ | ------------- | ---------------------------- | --------------- |
| `accumulate`       | `<numeric>`   | Sum/reduce elements          | O(n)            |
| `min_element`      | `<algorithm>` | Find minimum                 | O(n)            |
| `max_element`      | `<algorithm>` | Find maximum                 | O(n)            |
| `sort`             | `<algorithm>` | Sort container               | O(n log n)      |
| `binary_search`    | `<algorithm>` | Search in sorted range       | O(log n)        |
| `lower_bound`      | `<algorithm>` | First position for insertion | O(log n)        |
| `upper_bound`      | `<algorithm>` | Last position for insertion  | O(log n)        |
| `count`            | `<algorithm>` | Count occurrences            | O(n)            |
| `find`             | `<algorithm>` | Find element                 | O(n)            |
| `reverse`          | `<algorithm>` | Reverse container            | O(n)            |
| `next_permutation` | `<algorithm>` | Generate next permutation    | O(n)            |
| `partial_sum`      | `<numeric>`   | Prefix sums                  | O(n)            |
| `iota`             | `<numeric>`   | Fill with sequence           | O(n)            |

## Common Patterns in Competitive Programming

### 1. Finding Second Largest Element

```cpp
vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};
sort(nums.begin(), nums.end());
int second_largest = nums[nums.size() - 2];  // After removing duplicates if needed
```

### 2. Checking if Array is Sorted

```cpp
bool is_sorted_asc = is_sorted(nums.begin(), nums.end());
bool is_sorted_desc = is_sorted(nums.begin(), nums.end(), greater<int>());
```

### 3. Rotating Array

```cpp
vector<int> nums = {1, 2, 3, 4, 5};
int k = 2;  // Rotate right by 2
rotate(nums.begin(), nums.begin() + nums.size() - k, nums.end());
// nums = {4, 5, 1, 2, 3}
```

### 4. Removing Duplicates

```cpp
vector<int> nums = {1, 1, 2, 2, 3, 3};
sort(nums.begin(), nums.end());  // If not already sorted
nums.erase(unique(nums.begin(), nums.end()), nums.end());
// nums = {1, 2, 3}
```

### 5. Prefix/Suffix Operations

```cpp
// Prefix maximum
vector<int> nums = {1, 3, 2, 5, 4};
vector<int> prefix_max(nums.size());
prefix_max[0] = nums[0];
for (int i = 1; i < nums.size(); i++) {
    prefix_max[i] = max(prefix_max[i-1], nums[i]);
}

// Using partial_sum with max
partial_sum(nums.begin(), nums.end(), prefix_max.begin(),
    [](int a, int b) { return max(a, b); });
```

## Performance Tips

1. **Use appropriate algorithms**: `nth_element` is faster than `sort` if you only need the kth element
2. **Reuse iterators**: Store `begin()` and `end()` if using them multiple times
3. **Consider containers**: `unordered_set` for O(1) lookups, `set` for ordered operations
4. **Lambda efficiency**: Capture by reference `[&]` when possible to avoid copies
5. **Memory allocation**: Use `reserve()` for vectors when you know the size

This cheat sheet covers the most commonly used algorithms in competitive programming and LeetCode problems!
