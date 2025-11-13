# Condition Variable (条件变量)

## 🎯 What is a Condition Variable?

A condition variable allows threads to **wait for a specific condition** to become true, without busy-waiting (polling).

**Key Concept:** "Wake me up when something happens"

- Thread waits for condition
- Another thread signals when condition is met
- **Always used with a mutex!**

---

## 📖 Basic Operations

```c
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

// Initialize
pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&cond, NULL);

// Wait for condition
pthread_mutex_lock(&mutex);
while (!condition) {
    pthread_cond_wait(&cond, &mutex);  // Atomically unlock & wait
}
// Condition is now true
pthread_mutex_unlock(&mutex);

// Signal condition
pthread_mutex_lock(&mutex);
condition = true;
pthread_cond_signal(&cond);  // Wake one thread
pthread_mutex_unlock(&mutex);

// Cleanup
pthread_cond_destroy(&cond);
pthread_mutex_destroy(&mutex);
```

---

## 🔑 Critical Concept: Why Always Use While Loop?

**❌ Wrong (using if):**

```c
pthread_mutex_lock(&mutex);
if (!condition) {
    pthread_cond_wait(&cond, &mutex);
}
// Might not be true anymore!
pthread_mutex_unlock(&mutex);
```

**✅ Correct (using while):**

```c
pthread_mutex_lock(&mutex);
while (!condition) {
    pthread_cond_wait(&cond, &mutex);
}
// Guaranteed to be true
pthread_mutex_unlock(&mutex);
```

**Why?**

1. **Spurious wakeups** - thread can wake up without signal
2. **Multiple waiters** - condition might be consumed by another thread
3. **Safety** - always recheck condition

---

