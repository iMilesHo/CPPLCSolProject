# C++ Container Emplace Operations Cheat Sheet

## Overview

`emplace` functions construct objects **in-place** directly in the container's memory, avoiding temporary object creation and unnecessary copies/moves.

**Key Benefit**: More efficient than `push`/`insert` + constructor, especially for complex objects.

---

## 1. Vector Operations

### `emplace_back()`

```cpp
std::vector<std::string> vec;

// ❌ Less efficient - creates temporary string
vec.push_back(std::string("Hello"));

// ✅ More efficient - constructs string in-place
vec.emplace_back("Hello");
vec.emplace_back(5, 'A');           // "AAAAA"
vec.emplace_back();                 // Default-constructed empty string

// Complex objects
struct Point { int x, y; Point(int x, int y) : x(x), y(y) {} };
std::vector<Point> points;

points.push_back(Point(1, 2));      // ❌ Creates temporary Point
points.emplace_back(1, 2);          // ✅ Constructs Point in-place
```

### `emplace()`

```cpp
std::vector<std::string> vec{"A", "C"};

// Insert at position
auto it = vec.emplace(vec.begin() + 1, "B");  // vec = {"A", "B", "C"}
vec.emplace(vec.end(), 3, 'X');               // vec = {"A", "B", "C", "XXX"}
```

---

## 2. Deque Operations

### `emplace_back()` & `emplace_front()`

```cpp
std::deque<std::pair<int, std::string>> dq;

dq.emplace_back(1, "first");        // Add to back
dq.emplace_front(0, "zero");        // Add to front
// dq = [(0,"zero"), (1,"first")]

dq.emplace_back(std::piecewise_construct,
                std::forward_as_tuple(2),
                std::forward_as_tuple(5, 'A'));  // Advanced construction
```

### `emplace()`

```cpp
std::deque<int> dq{1, 3, 5};
auto it = dq.emplace(dq.begin() + 1, 2);  // dq = {1, 2, 3, 5}
```

---

## 3. List Operations

### `emplace_back()`, `emplace_front()`, `emplace()`

```cpp
std::list<std::vector<int>> lst;

lst.emplace_back();                 // Empty vector
lst.emplace_back(3, 42);           // Vector with 3 elements of value 42
lst.emplace_front(5, 1);           // Vector with 5 elements of value 1

auto it = lst.begin();
++it;
lst.emplace(it, std::initializer_list<int>{1, 2, 3});  // Insert vector {1,2,3}
```

---

## 4. Set Operations

### `emplace()` & `emplace_hint()`

```cpp
std::set<std::string> s;

// Basic emplace
auto [it1, inserted1] = s.emplace("Hello");
auto [it2, inserted2] = s.emplace(5, 'A');    // "AAAAA"

// With hint for better performance
auto hint = s.end();
auto it3 = s.emplace_hint(hint, "World");

// Custom objects
struct Person {
    std::string name;
    int age;
    Person(const std::string& n, int a) : name(n), age(a) {}
    bool operator<(const Person& other) const { return age < other.age; }
};

std::set<Person> people;
people.emplace("Alice", 25);
people.emplace("Bob", 30);
```

### `multiset`

```cpp
std::multiset<std::pair<int, std::string>> ms;

ms.emplace(1, "one");
ms.emplace(std::piecewise_construct,
           std::forward_as_tuple(2),
           std::forward_as_tuple(3, 'X'));  // (2, "XXX")
```

---

## 5. Map Operations

### `emplace()`

```cpp
std::map<int, std::string> m;

// Basic emplace
auto [it1, inserted1] = m.emplace(1, "one");
auto [it2, inserted2] = m.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(2),
                                  std::forward_as_tuple(5, 'A'));

// Complex value types
std::map<std::string, std::vector<int>> complexMap;
complexMap.emplace("key1", std::vector<int>{1, 2, 3});
complexMap.emplace(std::piecewise_construct,
                   std::forward_as_tuple("key2"),
                   std::forward_as_tuple(5, 42));  // vector with 5 elements of 42
```

### `emplace_hint()`

```cpp
std::map<int, std::string> m{{1, "one"}, {3, "three"}};

auto hint = m.find(1);
auto it = m.emplace_hint(hint, 2, "two");  // Efficient insertion
```

### `try_emplace()` (C++17)

```cpp
std::map<int, std::string> m;

// Only emplaces if key doesn't exist
auto [it1, inserted1] = m.try_emplace(1, "one");      // Inserted
auto [it2, inserted2] = m.try_emplace(1, "ONE");      // Not inserted
auto [it3, inserted3] = m.try_emplace(2, 5, 'A');     // Inserted: "AAAAA"
```

### `multimap`

```cpp
std::multimap<int, std::string> mm;

mm.emplace(1, "first");
mm.emplace(1, "second");           // Multiple values for same key
auto it = mm.emplace_hint(mm.end(), 2, "two");
```

---

## 6. Unordered Containers

### `unordered_set`

```cpp
std::unordered_set<std::string> us;

us.emplace("Hello");
us.emplace(5, 'A');                // "AAAAA"

// With hint
auto it = us.emplace_hint(us.begin(), "World");
```

### `unordered_map`

```cpp
std::unordered_map<int, std::vector<int>> um;

um.emplace(1, std::vector<int>{1, 2, 3});
um.emplace(std::piecewise_construct,
           std::forward_as_tuple(2),
           std::forward_as_tuple(3, 42));

// try_emplace (C++17)
auto [it, inserted] = um.try_emplace(3, 5, 100);  // vector with 5 elements of 100
```

