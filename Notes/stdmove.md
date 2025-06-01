```cpp
std::string s1 = "hello";
std::string s2 = std::move(s1); // s1 is now an rvalue, resources moved to s2
```

---

## What is `s1` after `std::move(s1)`?

- **s1 itself is NOT an rvalue**—it is still a named variable (an lvalue).
- `std::move(s1)` **casts** `s1` to an rvalue reference **just for that one expression**.
- After the move, `s1` is in a **valid but unspecified state**.

---

### What does "valid but unspecified" mean?

- `s1` is still a valid `std::string` object—you can assign a new value to it, call methods, or even check if it’s empty.
- But its **content is unspecified**. Most likely, it’s now empty (`""`), but **you should not rely on it**.
- You can safely do:

  ```cpp
  s1 = "again";  // This works, s1 can be reused
  ```

---

### Example Demo

```cpp
std::string s1 = "hello";
std::string s2 = std::move(s1);

std::cout << "s1: '" << s1 << "'\n"; // Might print empty string
std::cout << "s2: '" << s2 << "'\n"; // Prints 'hello'
```

**Output:**

```
s1: ''
s2: 'hello'
```

But don’t depend on `s1` always being empty—only that it’s safe to use or assign to.

---

## **Summary Table**

| After `std::move(s1)` | Description                                                    |
| --------------------- | -------------------------------------------------------------- |
| s1 is                 | Still a valid string, but content is unspecified (often empty) |
| s2 is                 | Owns the original data (`"hello"`)                             |
| Can I reuse s1?       | Yes, by assigning a new value or using it like normal          |

---

**TL;DR:**
After `std::move(s1)`, `s1` can be reused, but its content is unspecified. It is **not** an rvalue—it’s just a regular variable that’s had its content “moved from.”

Let me know if you want a visualization or more about move semantics!
