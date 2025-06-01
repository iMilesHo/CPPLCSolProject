### ✅ What is **Time Complexity**?

**Time complexity** describes **how the running time of an algorithm increases** as the **input size (n)** grows.

- It helps estimate **performance** and **scalability**.
- Expressed using **Big O notation** (e.g., O(n), O(n²), O(log n)).

---

### ✅ How to Explain It in an Interview (Concise + Clear):

> "**Time complexity is a way to measure how the runtime of an algorithm scales with the size of the input.** It helps evaluate the efficiency of an algorithm, especially for large datasets."

You can add:

> "For example, if a loop runs once for each element in an array of size n, it’s O(n). If we have nested loops, it might be O(n²), depending on how they’re structured."

---

### ✅ How to Say Big O Notations Clearly:

| Notation     | How to Say It                       | Example                                           |
| ------------ | ----------------------------------- | ------------------------------------------------- |
| `O(1)`       | “Constant time”                     | No matter the input size, runtime stays the same. |
| `O(n)`       | “Oh of n” or “Linear time”          | One loop over all elements.                       |
| `O(n²)`      | “Oh of n squared”                   | Two nested loops.                                 |
| `O(n³)`      | “Oh of n cubed”                     | Three nested loops.                               |
| `O(n⁴)`      | “Oh of n to the fourth”             | Four nested loops.                                |
| `O(n log n)` | “Oh of n log n”                     | Common in efficient sorting algorithms.           |
| `O(log n)`   | “Logarithmic time” or “Oh of log n” | Binary search.                                    |

---

### ✅ Sample Interview Phrase:

> “This algorithm has a time complexity of O(n log n), because we divide the input recursively and merge results—like merge sort.”

Want a quick practice question to try describing time complexity?
