# Solutions to Priority Inversion

## 🛡️ Overview of Solutions

There are **three main approaches** to solving priority inversion:

1. **Priority Inheritance Protocol** (dynamic)
2. **Priority Ceiling Protocol** (static)
3. **Avoiding Shared Resources** (design)

---

## 1️⃣ Priority Inheritance Protocol

### 📖 How It Works

**Principle:** When a high-priority task is blocked by a low-priority task, the low-priority task **temporarily inherits** the high-priority task's priority.

### Step-by-Step Process

```
Initial State:
- Task L (priority 10) holds mutex
- Task M (priority 50) is ready
- Task H (priority 99) needs mutex

Without Priority Inheritance:
t1: Task L (10) holds mutex
t2: Task H (99) blocks on mutex
t3: Task M (50) preempts Task L → Priority Inversion!
t4: Task M completes
t5: Task L releases mutex
t6: Task H runs

With Priority Inheritance:
t1: Task L (priority 10) holds mutex
t2: Task H (priority 99) blocks on mutex
    → Task L's priority boosted to 99 ⬆️
t3: Task M (priority 50) cannot preempt Task L (now 99)
t4: Task L completes and releases mutex
    → Task L's priority restored to 10 ⬇️
t5: Task H runs immediately
```

### 🔧 POSIX Implementation

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_mutexattr_t mutex_attr;

int main() {
    // Initialize mutex attributes
    pthread_mutexattr_init(&mutex_attr);

    // Set priority inheritance protocol
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);

    // Initialize mutex with attributes
    pthread_mutex_init(&mutex, &mutex_attr);

    // Now the mutex uses priority inheritance!
    // When a high-priority thread blocks on this mutex,
    // the thread holding it inherits the high priority

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}
```

### ✅ Advantages

- **Dynamic**: Automatically adjusts priorities as needed
- **Flexible**: Works without knowing future priorities
- **Bounded**: Limits the duration of priority inversion
- **POSIX standard**: Widely supported

### ❌ Disadvantages

- **Runtime overhead**: Priority changes at runtime
- **Complex**: More difficult to implement in kernel
- **Chained blocking**: Can still have some delay with multiple resources

### 📊 Example Scenario

```c
void* low_task(void* arg) {
    // Initially priority 10
    pthread_mutex_lock(&mutex);
    // Priority becomes 99 when high_task blocks

    do_work();  // Executes with priority 99

    pthread_mutex_unlock(&mutex);
    // Priority restored to 10

    return NULL;
}

void* high_task(void* arg) {
    // Priority 99
    pthread_mutex_lock(&mutex);  // Boosts low_task to 99

    do_critical_work();

    pthread_mutex_unlock(&mutex);
    return NULL;
}
```

---

## 2️⃣ Priority Ceiling Protocol

### 📖 How It Works

**Principle:** Each mutex is assigned a **priority ceiling** (the highest priority of any task that can lock it). Any task that locks the mutex immediately inherits this ceiling priority.

### Step-by-Step Process

```
Setup:
- Mutex ceiling = 99 (highest priority that uses it)
- Task L (priority 10)
- Task M (priority 50)
- Task H (priority 99)

Execution:
t1: Task L (priority 10) locks mutex
    → Priority immediately boosted to 99 ⬆️
t2: Task M (priority 50) becomes ready
    → Cannot preempt Task L (now 99)
t3: Task H (priority 99) becomes ready
    → Blocks on mutex (Task L has same priority)
t4: Task L unlocks mutex
    → Priority restored to 10 ⬇️
t5: Task H locks mutex and runs
```

### 🔧 POSIX Implementation

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_mutexattr_t mutex_attr;

int main() {
    // Initialize mutex attributes
    pthread_mutexattr_init(&mutex_attr);

    // Set priority ceiling protocol
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_PROTECT);

    // Set the ceiling priority (highest priority that uses this mutex)
    pthread_mutexattr_setprioceiling(&mutex_attr, 99);

    // Initialize mutex with attributes
    pthread_mutex_init(&mutex, &mutex_attr);

    // Now any thread that locks this mutex gets priority 99!

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}
```

### ✅ Advantages

- **Prevents deadlock**: No circular dependencies possible
- **Predictable**: Priority known at compile time
- **Immediate boost**: No delay waiting for high-priority task
- **Simple analysis**: Easier to analyze system behavior

### ❌ Disadvantages

- **Requires planning**: Must know all task priorities beforehand
- **May over-boost**: Low task gets high priority even if no high task is ready
- **Less flexible**: Hard to change priorities at runtime

### 📊 Comparison with Priority Inheritance

```c
// Priority Inheritance: Reactive
pthread_mutex_lock(&mutex);  // Low task locks (priority 10)
// ... time passes ...
// High task blocks → Low task NOW boosted to 99
pthread_mutex_unlock(&mutex);  // Restored to 10

// Priority Ceiling: Proactive
pthread_mutex_lock(&mutex);  // Low task IMMEDIATELY boosted to 99
// No high task needed, already at ceiling
pthread_mutex_unlock(&mutex);  // Restored to 10
```

---

## 3️⃣ Avoiding Shared Resources (Design Approach)

### 📖 Design Strategies

#### Strategy 1: Lock-Free Data Structures

```c
#include <stdatomic.h>

// Instead of mutex-protected counter
typedef struct {
    atomic_int counter;
} LockFreeCounter;

void increment(LockFreeCounter* c) {
    atomic_fetch_add(&c->counter, 1);
    // No mutex needed → No priority inversion!
}
```

