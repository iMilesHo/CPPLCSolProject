Here's a structured and concise cheat sheet for common interfaces (member functions) shared across STL containers in C++, followed by specific sheets for unique functions for each container.

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
