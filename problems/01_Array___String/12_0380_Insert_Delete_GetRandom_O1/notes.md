# Insert Delete GetRandom O(1)

- **ID:** 380
- **Difficulty:** MEDIUM
- **Topic Tags:** Array, Hash Table, Math, Design, Randomized
- **Link:** [LeetCode Problem](https://leetcode.com/problems/insert-delete-getrandom-o1/description/)

## Problem Description

## Approach

```cpp
class RandomizedSet {
public:
    set<int> randmoizedSet;
    RandomizedSet() {
        randmoizedSet = set<int>{};
    }

    bool insert(int val) {
        randmoizedSet.push_back(val);
    }

    bool remove(int val) {
        randmoizedSet.
    }

    int getRandom() {

    }
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */
```

## Complexity

- **Time Complexity:** O(1) for each operation (insert, remove, getRandom)
- **Space Complexity:** O(n) for storing all elements twice (vector + map)

Great — let’s do a systematic review of your solution attempt and then build it into a production-ready version you can confidently explain in interviews.

---

## 1. Time / Space Complexity, Readability, and Naming

### Issues in your draft:

- You used `set<int>` but then tried to call `push_back()`. A `std::set` is an ordered container with O(log n) operations, not O(1). Also, it doesn’t allow indexing, which you need for `getRandom()`.
- To achieve O(1), you must use a **combination** of:

  - **`std::vector<int>`**: stores values for O(1) random access.
  - **`std::unordered_map<int,int>`**: maps a value → its index in the vector for O(1) lookup/removal.

### Correct complexity:

- `insert`: O(1) average (amortized for vector growth).
- `remove`: O(1) average (swap with last element, pop_back, update map).
- `getRandom`: O(1) (uniform random index).
- Space: O(n) for storing all elements twice (vector + map).

---

## 2. Invariant Derivation & Proof

### Invariant:

> For every value in the set, its index in the vector is correctly recorded in the map.

- **Initialization**: empty set, invariant trivially holds.
- **Insert**: add to vector’s end, record its index in map → still consistent.
- **Remove**: swap target element with last element in vector, update the map for the swapped element, pop_back → invariant maintained.
- **getRandom**: uses vector size to pick valid random index → invariant ensures every stored value is accessible.

This invariant guarantees correctness and uniform random selection.

---

## 3. Idiomatic C++ & Style

### Key points:

- Prefer `std::unordered_map` for O(1).
- Use `std::uniform_int_distribution` (C++11) over `rand()` for modern randomness.
- Avoid redundant initialization (`randmoizedSet = set<int>{};` is unnecessary).
- Use **camelCase** or **snake_case** consistently; e.g., `randomizedSet` instead of `randmoizedSet` (typo).
- Add comments describing each step.

---

## 4. Interview Follow-Ups & Harder Variants

- **Design RandomizedCollection**: duplicates allowed. Needs `unordered_map<int, unordered_set<int>>`.
- **Weighted getRandom()**: return proportional to weight (prefix sums + binary search or alias method).
- **Thread-safety**: wrap with mutexes if concurrent.
- **Persistence / serialization**: saving and restoring state.
- **Scaling to disk**: if n is very large.

---

## 5. 30–90 Second Elevator Pitch

> “We want a data structure that can insert, remove, and return a random element all in O(1) time. A single container like a set or vector can’t do all three efficiently, so we combine them: a vector to support fast random indexing, and an unordered_map to support O(1) lookups and deletions. When we remove an element, we swap it with the last element in the vector, update the map, and pop_back, keeping everything consistent. This way, every operation is O(1) on average, and `getRandom` just picks a random index in the vector.”

---

## 6. Production-Ready Solution (C++)

```cpp
#include <vector>
#include <unordered_map>
#include <random>

class RandomizedSet {
private:
    std::vector<int> values;                       // store elements
    std::unordered_map<int, int> indexMap;         // val -> index in vector
    std::mt19937 gen;                              // random number generator

public:
    RandomizedSet() : gen(std::random_device{}()) {}

    bool insert(int val) {
        if (indexMap.count(val)) return false;
        values.push_back(val);
        indexMap[val] = values.size() - 1;
        return true;
    }

    bool remove(int val) {
        if (!indexMap.count(val)) return false;
        int idx = indexMap[val];
        int lastVal = values.back();
        values[idx] = lastVal;
        indexMap[lastVal] = idx;
        values.pop_back(); // remove last element
        indexMap.erase(val); // remove from map
        return true;
    }

    int getRandom() {
        std::uniform_int_distribution<> dist(0, values.size() - 1);
        return values[dist(gen)];
    }
};
```

---

## 7. Additional Optimizations & Perspectives

- **Memory**: `unordered_map` has overhead; tradeoff if `n` is small.
- **Security**: use `std::mt19937_64` if large key space.
- **Deterministic testing**: allow seeding RNG for unit tests.
- **Custom allocator**: if extremely high performance needed.

## 1. What Each Structure Gives Us

- **`std::vector<int> values`**

  - Stores all elements compactly.
  - Allows **O(1) random access** by index → essential for `getRandom()`.
  - Lets us pick a random element uniformly by generating an index between `0` and `values.size() - 1`.

- **`std::unordered_map<int,int> indexMap`**

  - Maps each value → its index in the vector.
  - Allows **O(1) lookup** to check if a value exists (`insert` check) or find where it lives (`remove`).
  - Keeps the vector and set representation in sync.

---

## 2. Why We Need Both Together

- If we only had a **vector**:

  - `insert` = O(1) (push_back).
  - `getRandom` = O(1).
  - `remove` = O(n) because we’d need to search linearly for the value. ❌

- If we only had a **map/set**:

  - `insert`/`remove` = O(1) (unordered_map) or O(log n) (set).
  - But `getRandom` would be **O(n)**, since you’d have to iterate through elements or build an index. ❌

👉 By **combining** the two:

- `insert`: vector push_back + update map → O(1).
- `remove`: map tells us the index instantly, we swap with last element in vector + update map → O(1).
- `getRandom`: random index in vector → O(1).

---

## 3. The Invariant They Maintain

At all times:

> Every element in the vector has an entry in the map pointing to its correct index.
> Every map entry points to the location of its element in the vector.

This invariant ensures that no matter what operation we do (insert, remove, random), both structures stay consistent.

---

## 4. Elevator Pitch for Why Both

> “The vector gives us fast random access for `getRandom`, and the hash map gives us fast lookups for `insert` and `remove`. Neither structure alone can achieve O(1) for all three operations, but together they complement each other perfectly.”

---

Do you want me to also **draw out a remove operation step-by-step with an example vector + map** so you can visualize how the swap-with-last trick works? That’s often the part interviewers press on.
