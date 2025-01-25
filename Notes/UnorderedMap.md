## **Inserting a New Value into an Unordered Map in C++**

- **`operator[]`**  
  ```cpp
  std::unordered_map<Key, Value> myMap;
  myMap[someKey] = someValue;
  ```
  - If `someKey` doesn’t exist in the map, this creates a new entry with `someKey` as the key and `someValue` as the value.
  - If `someKey` already exists, its associated value gets overwritten.

- **`insert`**  
  ```cpp
  myMap.insert({someKey, someValue});
  ```
  - Tries to insert a new key-value pair only if `someKey` doesn’t exist yet.
  - If the key is already present, nothing is changed.  
  - Returns a `std::pair<iterator, bool>` indicating the position of the inserted or existing element, and whether the insertion took place.
  - by default `myMap.insert({someKey, someValue});` will construct (via copy or move) a new `std::pair<Key, Value>` from `someKey` and `someValue`, then attempt to insert that pair into the map. If the key doesn’t already exist, it inserts the new pair; otherwise, nothing changes. 

- **Copy vs. Move**  
  - If `someKey` or `someValue` are lvalues (e.g., named variables), insertion happens by copy.  
  - If they are rvalues (e.g., temporary objects) and the types have valid move constructors, the map can move-construct instead of copy-construct.  
- **If you want more control** over how the object is constructed (e.g., to avoid unnecessary copies), consider using `emplace`.

- **`emplace`**  
  ```cpp
  myMap.emplace(someKey, someValue);
  ```
  - Similar to `insert`, but constructs the value in-place. Can be more efficient if `Value` is expensive to copy.

In most cases, using `operator[]` is succinct and straightforward. However, if you want to avoid overwriting existing data, `insert` or `emplace` is preferable.

## **Different Ways to Return a `std::vector` in C++**

1. **Return by directly constructing in-place**  
   ```cpp
   return {x, y}; 
   ```
   - Constructs a temporary `std::vector<int>` with the given elements.
   - Requires C++11 or later (initializer list syntax).

2. **Return a named local variable**  
   ```cpp
   std::vector<int> result;
   // ... fill result ...
   return result;
   ```
   - Common pattern: build a local `std::vector<int>` and return it at the end.

3. **Return a pre-existing `std::vector`**  
   ```cpp
   std::vector<int> helperFunction();
   
   std::vector<int> myFunction() {
       std::vector<int> vec = helperFunction();
       // ... optional processing ...
       return vec;
   }
   ```
   - Return a `std::vector<int>` that was received or created elsewhere.

In modern C++ (C++17 or later), **return value optimization (RVO)** and **move semantics** ensure that returning a `std::vector<int>` generally does not involve extra copies, making these approaches both clean and efficient.