## 💻 Interview Example 1: Simple Wait/Signal

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* waiter(void* arg) {
    printf("Waiter: Waiting for signal...\n");

    pthread_mutex_lock(&mutex);
    while (!ready) {  // Use while, not if!
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Waiter: Received signal! ready = %d\n", ready);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* signaler(void* arg) {
    sleep(2);  // Simulate some work

    printf("Signaler: Setting ready and signaling...\n");

    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, waiter, NULL);
    pthread_create(&t2, NULL, signaler, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
```

**Output:**

```
Waiter: Waiting for signal...
Signaler: Setting ready and signaling...
Waiter: Received signal! ready = 1
```

---

## 💻 Interview Example 2: Producer-Consumer with Queue

**Most Common Interview Question!**

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_SIZE 10

typedef struct {
    int buffer[QUEUE_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Queue;

void queue_init(Queue* q) {
    q->count = 0;
    q->in = 0;
    q->out = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

void queue_put(Queue* q, int value) {
    pthread_mutex_lock(&q->mutex);

    // Wait while queue is full
    while (q->count >= QUEUE_SIZE) {
        printf("Queue full, producer waiting...\n");
        pthread_cond_wait(&q->not_full, &q->mutex);
    }

    // Add to queue
    q->buffer[q->in] = value;
    q->in = (q->in + 1) % QUEUE_SIZE;
    q->count++;

    printf("Produced: %d (count: %d)\n", value, q->count);

    // Signal that queue is not empty
    pthread_cond_signal(&q->not_empty);

    pthread_mutex_unlock(&q->mutex);
}

int queue_get(Queue* q) {
    pthread_mutex_lock(&q->mutex);

    // Wait while queue is empty
    while (q->count == 0) {
        printf("Queue empty, consumer waiting...\n");
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }

    // Remove from queue
    int value = q->buffer[q->out];
    q->out = (q->out + 1) % QUEUE_SIZE;
    q->count--;

    printf("Consumed: %d (count: %d)\n", value, q->count);

    // Signal that queue is not full
    pthread_cond_signal(&q->not_full);

    pthread_mutex_unlock(&q->mutex);
    return value;
}

void* producer(void* arg) {
    Queue* q = (Queue*)arg;
    for (int i = 1; i <= 15; i++) {
        queue_put(q, i);
        usleep(100000);  // 100ms
    }
    return NULL;
}

void* consumer(void* arg) {
    Queue* q = (Queue*)arg;
    for (int i = 1; i <= 15; i++) {
        queue_get(q);
        usleep(200000);  // 200ms (slower than producer)
    }
    return NULL;
}

int main() {
    Queue queue;
    queue_init(&queue);

    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer, &queue);
    pthread_create(&cons, NULL, consumer, &queue);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_cond_destroy(&queue.not_full);
    pthread_cond_destroy(&queue.not_empty);
    pthread_mutex_destroy(&queue.mutex);

    return 0;
}
```

---

## 💻 Interview Example 3: Thread Pool

**Advanced Interview Question!**

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_TASKS 100

typedef struct {
    void (*function)(void*);
    void* arg;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int task_count;
    int running;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ThreadPool;

void pool_init(ThreadPool* pool) {
    pool->task_count = 0;
    pool->running = 1;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
}

void pool_add_task(ThreadPool* pool, void (*function)(void*), void* arg) {
    pthread_mutex_lock(&pool->mutex);

    if (pool->task_count < MAX_TASKS) {
        pool->tasks[pool->task_count].function = function;
        pool->tasks[pool->task_count].arg = arg;
        pool->task_count++;

        // Signal worker threads
        pthread_cond_signal(&pool->cond);
    }

    pthread_mutex_unlock(&pool->mutex);
}

void* worker_thread(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;

    while (1) {
        pthread_mutex_lock(&pool->mutex);

        // Wait for tasks
        while (pool->task_count == 0 && pool->running) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }

        // Exit if pool is shutting down
        if (!pool->running && pool->task_count == 0) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }

        // Get task
        Task task = pool->tasks[0];

        // Shift remaining tasks
        for (int i = 0; i < pool->task_count - 1; i++) {
            pool->tasks[i] = pool->tasks[i + 1];
        }
        pool->task_count--;

        pthread_mutex_unlock(&pool->mutex);

        // Execute task
        task.function(task.arg);
    }

    return NULL;
}

void pool_shutdown(ThreadPool* pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->running = 0;
    pthread_cond_broadcast(&pool->cond);  // Wake all threads
    pthread_mutex_unlock(&pool->mutex);
}

// Example task
void sample_task(void* arg) {
    int id = *(int*)arg;
    printf("Task %d: Executing...\n", id);
    sleep(1);
    printf("Task %d: Done\n", id);
}

int main() {
    ThreadPool pool;
    pool_init(&pool);

    pthread_t workers[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&workers[i], NULL, worker_thread, &pool);
    }

    // Add tasks
    int task_ids[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        pool_add_task(&pool, sample_task, &task_ids[i]);
    }

    sleep(5);  // Let tasks complete

    pool_shutdown(&pool);

    for (int i = 0; i < 3; i++) {
        pthread_join(workers[i], NULL);
    }

    pthread_cond_destroy(&pool.cond);
    pthread_mutex_destroy(&pool.mutex);

    return 0;
}
```

---

## 🔄 Signal vs Broadcast

### `pthread_cond_signal()` - Wake ONE thread

```c
pthread_cond_signal(&cond);  // Wakes 1 waiting thread
```

### `pthread_cond_broadcast()` - Wake ALL threads

```c
pthread_cond_broadcast(&cond);  // Wakes ALL waiting threads
```

**When to use which?**

```c
// Signal - when only ONE thread can proceed
pthread_mutex_lock(&mutex);
if (queue_count > 0) {
    pthread_cond_signal(&not_empty);  // Only 1 consumer needed
}
pthread_mutex_unlock(&mutex);

// Broadcast - when ALL threads should check condition
pthread_mutex_lock(&mutex);
shutdown = 1;
pthread_cond_broadcast(&cond);  // Wake all workers to exit
pthread_mutex_unlock(&mutex);
```

---

## 🐛 Common Mistakes

### 1. Using `if` instead of `while`

```c
// ❌ WRONG
pthread_mutex_lock(&mutex);
if (!condition) {
    pthread_cond_wait(&cond, &mutex);
}
// condition might be false again!
pthread_mutex_unlock(&mutex);

// ✅ CORRECT
pthread_mutex_lock(&mutex);
while (!condition) {
    pthread_cond_wait(&cond, &mutex);
}
// condition is definitely true
pthread_mutex_unlock(&mutex);
```

### 2. Forgetting to lock mutex

```c
// ❌ WRONG
condition = true;
pthread_cond_signal(&cond);  // Race condition!

