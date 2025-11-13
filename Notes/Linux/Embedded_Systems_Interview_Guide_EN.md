# Embedded Systems & C/C++ Interview Preparation Guide

## 1. What is Memory Mapping?

**Answer:** Memory mapping is a mechanism that maps a block of memory into different process address spaces, allowing multiple processes to share the same physical memory region. This enables efficient inter-process communication (IPC) and reduces memory overhead by avoiding data duplication.

**Key Points:**

- Maps files or devices into memory address space
- Enables shared memory between processes
- Used for efficient file I/O and IPC
- Implemented via `mmap()` system call in Linux

---

## 2. What is the Difference Between #define and const?

**Answer:**

| Aspect               | #define                                       | const                                     |
| -------------------- | --------------------------------------------- | ----------------------------------------- |
| **Nature**           | Preprocessor directive for symbolic constants | C/C++ keyword for read-only variables     |
| **Processing Stage** | Preprocessor stage (before compilation)       | Compilation stage                         |
| **Type Checking**    | No type checking, simple text replacement     | Type-safe, associated with variable type  |
| **Memory**           | No memory allocation                          | Allocates memory                          |
| **Debugging**        | Harder to debug (no symbol in debugger)       | Easier to debug (appears in symbol table) |
| **Scope**            | No scope rules                                | Follows scope rules                       |

**Example:**

```c
#define MAX_SIZE 100        // Text substitution
const int MAX_SIZE = 100;   // Typed constant variable
```

---

## 3. What is the Difference Between Arrays and Linked Lists?

**Answer:**

| Feature                | Array                             | Linked List                      |
| ---------------------- | --------------------------------- | -------------------------------- |
| **Memory Layout**      | Contiguous memory                 | Non-contiguous memory            |
| **Access Time**        | O(1) random access                | O(n) sequential access           |
| **Insertion/Deletion** | O(n) - requires shifting elements | O(1) - only pointer manipulation |
| **Memory Overhead**    | No extra overhead                 | Extra memory for pointers        |
| **Cache Performance**  | Better (spatial locality)         | Worse (scattered in memory)      |
| **Size**               | Fixed size (static arrays)        | Dynamic size                     |

**Use Cases:**

- **Arrays:** Fast access, fixed size, cache-friendly operations
- **Linked Lists:** Frequent insertions/deletions, dynamic size requirements

---

## 4. What is the Difference Between Pointers and References?

**Answer:**

| Aspect           | Pointer                                  | Reference                                    |
| ---------------- | ---------------------------------------- | -------------------------------------------- |
| **Definition**   | Variable that stores memory address      | Alias for an existing variable               |
| **Null Value**   | Can be NULL                              | Cannot be null, must be initialized          |
| **Reassignment** | Can be reassigned to different addresses | Cannot be changed after initialization       |
| **Memory**       | Requires separate memory for address     | No separate memory (compiler implementation) |
| **Syntax**       | Uses `*` for dereferencing               | Transparent access (no special operator)     |
| **Indirection**  | Multiple levels possible (`**p`)         | Only one level                               |

**Example:**

```cpp
int x = 10;
int* ptr = &x;      // Pointer - can be reassigned
int& ref = x;       // Reference - cannot be reassigned

ptr = nullptr;      // Valid
// ref = nullptr;   // Invalid - references cannot be null
```

---

## 5. What is the Difference Between Process and Thread?

**Answer:**

| Aspect                | Process                                       | Thread                                         |
| --------------------- | --------------------------------------------- | ---------------------------------------------- |
| **Definition**        | Independent program with its own memory space | Lightweight unit of execution within a process |
| **Memory**            | Separate address space                        | Shares address space with other threads        |
| **Communication**     | IPC required (pipes, sockets, shared memory)  | Direct access to shared memory                 |
| **Creation Overhead** | Expensive (fork/exec)                         | Cheap (thread creation)                        |
| **Context Switch**    | Slower (more state to save/restore)           | Faster (less overhead)                         |
| **Isolation**         | Isolated from other processes                 | Shares resources, less isolated                |
| **Failure Impact**    | Crash doesn't affect other processes          | Crash can bring down entire process            |

**Key Points:**