#### Strategy 2: Message Passing

```c
// Instead of shared memory with locks
typedef struct {
    int data;
} Message;

// High-priority task
void* high_task(void* arg) {
    Message msg = {.data = 42};
    send_message(queue, &msg);  // Non-blocking
    return NULL;
}

// Low-priority task
void* low_task(void* arg) {
    Message msg;
    receive_message(queue, &msg);  // Blocking on queue, not mutex
    return NULL;
}
```

#### Strategy 3: Minimize Critical Sections

```c
// ❌ Bad: Long critical section
void bad_design() {
    pthread_mutex_lock(&mutex);

    read_data();          // 1ms
    complex_calculation(); // 50ms ← Problem!
    write_data();         // 1ms

    pthread_mutex_unlock(&mutex);
}

// ✅ Good: Short critical section
void good_design() {
    pthread_mutex_lock(&mutex);
    read_data();  // 1ms
    pthread_mutex_unlock(&mutex);

    complex_calculation();  // 50ms outside lock

    pthread_mutex_lock(&mutex);
    write_data();  // 1ms
    pthread_mutex_unlock(&mutex);
}
```

---

## 📊 Solution Comparison Table

| Feature                 | Priority Inheritance      | Priority Ceiling        | Lock-Free Design |
| ----------------------- | ------------------------- | ----------------------- | ---------------- |
| **When applied**        | When blocking occurs      | Immediately on lock     | Always           |
| **Runtime overhead**    | Medium (priority changes) | Low (predetermined)     | Very low         |
| **Requires planning**   | No                        | Yes                     | Yes              |
| **Deadlock prevention** | No                        | Yes                     | Yes              |
| **Flexibility**         | High                      | Low                     | Medium           |
| **Implementation**      | Complex                   | Medium                  | Very complex     |
| **POSIX support**       | ✅ PTHREAD_PRIO_INHERIT   | ✅ PTHREAD_PRIO_PROTECT | ❌ (manual)      |

---

## 🎯 Choosing the Right Solution

### Use Priority Inheritance When:

- ✅ You don't know all task priorities in advance
- ✅ System requirements change frequently
- ✅ You want POSIX standard solution
- ✅ Runtime flexibility is important

### Use Priority Ceiling When:

- ✅ All task priorities are known at design time
- ✅ You want to prevent deadlock
- ✅ Predictability is critical
- ✅ System is static (few runtime changes)

### Use Lock-Free Design When:

- ✅ Designing a new system
- ✅ Performance is critical
- ✅ You can invest in complex implementation
- ✅ Shared data is simple (counters, flags)

---

## 💻 Complete Working Example

### Problem + Solution Demonstration

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

pthread_mutex_t mutex;

void set_priority(pthread_t thread, int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    pthread_setschedparam(thread, SCHED_FIFO, &param);
}

void* low_task(void* arg) {
    set_priority(pthread_self(), 10);

    printf("[LOW] Locking mutex (priority 10)\n");
    pthread_mutex_lock(&mutex);

    printf("[LOW] Working for 3 seconds...\n");
    sleep(3);

    printf("[LOW] Unlocking mutex\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* medium_task(void* arg) {
    set_priority(pthread_self(), 50);
    sleep(1);

    printf("[MEDIUM] Running for 2 seconds (no mutex)\n");
    sleep(2);
    printf("[MEDIUM] Done\n");

    return NULL;
}

void* high_task(void* arg) {
    set_priority(pthread_self(), 99);
    sleep(2);

    printf("[HIGH] Need mutex urgently!\n");
    pthread_mutex_lock(&mutex);
    printf("[HIGH] Got mutex\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t low, med, high;
    pthread_mutexattr_t attr;

    // Solution: Enable Priority Inheritance
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &attr);

    pthread_create(&low, NULL, low_task, NULL);
    pthread_create(&med, NULL, medium_task, NULL);
    pthread_create(&high, NULL, high_task, NULL);

    pthread_join(low, NULL);
    pthread_join(med, NULL);
    pthread_join(high, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);

    return 0;
}
```

**Compile:** `gcc -pthread priority_inheritance.c -o priority_inheritance`

---

## 🎯 Interview Quick Answers

**Q: "How do you solve priority inversion?"**

> There are two main POSIX solutions:
>
> 1. **Priority Inheritance**: The low-priority task holding the mutex temporarily inherits the high-priority task's priority when blocking occurs. Use `PTHREAD_PRIO_INHERIT`.
>
> 2. **Priority Ceiling**: Each mutex has a maximum priority, and any task locking it gets that priority. Use `PTHREAD_PRIO_PROTECT`.
>
> I would also minimize critical sections and use lock-free designs where possible.

**Q: "Priority inheritance vs priority ceiling?"**

> - **Inheritance** is dynamic - priority boost happens only when high-priority task blocks. More flexible but reactive.
>
> - **Ceiling** is static - priority boost happens immediately on lock. More predictable and prevents deadlock, but requires knowing all priorities upfront.
>
> For embedded systems with known priorities, I'd use ceiling. For general applications, inheritance.

---

## 🔗 Next Steps

👉 [Real-World Examples](./03_Real_World_Examples.md) - Mars Pathfinder case study

👉 [Code Examples](./04_Code_Examples.md) - Hands-on implementations