---

## 7. Stack & Queue Operations

### `stack`

```cpp
std::stack<std::pair<int, std::string>> st;

st.emplace(1, "one");
st.emplace(std::piecewise_construct,
           std::forward_as_tuple(2),
           std::forward_as_tuple(3, 'A'));
```

### `queue`

```cpp
std::queue<std::vector<int>> q;

q.emplace();                       // Empty vector
q.emplace(3, 42);                  // Vector with 3 elements of 42
q.emplace(std::initializer_list<int>{1, 2, 3});
```

### `priority_queue`

```cpp
std::priority_queue<std::pair<int, std::string>> pq;

pq.emplace(1, "low");
pq.emplace(10, "high");
pq.emplace(std::piecewise_construct,
           std::forward_as_tuple(5),
           std::forward_as_tuple(4, 'M'));
```

---

## 8. Advanced Techniques

### Piecewise Construction

```cpp
// For pair/tuple construction when arguments need forwarding
std::map<std::string, std::vector<int>> m;

m.emplace(std::piecewise_construct,
          std::forward_as_tuple("key"),           // Key construction args
          std::forward_as_tuple(5, 42));          // Value construction args
```

### Perfect Forwarding with Custom Classes

```cpp
class MyClass {
public:
    MyClass(int a, const std::string& b, double c) : a_(a), b_(b), c_(c) {}
private:
    int a_;
    std::string b_;
    double c_;
};

std::vector<MyClass> vec;
vec.emplace_back(42, "hello", 3.14);  // Perfect forwarding to MyClass constructor
```

### Move-Only Types

```cpp
std::vector<std::unique_ptr<int>> vec;

// ❌ Won't compile - unique_ptr is move-only
// vec.push_back(std::unique_ptr<int>(new int(42)));

// ✅ Works - constructs unique_ptr in-place
vec.emplace_back(new int(42));
vec.emplace_back(std::make_unique<int>(42));  // Even better
```

---

## 9. Performance Comparison

```cpp
#include <chrono>
#include <vector>
#include <string>

// Test class to measure constructor calls
class TestClass {
public:
    TestClass(const std::string& s) : data(s) {
        std::cout << "Constructor called\n";
    }
    TestClass(const TestClass& other) : data(other.data) {
        std::cout << "Copy constructor called\n";
    }
    TestClass(TestClass&& other) : data(std::move(other.data)) {
        std::cout << "Move constructor called\n";
    }
private:
    std::string data;
};

void performance_test() {
    std::vector<TestClass> vec;

    // ❌ Creates temporary + move
    vec.push_back(TestClass("test"));    // Constructor + Move constructor

    // ✅ Direct construction
    vec.emplace_back("test");            // Constructor only
}
```

---

## 10. Common Pitfalls

### 1. Forgetting Return Values

```cpp
std::set<int> s;

// ❌ Ignoring return value
s.emplace(42);

// ✅ Check if insertion happened
auto [it, inserted] = s.emplace(42);
if (inserted) {
    std::cout << "Inserted successfully\n";
}
```

### 2. Wrong Argument Types

```cpp
std::vector<std::string> vec;

// ❌ Wrong - creates char array
vec.emplace_back({'H', 'e', 'l', 'l', 'o'});

// ✅ Correct ways
vec.emplace_back("Hello");
vec.emplace_back(5, 'A');
```

### 3. Exception Safety

```cpp
std::vector<std::unique_ptr<int>> vec;

// ❌ Potential memory leak if vector reallocation throws
vec.emplace_back(new int(42));

// ✅ Exception safe
auto ptr = std::make_unique<int>(42);
vec.emplace_back(std::move(ptr));
// or directly:
vec.emplace_back(std::make_unique<int>(42));
```

---

## 11. Best Practices

### ✅ DO:

- Use `emplace` for complex object construction
- Prefer `emplace` over `push`/`insert` for performance-critical code
- Use `try_emplace` for maps when you don't want to overwrite existing keys
- Check return values for associative containers

### ❌ DON'T:

- Use `emplace` with simple types where `push` is equally efficient
- Ignore exception safety with raw pointers
- Forget that `emplace` can still trigger reallocations in vectors

### 🎯 When to Use:

- **Always**: When constructing complex objects
- **Consider**: When performance is critical
- **Skip**: For simple types like `int`, `double` where construction cost is minimal

---

## 12. Quick Reference

| Container        | Methods                                          | Notes                   |
| ---------------- | ------------------------------------------------ | ----------------------- |
| `vector`         | `emplace_back()`, `emplace()`                    | Most common use case    |
| `deque`          | `emplace_back()`, `emplace_front()`, `emplace()` | Front/back insertion    |
| `list`           | `emplace_back()`, `emplace_front()`, `emplace()` | Any position            |
| `set/multiset`   | `emplace()`, `emplace_hint()`                    | Returns iterator + bool |
| `map/multimap`   | `emplace()`, `emplace_hint()`, `try_emplace()`   | Key-value construction  |
| `unordered_*`    | Same as ordered versions                         | Hash-based containers   |
| `stack/queue`    | `emplace()`                                      | Adapter containers      |
| `priority_queue` | `emplace()`                                      | Heap-based container    |

Remember: **`emplace` = construct in-place = better performance for complex objects!**