- **Process:** Own memory, heap, stack, file descriptors
- **Thread:** Shares code, data, heap; has own stack and registers
- **Use Cases:**
  - Processes for isolation and security
  - Threads for performance and resource sharing

**Example:**

```c
// Process creation
pid_t pid = fork();
if (pid == 0) {
    // Child process
    execl("/bin/ls", "ls", NULL);
}

// Thread creation
pthread_t thread;
pthread_create(&thread, NULL, thread_function, NULL);
```

---

## 6. What is a Memory Leak and How Do You Detect/Prevent It?

**Answer:**

### Definition:

A memory leak occurs when dynamically allocated memory is not properly deallocated, causing the program to gradually consume more memory over time, potentially leading to system slowdown or crashes.

### Common Causes:

1. **Forgetting to free allocated memory:**

```c
void processData() {
    int* data = (int*)malloc(100 * sizeof(int));
    // Process data...
    // MEMORY LEAK - forgot to call free(data)
}
```

2. **Lost pointer reference:**

```c
int* ptr = (int*)malloc(sizeof(int));
ptr = NULL;  // MEMORY LEAK - lost reference, can't free
```

3. **Exception/Early Return:**

```c
void function() {
    char* buffer = (char*)malloc(1024);
    if (error_condition) {
        return;  // MEMORY LEAK - exits without freeing
    }
    free(buffer);
}
```

### Detection Methods:

1. **Valgrind (Linux):**

```bash
valgrind --leak-check=full ./your_program
```

2. **AddressSanitizer:**

```bash
gcc -fsanitize=address -g program.c -o program
```

3. **Static Analysis Tools:**
   - Cppcheck
   - Clang Static Analyzer
   - Coverity

### Prevention Strategies:

1. **Use RAII in C++ (Resource Acquisition Is Initialization):**

```cpp
{
    std::unique_ptr<int[]> data(new int[100]);
    // Automatically freed when going out of scope
}
```

2. **Pair allocations with deallocations:**

```c
void* allocate() {
    return malloc(size);
}
void deallocate(void* ptr) {
    free(ptr);
}
```

3. **Set pointers to NULL after freeing:**

```c
free(ptr);
ptr = NULL;  // Prevents double-free and use-after-free
```

4. **Use smart pointers in C++:**

```cpp
std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
std::unique_ptr<int[]> array = std::make_unique<int[]>(100);
```

**Best Practices:**

- Always match `malloc`/`new` with `free`/`delete`
- Use automatic memory management when possible
- Regularly run memory analysis tools during development
- Implement proper error handling to ensure cleanup

---

## 7. Describe the MQTT Communication Process

**Answer:**

1. **Client Creation:** Initialize MQTT client instance
2. **Broker Configuration:** Specify broker IP address and port (default: 1883)
3. **Connection Establishment:** Connect to MQTT broker with credentials
4. **Topic Management:**
   - **Publish:** Send messages to specific topics
   - **Subscribe:** Register interest in topics to receive messages
5. **Data Transmission:** Exchange messages through the broker
6. **Disconnection:** Gracefully close the connection

**Key Features:**

- Lightweight publish/subscribe protocol
- Quality of Service (QoS) levels: 0, 1, 2
  - QoS 0: At most once (no acknowledgment)
  - QoS 1: At least once (acknowledged delivery)
  - QoS 2: Exactly once (two-phase acknowledgment)
- Retained messages and last will
- Ideal for IoT and resource-constrained devices

---

## 8. How to Implement Synchronization in Linux?

**Answer:**

### Three Main Mechanisms:

1. **Mutex (Mutual Exclusion Lock)**

   - Provides exclusive access to shared resources
   - Only one thread can hold the mutex at a time
   - Functions: `pthread_mutex_lock()`, `pthread_mutex_unlock()`

2. **Semaphore**

   - Counting synchronization primitive
   - Controls access to resources with limited capacity
   - Binary semaphore vs. Counting semaphore
   - Functions: `sem_wait()`, `sem_post()`

3. **Condition Variable**
   - Allows threads to wait for specific conditions
   - Must be used with mutex
   - Avoids busy-waiting (polling)
   - Functions: `pthread_cond_wait()`, `pthread_cond_signal()`

