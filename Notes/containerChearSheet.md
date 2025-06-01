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

With these charts, you have quick references to frequently used STL containers and their core functionalities, aiding efficient coding practice and effective preparation for interviews.