// ✅ CORRECT
pthread_mutex_lock(&mutex);
condition = true;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&mutex);
```

### 3. Signaling before setting condition

```c
// ❌ WRONG
pthread_cond_signal(&cond);
condition = true;  // Too late!

// ✅ CORRECT
condition = true;
pthread_cond_signal(&cond);
```

---

## 🎯 Common Interview Questions

### Q1: "How does pthread_cond_wait work atomically?"

**Answer:**

```c
pthread_cond_wait(&cond, &mutex);
```

**Internally does:**

1. **Atomically** unlock mutex
2. Put thread to sleep
3. Wait for signal
4. Wake up and **atomically** relock mutex

**Why atomic?** Prevents lost wakeup:

```
Thread A (waiter):
  lock mutex
  check condition (false)
  unlock mutex  ← If not atomic, Thread B could signal here
  sleep         ← Signal lost!
```

### Q2: "Difference between cond_signal and cond_broadcast?"

| Operation     | Wakes       | Use When                             |
| ------------- | ----------- | ------------------------------------ |
| `signal()`    | 1 thread    | Only one thread can proceed          |
| `broadcast()` | All threads | All threads should recheck condition |

### Q3: "Can you use condition variable without mutex?"

**Answer:** **NO!** Always use with mutex because:

1. Protects the condition variable itself
2. Protects the shared state (condition)
3. Prevents race conditions

---

## 📊 Performance Comparison

### Busy Waiting (Bad)

```c
// ❌ Wastes CPU
while (!ready) {
    // Spin! CPU at 100%
}
```

### Sleep Polling (Better but not good)

```c
// ⚠️ Wastes time, not responsive
while (!ready) {
    usleep(1000);  // Check every 1ms
}
```

### Condition Variable (Best)

```c
// ✅ Efficient and responsive
pthread_mutex_lock(&mutex);
while (!ready) {
    pthread_cond_wait(&cond, &mutex);  // No CPU waste
}
pthread_mutex_unlock(&mutex);
```

---

## 📝 Summary for Interview

**When to use Condition Variable:**

- ✅ Wait for specific condition
- ✅ Producer-Consumer with bounded buffer
- ✅ Thread pool / work queue
- ✅ Barriers and synchronization points

**Key Points:**

1. **Always** use with mutex
2. **Always** use `while` loop, not `if`
3. `signal()` wakes one, `broadcast()` wakes all
4. `pthread_cond_wait()` atomically unlocks/locks
5. Prevents busy-waiting

**Interview Tip:** If asked "Implement blocking queue", use condition variables!

---

## 🎯 Complete Comparison Table

| Feature       | Mutex            | Semaphore           | Condition Variable      |
| ------------- | ---------------- | ------------------- | ----------------------- |
| **Purpose**   | Mutual exclusion | Count resources     | Wait for condition      |
| **Wait type** | Lock/Unlock      | Decrement/Increment | Wait/Signal             |
| **Use with**  | Alone            | Alone               | **Must use with mutex** |
| **Value**     | Binary           | Counter (0-N)       | No counter              |
| **Ownership** | Yes              | No                  | No                      |
| **Busy wait** | No               | No                  | No                      |
| **Best for**  | Protect data     | Limit access        | Event notification      |

---

## 🚀 Final Interview Coding Challenge

**"Implement a blocking queue with timeout"**

```c
typedef struct {
    int* buffer;
    int size;
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} BlockingQueue;

// Returns 0 on success, -1 on timeout
int queue_get_timeout(BlockingQueue* q, int* value, int timeout_ms) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout_ms / 1000;
    ts.tv_nsec += (timeout_ms % 1000) * 1000000;

    pthread_mutex_lock(&q->mutex);

    while (q->count == 0) {
        int ret = pthread_cond_timedwait(&q->not_empty, &q->mutex, &ts);
        if (ret == ETIMEDOUT) {
            pthread_mutex_unlock(&q->mutex);
            return -1;  // Timeout
        }
    }

    *value = q->buffer[q->out];
    q->out = (q->out + 1) % q->size;
    q->count--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);

    return 0;  // Success
}
```

---

**Congratulations! You've mastered all three synchronization mechanisms! 🎉**

[← Back to README](./README.md)