**Example:**

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&mutex);
// Critical section
pthread_mutex_unlock(&mutex);
```

---

## 9. What are the Application Scenarios for TCP and UDP?

**Answer:**

### TCP (Transmission Control Protocol):

- **Characteristics:** Connection-oriented, reliable, ordered delivery
- **Use Cases:**
  - File transfer (FTP, HTTP)
  - Email (SMTP, POP3, IMAP)
  - Web browsing (HTTP/HTTPS)
  - Remote administration (SSH, Telnet)
  - Database connections

### UDP (User Datagram Protocol):

- **Characteristics:** Connectionless, unreliable, low latency
- **Use Cases:**
  - Real-time audio/video streaming (VoIP, video conferencing)
  - Online gaming (where speed > reliability)
  - DNS queries
  - DHCP
  - Real-time monitoring and telemetry
  - IoT sensor data

**Selection Criteria:** Use TCP when reliability is critical; use UDP when low latency and speed are prioritized over guaranteed delivery.

---

## 10. What is a Dangling Pointer and When Does It Occur?

**Answer:**

### Definition:

A dangling pointer is a pointer that references memory that has been freed, deallocated, or is otherwise invalid.

### Common Scenarios:

1. **Memory Freed but Pointer Not Nullified:**

```c
int* ptr = (int*)malloc(sizeof(int));
free(ptr);
*ptr = 10;  // DANGLING POINTER - accessing freed memory
```

2. **Returning Pointer to Local Variable:**

```c
int* getIntPointer() {
    int num = 5;
    return &num;  // Returns address of local variable
}
int* ptr = getIntPointer();
*ptr = 10;  // DANGLING POINTER - local variable destroyed
```

3. **Uninitialized Pointer:**

```c
char* p;  // Contains garbage value
*p = 'A'; // DANGLING POINTER - points to random memory
```

### Prevention:

- Set pointers to NULL after freeing
- Use smart pointers (C++: `unique_ptr`, `shared_ptr`)
- Initialize all pointers
- Avoid returning addresses of local variables

---

## 11. What is a Mutex?

**Answer:** A mutex (mutual exclusion lock) is a synchronization primitive used to ensure that only one thread can access a shared resource at any given time.

**Key Characteristics:**

- **Binary lock:** Either locked or unlocked
- **Ownership:** Only the thread that locked it can unlock it
- **Blocking:** Threads waiting for the mutex are put to sleep
- **Prevents race conditions**

**Usage Pattern:**

```c
pthread_mutex_lock(&mutex);
// Critical section - protected resource access
pthread_mutex_unlock(&mutex);
```

**Common Issues:**

- Deadlock (circular wait)
- Priority inversion
- Forgotten unlock (use RAII in C++)

---

## 12. What is the Difference Between Arrays and Pointers?

**Answer:**

| Aspect           | Array                                   | Pointer                          |
| ---------------- | --------------------------------------- | -------------------------------- |
| **Definition**   | Contiguous memory block with fixed size | Variable storing memory address  |
| **Size**         | Fixed at compile time                   | Fixed size (4 or 8 bytes)        |
| **Memory**       | Actual data storage                     | Stores address only              |
| **sizeof()**     | Returns total array size                | Returns pointer size (4/8 bytes) |
| **Assignment**   | Cannot be reassigned                    | Can be reassigned                |
| **Arithmetic**   | Array name decays to pointer            | Full pointer arithmetic          |
| **Modification** | Array name is constant                  | Pointer is variable              |

**Example:**

```c
int arr[10];        // Array of 10 integers
int* ptr = arr;     // Pointer to first element

sizeof(arr);        // 40 bytes (10 * 4)
sizeof(ptr);        // 8 bytes (on 64-bit system)
```

**Key Point:** Array name decays to pointer in most contexts, but they are fundamentally different.

---

## 13. How to Prevent Multiple Inclusion of Header Files?

**Answer:**

### Method 1: Include Guards (Preprocessor Directives)

```c
#ifndef HEADER_FILE_NAME_H
#define HEADER_FILE_NAME_H

// Header file contents

