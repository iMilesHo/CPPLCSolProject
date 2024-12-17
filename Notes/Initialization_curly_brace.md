In C++, both syntaxes you provided for initializing arrays are correct and serve similar purposes, but they stem from different stages in the evolution of the language. Here's a bit more detail on each and how they relate to modern C++ practices:

1. **Direct-list-initialization (since C++11)**:
   ```cpp
   int arr[5] {1, 2, 3, 4, 5};
   ```
   This syntax uses curly braces `{}` for initialization and is known as direct-list-initialization. It was introduced with C++11 as part of the uniform initialization approach. This method is favored in modern C++ because it avoids most forms of implicit narrowing conversions, making it safer in contexts where type conversions could lead to data loss. It's applicable to all types of variables and can be used with arrays, objects, and primitives.

2. **Copy-list-initialization**:
   ```cpp
   int arr[5] = {1, 2, 3, 4, 5};
   ```
   This form is the traditional C++ array initialization and is perfectly valid. It uses an equals sign followed by curly braces. This was the standard way to initialize arrays before C++11. The equals sign here doesn't imply assignment but rather initialization. This form is still widely used and understood but can lead to ambiguities and inefficiencies in some cases when used with objects (not primitives or arrays), as it might invoke extra copy or move constructors prior to C++17.

**Other ways to use `{}` for initialization in C++**:
- **Aggregate initialization**: For structs and arrays where you can list out values in braces which directly correspond to the members or elements.
- **Uniform initialization for objects**: You can use curly braces to initialize objects of classes if appropriate constructors are available.
- **Initializer lists in functions**: Functions that take `std::initializer_list<T>` as a parameter can be called using curly braces to include multiple elements.

For example:
```cpp
struct Point {
    int x, y;
};

Point p {10, 20}; // Aggregate initialization

std::vector<int> vec {1, 2, 3, 4}; // Initializer list for constructor
```

**Choosing a method**:
- Prefer using `{}` (direct-list-initialization) in modern C++ as it provides more consistency and safety in type conversions.
- Use `=` when dealing with legacy code or when the context makes it clear and unambiguous, particularly for plain old data types like arrays.

Both methods you mentioned are standard and correct, but embracing `{}` without `=` aligns better with modern C++ best practices.