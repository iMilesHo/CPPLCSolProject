
In C++, where an object’s storage lives depends on how you define and create it. Broadly speaking, you’ve got three common regions:

1. **Automatic storage (“the stack”)**
    
2. **Dynamic storage (“the heap” or free store”)**
    
3. **Static storage** (globals, function‐static variables, string literals, etc.)
    

---

## **1. Automatic storage (“the stack”)**

- **What lives here?**
    
    - Local variables declared inside a function (including function parameters).
        
    - Temporary objects (e.g. the result of an expression that’s an r-value, if bound to a reference with automatic lifetime).
        
    
- **How it works:**
    
    - When you enter a function, the runtime “pushes” a stack frame. Each automatic variable gets an offset in that frame.
        
    - When you leave the function (return or exception), its stack frame is “popped” and all those objects are destroyed and their memory reclaimed.
        
    
- **Example:**
    

```
void foo() {
    int x = 42;          // `x` lives on the stack
    std::string s = "Hi"; // the std::string object lives on the stack…
                          // but see note below about its internal buffer
} // at the closing brace, `s` is destroyed, then `x`
```

  

  

> **Note on std::string**

> The std::string object itself is automatic here (so its control block lives on the stack), but its _character buffer_ may be on the heap if the string is too large for the “small-string optimization” (SSO). With SSO, short strings (e.g. ≤15 chars on many implementations) are stored entirely inside the std::string’s own bytes, so no heap allocation is needed.

---

## **2. Dynamic storage (“the heap” or free store)**

- **What lives here?**
    
    - Anything you allocate with new/new[] (or std::allocator, std::make_shared, etc.)
        
    - Memory obtained via C APIs like malloc().
        
    
- **How it works:**
    
    - You explicitly ask for it; the runtime gives you a pointer.
        
    - It stays alive until you delete/delete[] it (or, if using smart pointers, until the last owner releases it).
        
    - The heap is generally larger but slower (both allocation and access) than the stack.
        
    
- **Example:**
    

```
void bar() {
    int* p = new int(123);     // control block for `123` is on the heap
    std::string* sp = new std::string("Hello, heap!");
    // …
    delete p;                   // you MUST free what you allocate
    delete sp;
}
```

  

---

## **3. Static storage**

- **What lives here?**
    
    - Global variables, namespace‐scope variables.
        
    - static variables inside functions or classes.
        
    - String literals (e.g. "World" in your code).
        
    
- **Lifetime:**
    
    - They are constructed before main() starts (roughly) and destroyed after main() ends.
        
    
- **Example:**
    

```
static int counter = 0;    // lives for the entire program
const char* msg = "Hi!";   // the literal “Hi!” resides in static storage
```

  

---

### **Putting it together for your** 

### **Wrapper w1(s);**

###  **example**

```
std::string s = "World";     // `s` is automatic, on the stack
Wrapper w1(s);               // `w1` is automatic, on the stack
                              // —but inside w1, `data` (the std::string)
                              //  will allocate on the heap if “World” exceeds
                              //  the SSO capacity.
```

1. **w1 object** lives in the caller’s stack frame.
    
2. **Copy-constructing data** allocates (heap) storage if needed, or uses SSO (stack‐within‐the‐object) otherwise.
    
3. When main() returns, w1 is destroyed (its std::string destructor frees any heap buffer), then s is destroyed.
    

---

### **Quick decision guide**

|**You write…**|**Storage location**|**Lifetime**|
|---|---|---|
|T x; (local)|Stack (automatic)|Until end of enclosing scope/function|
|new T|Heap|Until you delete it (or smart ptr drop)|
|static T x; or global|Static region|Whole program duration|

Hope this clarifies how and where C++ stores your variables!