#endif // HEADER_FILE_NAME_H
```

### Method 2: #pragma once (Modern Approach)

```c
#pragma once

// Header file contents
```

**Comparison:**

- **Include Guards:** Standard, portable, C and C++ compatible
- **#pragma once:** Simpler, faster compilation, non-standard but widely supported

**Best Practice:** Use include guards for maximum portability, or `#pragma once` for modern projects.

---

## 14. What is the Difference Between Stack and Queue?

**Answer:**

| Feature        | Stack                                                                         | Queue                                                   |
| -------------- | ----------------------------------------------------------------------------- | ------------------------------------------------------- |
| **Principle**  | LIFO (Last In, First Out)                                                     | FIFO (First In, First Out)                              |
| **Operations** | push, pop, top                                                                | enqueue, dequeue, front                                 |
| **Access**     | Only top element                                                              | Front and rear elements                                 |
| **Use Cases**  | Function call stack, expression evaluation, undo operations, bracket matching | Task scheduling, buffer management, BFS, message queues |

**Visual Representation:**

```
Stack:           Queue:
[3] <- top       [1][2][3]
[2]               ^      ^
[1]              front  rear
```

**Real-world Examples:**

- **Stack:** Browser back button, recursive function calls
- **Queue:** Print spooler, packet buffering, customer service lines

---

## 15. Why Can't Interrupts Accept Parameters?

**Answer:**

**Reasons:**

1. **Asynchronous Nature:** Interrupts are asynchronous events triggered by hardware or software events, not called directly by code
2. **Unpredictable Timing:** Cannot predict when interrupt will occur, so cannot prepare parameters in advance
3. **No Call Stack:** Interrupts don't follow normal function calling conventions
4. **Hardware Triggered:** Often triggered by hardware signals, not software calls

**Alternative Approaches:**

- Use global/static variables to share data
- Read hardware registers directly
- Use interrupt context/status registers
- Implement handler that reads necessary data from predetermined locations

**Comparison with Functions:**

```c
// Normal function - can pass parameters
void processData(int value, char* buffer);

// Interrupt handler - no parameters
void ISR_Timer(void) __attribute__((interrupt));
```

---

## 16. UART Data Frame Format

**Answer:**

A UART (Universal Asynchronous Receiver-Transmitter) data frame consists of:

1. **Start Bit:** 1 bit (always logic LOW/0)

   - Signals beginning of transmission

2. **Data Bits:** 5-9 bits (typically 8 bits)

   - Contains actual data
   - LSB transmitted first

3. **Parity Bit:** 0 or 1 bit (optional)

   - Error detection
   - Even, Odd, or None

4. **Stop Bit(s):** 1, 1.5, or 2 bits (always logic HIGH/1)
   - Signals end of transmission

**Common Configuration: 8N1**

- 8 data bits, No parity, 1 stop bit

**Example Frame:**

```
[Start][D0][D1][D2][D3][D4][D5][D6][D7][Parity][Stop]
  0     b0  b1  b2  b3  b4  b5  b6  b7    ?      1
```

---

## 17. What is an Interrupt?

**Answer:**

An interrupt is a critical mechanism in computer systems used to respond to events or conditions requiring immediate attention. It is an asynchronous event that can temporarily halt the currently executing program to handle urgent situations or external device requests.

**Key Characteristics:**

- **Asynchronous:** Can occur at any time
- **Hardware or Software:** Triggered by peripherals or software instructions
- **Priority Levels:** Supports nested interrupts based on priority
- **Context Switching:** Saves current state, executes handler, restores state

**Types:**

1. **Hardware Interrupts:** Timer, keyboard, network card, GPIO
2. **Software Interrupts:** System calls, exceptions, traps

**Benefits:**

- Efficient I/O handling
- Real-time response
- Better CPU utilization
- Event-driven programming

---

## 18. What are the Uses of the static Keyword?

**Answer:**

### Three Main Uses:

1. **Control Variable Lifetime**

   - Static local variables persist across function calls
   - Initialized only once

   ```c
   void counter() {
       static int count = 0;  // Persists between calls
       count++;
   }
   ```

