## C++ Enum Notes

### Unscoped Enums (Traditional)

```cpp
enum State { UNKNOWN = -1, BAD = 0, GOOD = 1 };
```

**Key Features:**

- **Scope Injection**: Enum constants are injected into the enclosing scope
- **Direct Access**: Can use `UNKNOWN` directly without qualification
- **Implicit Conversion**: Automatically converts to underlying type (usually `int`)
- **Backward Compatibility**: Compatible with C-style enums

**Usage Example:**

```cpp
class Solution {
public:
    enum State { UNKNOWN = -1, BAD = 0, GOOD = 1 };

    void function() {
        // ✅ All of these work
        int value = UNKNOWN;                    // Direct access
        vector<int> memo(10, UNKNOWN);          // Used in initialization
        if (memo[0] == BAD) { /* ... */ }       // Comparison
        State s = State::GOOD;                  // Qualified access also works
    }
};
```

### Scoped Enums (C++11+)

```cpp
enum class State { UNKNOWN = -1, BAD = 0, GOOD = 1 };
```

**Key Features:**

- **No Scope Injection**: Constants stay within enum scope
- **Qualified Access Required**: Must use `State::UNKNOWN`
- **Type Safety**: No implicit conversion to underlying type
- **Better Encapsulation**: Prevents naming conflicts

**Usage Example:**

```cpp
class Solution {
public:
    enum class State { UNKNOWN = -1, BAD = 0, GOOD = 1 };

    void function() {
        // ❌ These don't work
        // int value = UNKNOWN;              // Compilation error
        // vector<int> memo(10, UNKNOWN);    // Compilation error

        // ✅ Correct usage
        State s = State::UNKNOWN;
        int value = static_cast<int>(State::UNKNOWN);
        vector<int> memo(10, static_cast<int>(State::UNKNOWN));
    }
};
```

### Comparison Table

| Feature          | Unscoped Enum                  | Scoped Enum                  |
| ---------------- | ------------------------------ | ---------------------------- |
| Declaration      | `enum Name { ... }`            | `enum class Name { ... }`    |
| Access           | `CONSTANT` or `Name::CONSTANT` | `Name::CONSTANT` only        |
| Type Safety      | Weak (implicit conversion)     | Strong (explicit conversion) |
| Scope Pollution  | Yes                            | No                           |
| Naming Conflicts | Possible                       | Avoided                      |
| C Compatibility  | Yes                            | No                           |

### Best Practices

1. **Prefer scoped enums** for new code (better type safety)
2. **Use unscoped enums** when you need frequent direct access and backward compatibility
3. **Specify underlying type** explicitly when needed:
   ```cpp
   enum class ErrorCode : int { SUCCESS = 0, FAILURE = -1 };
   ```
4. **Use meaningful names** for both enum and constants
5. **Group related constants** logically

### Common Use Cases

- **State machines**: `enum State { IDLE, RUNNING, STOPPED }`
- **Error codes**: `enum class Result { SUCCESS, FAILED, TIMEOUT }`
- **Memoization states**: `enum { UNKNOWN = -1, BAD = 0, GOOD = 1 }`
- **Configuration flags**: `enum Options { VERBOSE = 1, DEBUG = 2, QUIET = 4 }`
