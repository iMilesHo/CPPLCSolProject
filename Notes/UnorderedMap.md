## **How to Insert into `std::unordered_map` in C++**

### 1. **Using `operator[]`**

```cpp
std::unordered_map<std::string, int> scores;
std::string studentName = "Alice";
int studentScore = 90;

scores[studentName] = studentScore;
```

- If `studentName` is not present, adds a new key-value pair.
  - for the `int` type, it initializes the value to `0` if the key does not exist.
- If it exists, updates the value.

---

### 2. **Using `insert`**

```cpp
std::unordered_map<std::string, int> scores;
std::string studentName = "Bob";
int studentScore = 85;

auto result = scores.insert({studentName, studentScore});
if (result.second) {
    std::cout << "Inserted " << studentName << "\n";
} else {
    std::cout << studentName << " already exists with value " << result.first->second << "\n";
}
```

- Inserts only if the key does **not** already exist.
- the `insert` method returns a pair:
  - `first`: iterator to the inserted or existing element.
  - `second`: boolean indicating if the insertion took place (`true` if inserted, `false` if key already existed).

---

### 3. **Using `emplace`**

```cpp
std::unordered_map<std::string, std::vector<int>> grades;
std::string subject = "Math";
std::vector<int> subjectScores = {88, 92, 95};

grades.emplace(subject, subjectScores);
```

- Like `insert`, but constructs the value in-place—useful for expensive-to-copy types.
- If the key already exists, it does not overwrite the existing value.

---

### 4. **Copy vs. Move Example**

```cpp
std::unordered_map<std::string, std::vector<int>> data;
std::string key = "Science";
std::vector<int> value = {75, 80};

data.insert({key, value}); // Copy
data.insert({std::move(key), std::move(value)}); // Move if you want to transfer ownership (use with care)
```

- **Copy**: Pass variables as-is.
- **Move**: Use `std::move()` to transfer ownership, avoiding copies.

---

## **Different Ways to Return a `std::vector` in C++**

### 1. **Return via Initializer List with Variables**

```cpp
int a = 1, b = 2, c = 3;
return std::vector<int>{a, b, c};
```

- Constructs and returns a vector using variables.

---

### 2. **Return a Local Variable**

```cpp
std::vector<int> getSquares(int n) {
    std::vector<int> result;
    for (int i = 1; i <= n; ++i) {
        int square = i * i;
        result.push_back(square);
    }
    return result;
}
```

- Build up a vector using variables, return it at the end.

---

### 3. **Return a Value from Another Function**

```cpp
std::vector<int> generateSeries(int n) {
    std::vector<int> series(n);
    for (int i = 0; i < n; ++i) series[i] = i;
    return series;
}

std::vector<int> getModifiedSeries(int n) {
    std::vector<int> data = generateSeries(n);
    for (int& x : data) x += 10; // Modify with a variable
    return data;
}
```

- Receive a vector from another function, modify or use as needed, then return.

---

## **How to Return an Array in C++**

### 1. **Return a `std::vector`**

```cpp
int x = 5, y = 10, z = 15;
return std::vector<int>{x, y, z};
```

### 2. **Return a `std::array` (fixed size)**

```cpp
int x = 100, y = 200, z = 300;
return std::array<int, 3>{x, y, z};
```

### 3. **Return a Pointer (not recommended in modern C++)**

```cpp
int a = 7, b = 8, c = 9;
static int arr[3];
arr[0] = a; arr[1] = b; arr[2] = c;
return arr; // Use only if necessary, and be aware of static storage!
```

### 4. **Fill an Array Passed by Reference**

```cpp
void fillArray(int (&arr)[3], int a, int b, int c) {
    arr[0] = a;
    arr[1] = b;
    arr[2] = c;
}
```

- The caller provides an array and values to fill.

---

## **Summary Table**

| Method       | Overwrites Existing? | Only Inserts If Absent? | In-place Construction? | Example                                       |
| ------------ | -------------------- | ----------------------- | ---------------------- | --------------------------------------------- |
| `operator[]` | Yes                  | No                      | No                     | `scores[studentName] = studentScore;`         |
| `insert`     | No                   | Yes                     | No                     | `scores.insert({studentName, studentScore});` |
| `emplace`    | No                   | Yes                     | Yes                    | `grades.emplace(subject, subjectScores);`     |

---

If you want tailored examples for any other STL container or a deeper explanation, just let me know!