2. **Control Scope (Internal Linkage)**

   - Static global variables/functions visible only within the file
   - Prevents naming conflicts

   ```c
   static int filePrivateVar;  // Not visible to other files
   static void helperFunction() { }  // Internal linkage
   ```

3. **Class Members (C++)**
   - Static members shared across all instances
   - Accessed without object instance
   ```cpp
   class MyClass {
       static int instanceCount;  // Shared by all objects
   };
   ```

---

## 19. Describe the Interrupt Execution Process

**Answer:**

### Interrupt Handling Steps:

1. **Interrupt Request (IRQ)**

   - Hardware device or software triggers interrupt signal

2. **Interrupt Controller Response**

   - Interrupt controller receives and prioritizes the request
   - Checks if interrupts are enabled and if priority is sufficient

3. **Interrupt Acknowledgment**

   - CPU acknowledges the interrupt
   - Completes current instruction execution

4. **Context Saving**

   - Save CPU registers (PC, flags, general-purpose registers) to stack
   - Save current execution state

5. **Interrupt Vector Determination**

   - Look up interrupt vector table
   - Determine ISR (Interrupt Service Routine) address

6. **ISR Execution**

   - Jump to interrupt handler
   - Execute interrupt-specific code
   - Handle the event that triggered the interrupt

7. **Context Restoration**
   - Restore saved registers and state
   - Return from interrupt (RETI instruction)
   - Resume interrupted program

**Critical Point:** ISRs should be short and efficient to minimize interrupt latency.

---

## 20. What is Polymorphism?

**Answer:**

Polymorphism is a core feature of object-oriented programming that allows objects of different types to be treated uniformly through a common interface while exhibiting different behaviors.

**Types:**

1. **Compile-time Polymorphism (Static)**

   - Function overloading
   - Operator overloading
   - Template specialization

2. **Runtime Polymorphism (Dynamic)**
   - Virtual functions
   - Abstract classes and interfaces
   - Resolved at runtime via vtable

**Example:**

```cpp
class Animal {
public:
    virtual void makeSound() { cout << "Generic sound"; }
};

class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!"; }
};

class Cat : public Animal {
public:
    void makeSound() override { cout << "Meow!"; }
};

Animal* pet = new Dog();
pet->makeSound();  // Output: "Woof!" (runtime polymorphism)
```

**Benefits:**

- Code reusability
- Flexibility and extensibility
- Simplified code maintenance

---

## 21. What are the Memory Allocation Methods in C?

**Answer:**

### Three Memory Allocation Methods:

1. **Static Memory Allocation**

   - Allocated at compile time
   - Fixed size and lifetime
   - Stored in data segment (.data or .bss)

   ```c
   static int globalVar;      // Global static
   int array[100];            // Fixed-size array
   ```

2. **Stack Allocation (Automatic)**

   - Allocated at runtime when entering scope
   - Automatic deallocation when leaving scope
   - Fast allocation/deallocation (just stack pointer adjustment)
   - Limited size (typically 1-8 MB)

   ```c
   void function() {
       int localVar;          // Stack allocation
       char buffer[256];      // Stack allocation
   }
   ```

3. **Heap Allocation (Dynamic)**
   - Runtime allocation via `malloc()`, `calloc()`, `realloc()`
   - Manual deallocation via `free()`
   - Large, flexible size
   - Slower than stack (requires memory manager)
   ```c
   int* ptr = (int*)malloc(sizeof(int) * 100);
   free(ptr);
   ```

**Memory Layout:**

```
High Address
|  Stack  |  (grows down)
|---------|
|  Heap   |  (grows up)
|---------|
|  .bss   |  (uninitialized static)
|  .data  |  (initialized static)
|  .text  |  (code)
Low Address
```

---

## 22. What is the Difference Between struct and class?

**Answer:**

| Aspect                  | struct                 | class                          |
| ----------------------- | ---------------------- | ------------------------------ |
| **Default Access**      | public                 | private                        |
| **Default Inheritance** | public                 | private                        |
| **Purpose/Convention**  | Plain data structures  | Objects with behavior and data |
| **Usage**               | C-compatible POD types | Full OOP features              |

**C++ Examples:**

```cpp
struct Point {
    int x, y;  // public by default
};

class Rectangle {
    int width, height;  // private by default
public:
    void setDimensions(int w, int h);
};
```

**Key Point:** In C++, `struct` and `class` are nearly identical except for default access levels. The choice is often a matter of coding style and convention.

**Best Practice:**

- Use `struct` for simple data aggregates (POD types)
- Use `class` for objects with encapsulation and methods

---

## 23. What is the Difference Between Functions and Interrupts?

**Answer:**

| Aspect             | Function                         | Interrupt                          |
| ------------------ | -------------------------------- | ---------------------------------- |
| **Context Switch** | No context switch                | Full context switch occurs         |
| **Invocation**     | Called explicitly by code        | Triggered asynchronously by events |
| **Timing**         | Synchronous (predictable)        | Asynchronous (unpredictable)       |
| **Parameters**     | Can accept parameters            | Cannot accept parameters           |
| **Return Value**   | Can return values                | Cannot return values               |
| **Execution Flow** | Sequential, part of program flow | Preempts current execution         |
| **Overhead**       | Low (simple call/return)         | High (save/restore state)          |

**Example:**

```c
// Function - synchronous call
int add(int a, int b) {
    return a + b;
}
int result = add(5, 3);  // Called directly

// Interrupt - asynchronous event
void __attribute__((interrupt)) Timer_ISR(void) {
    // Handle timer interrupt
    // No parameters, no return value
}
```

---

## 24. What is the Difference Between Spinlock and Semaphore?

**Answer:**

| Feature               | Spinlock                        | Semaphore                     |
| --------------------- | ------------------------------- | ----------------------------- |
| **Waiting Mechanism** | Busy-waiting (active polling)   | Blocking (sleep/wake)         |
| **CPU Usage**         | Consumes CPU while waiting      | Releases CPU while waiting    |
| **Context Switch**    | No context switch               | Context switch occurs         |
| **Critical Section**  | Very short duration             | Can be longer duration        |
| **Use Case**          | Low contention, short locks     | High contention, longer locks |
| **Suitable For**      | Kernel code, interrupt handlers | User-space applications       |
| **Sleep**             | Cannot sleep while holding      | Thread sleeps when blocked    |

**When to Use:**

**Spinlock:**

- Critical section < context switch overhead
- Multiprocessor systems
- Interrupt handlers (cannot sleep)
- Lock held for microseconds

**Semaphore:**

- Critical section > context switch overhead
- Single processor systems
- User-space applications
- Lock held for milliseconds or longer

**Example:**

```c
// Spinlock - busy wait
spin_lock(&lock);
// Very short critical section
spin_unlock(&lock);

// Semaphore - sleep wait
sem_wait(&sem);
// Potentially longer critical section
sem_post(&sem);
```

---

## 25. How to Detect a Cycle in a Linked List?

**Answer:**

### Floyd's Cycle Detection Algorithm (Fast and Slow Pointers)

**Algorithm:**

1. Create two pointers: `fast` and `slow`, both starting at the head
2. Move `fast` two steps at a time
3. Move `slow` one step at a time
4. If `fast` and `slow` meet, a cycle exists
5. If `fast` reaches NULL, no cycle exists

**Implementation:**

```c
bool hasCycle(struct ListNode* head) {
    if (head == NULL) return false;

    struct ListNode* slow = head;
    struct ListNode* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;           // Move 1 step
        fast = fast->next->next;     // Move 2 steps

        if (slow == fast) {
            return true;  // Cycle detected
        }
    }

    return false;  // No cycle
}
```

**Time Complexity:** O(n)  
**Space Complexity:** O(1)

**Why It Works:** In a cycle, the faster pointer will eventually "lap" the slower pointer, causing them to meet within the cycle.

---

## 26. What Should You Pay Attention to When Using Multithreading?

**Answer:**

### Key Considerations:

1. **Thread Safety**

   - **Issue:** Race conditions when multiple threads access shared resources
   - **Solution:** Use synchronization mechanisms (mutex, semaphore, condition variables)
   - Protect critical sections properly

2. **Inter-Thread Communication**

   - Design proper communication mechanisms
   - Use thread-safe queues, semaphores, events
   - Avoid shared state when possible

3. **Deadlock Prevention**

   - **Causes:** Circular dependencies, improper lock ordering
   - **Prevention:**
     - Acquire locks in consistent order
     - Use timeouts
     - Avoid holding multiple locks
     - Use deadlock detection algorithms

4. **Context Switching Overhead**

   - Context switches consume CPU cycles and cache efficiency
   - **Optimization:**
     - Reduce thread count to appropriate level
     - Minimize lock contention
     - Use thread pools instead of creating/destroying threads

5. **Proper Resource Management**

   - Ensure exclusive access to shared resources
   - Use RAII in C++ for automatic cleanup
   - Avoid resource leaks

6. **Efficient Thread Scheduling and Task Division**

   - Balance workload across threads
   - Avoid thread starvation
   - Use appropriate thread pool sizes
   - Consider CPU core count

7. **Error and Exception Handling**
   - Catch exceptions within threads (uncaught exceptions terminate thread)
   - Implement proper error reporting mechanisms
   - Ensure thread stability and reliability

**Best Practices:**

```cpp
// Use RAII for lock management
{
    std::lock_guard<std::mutex> lock(mutex);
    // Critical section
}  // Automatic unlock

// Avoid deadlock with lock ordering
std::lock(mutex1, mutex2);  // Atomic lock acquisition
std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
```

---

## 27. Implement the strcpy Function

**Answer:**

```c
char* mystrcpy(char* dest, const char* src) {
    char* temp = dest;

    // Copy characters including null terminator
    while (*temp++ = *src++);

    return dest;
}
```

**Improved Version with Safety Checks:**

```c
char* mystrcpy(char* dest, const char* src) {
    // Safety checks
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    char* original_dest = dest;

    // Copy characters one by one
    while ((*dest++ = *src++) != '\0');

    return original_dest;
}
```

**Key Points:**

- Returns pointer to destination
- Copies until null terminator (inclusive)
- Assumes destination has sufficient space
- No overlap checking (use `memmove` for overlapping regions)

**Standard Library Signature:**

```c
char* strcpy(char* dest, const char* src);
```

---

## 28. Implement the strcat Function

**Answer:**

```c
char* mystrcat(char* dest, const char* src) {
    char* temp = dest;

    // Find end of dest string
    while (*temp != '\0') {
        temp++;
    }

    // Copy src to end of dest
    while (*src != '\0') {
        *temp = *src;
        temp++;
        src++;
    }

    // Add null terminator
    *temp = '\0';

    return dest;
}
```

**Improved Version:**

```c
char* mystrcat(char* dest, const char* src) {
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    char* original_dest = dest;

    // Find end of dest
    while (*dest != '\0') {
        dest++;
    }

    // Copy src to end of dest (including null terminator)
    while ((*dest++ = *src++) != '\0');

    return original_dest;
}
```

**More Efficient Version:**

```c
char* mystrcat(char* dest, const char* src) {
    char* ptr = dest;

    // Find end and copy in one loop structure
    while (*ptr) ptr++;              // Find end
    while ((*ptr++ = *src++));       // Copy src

    return dest;
}
```

**Key Points:**

- Concatenates `src` to end of `dest`
- Assumes `dest` has sufficient space
- Returns pointer to destination
- Original `dest` string is modified

**Standard Library Signature:**

```c
char* strcat(char* dest, const char* src);
```

---

## Additional Tips for Ciena Interview

### Network Protocol Questions (Based on Job Requirements):

**29. Explain the TCP Three-Way Handshake**

```
Client                Server
  |                      |
  |----SYN (seq=x)------>|
  |                      |
  |<--SYN-ACK (seq=y)---|
  |   (ack=x+1)          |
  |                      |
  |----ACK (ack=y+1)---->|
  |                      |
  [Connection Established]
```

**30. What is MPLS?**

- Multi-Protocol Label Switching
- Packet forwarding based on labels rather than IP addresses
- Used in telecom networks for traffic engineering
- Provides faster routing and QoS support

**31. Real-Time System Characteristics:**

- Deterministic response times
- Deadline-driven scheduling
- Priority-based task management
- Low interrupt latency requirements

**Good luck with your Ciena interview! 🎯**
