# Semaphore (信号量)

## 🎯 What is a Semaphore?

A semaphore is a **counter** that controls access to a shared resource by multiple threads/processes.

**Key Concept:** Think of it as a **parking lot with N spaces**:

- `sem_wait()` → Try to park (decrease counter)
- `sem_post()` → Leave parking (increase counter)
- If counter = 0, wait until space available

---

## 📖 Basic Operations

```c
#include <semaphore.h>

sem_t semaphore;

sem_init(&semaphore, 0, initial_value);  // Initialize
sem_wait(&semaphore);                    // Decrement (wait if 0)
// Critical section
sem_post(&semaphore);                    // Increment
sem_destroy(&semaphore);                 // Cleanup
```

**Parameters for `sem_init`:**

- `pshared`: 0 = threads, non-zero = processes
- `value`: Initial counter value

---

## 📊 Two Types of Semaphores

### 1. Binary Semaphore (Value: 0 or 1)

**Similar to mutex but NOT the same!**

```c
sem_t binary_sem;
sem_init(&binary_sem, 0, 1);  // Initial value = 1

// Thread 1
sem_wait(&binary_sem);  // Counter: 1 → 0
// Critical section
sem_post(&binary_sem);  // Counter: 0 → 1

// Thread 2 (if tries to enter while Thread 1 is inside)
sem_wait(&binary_sem);  // Blocks! Counter is 0
```

### 2. Counting Semaphore (Value: 0 to N)

**Control limited resources (e.g., connection pool)**

```c
sem_t counting_sem;
sem_init(&counting_sem, 0, 5);  // 5 resources available

// Thread 1
sem_wait(&counting_sem);  // Counter: 5 → 4
// Thread 2
sem_wait(&counting_sem);  // Counter: 4 → 3
// Thread 3
sem_wait(&counting_sem);  // Counter: 3 → 2
// Thread 4
sem_wait(&counting_sem);  // Counter: 2 → 1
// Thread 5
sem_wait(&counting_sem);  // Counter: 1 → 0
// Thread 6
sem_wait(&counting_sem);  // BLOCKS! Counter is 0
```

---

## 💻 Interview Example 1: Producer-Consumer (Single Buffer)

**Classic Interview Question!**

```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 1

int buffer;
sem_t empty;  // Counts empty slots
sem_t full;   // Counts full slots

void* producer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sem_wait(&empty);  // Wait for empty slot

        buffer = i;
        printf("Produced: %d\n", buffer);

        sem_post(&full);   // Signal: slot is now full
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sem_wait(&full);   // Wait for full slot

        printf("Consumed: %d\n", buffer);

        sem_post(&empty);  // Signal: slot is now empty
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, 1);  // Initially 1 empty slot
    sem_init(&full, 0, 0);   // Initially 0 full slots

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
```

**Output:**

```
Produced: 1
Consumed: 1
Produced: 2
Consumed: 2
Produced: 3
Consumed: 3
...
```

---

## 💻 Interview Example 2: Limited Resource Pool

**Question:** "Implement a connection pool with max 3 connections"

```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_CONNECTIONS 3

sem_t connection_pool;

void* worker(void* arg) {
    int id = *(int*)arg;

    printf("Worker %d: Requesting connection...\n", id);
    sem_wait(&connection_pool);  // Acquire connection

    printf("Worker %d: Got connection! Working...\n", id);
    sleep(2);  // Simulate work

    printf("Worker %d: Releasing connection\n", id);
    sem_post(&connection_pool);  // Release connection

    return NULL;
}

int main() {
    pthread_t threads[6];
    int ids[6];

    sem_init(&connection_pool, 0, MAX_CONNECTIONS);

    // Start 6 workers, but only 3 can run at once
    for (int i = 0; i < 6; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&connection_pool);
    return 0;
}
```

**Output:**

```
Worker 1: Requesting connection...
Worker 1: Got connection! Working...
Worker 2: Requesting connection...
Worker 2: Got connection! Working...
Worker 3: Requesting connection...
Worker 3: Got connection! Working...
Worker 4: Requesting connection...
Worker 5: Requesting connection...
Worker 6: Requesting connection...
[... Workers 4, 5, 6 wait ...]
Worker 1: Releasing connection
Worker 4: Got connection! Working...
...
```

---

## 💻 Interview Example 3: Reader-Writer Problem

**Advanced Interview Question!**

```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

int shared_data = 0;
int reader_count = 0;

sem_t write_lock;        // Protects shared_data
sem_t reader_count_lock; // Protects reader_count

void* reader(void* arg) {
    int id = *(int*)arg;

    // Entry section
    sem_wait(&reader_count_lock);
    reader_count++;
    if (reader_count == 1) {
        sem_wait(&write_lock);  // First reader locks writers out
    }
    sem_post(&reader_count_lock);

    // Reading section
    printf("Reader %d: Reading data = %d\n", id, shared_data);
    sleep(1);

    // Exit section
    sem_wait(&reader_count_lock);
    reader_count--;
    if (reader_count == 0) {
        sem_post(&write_lock);  // Last reader unlocks writers
    }
    sem_post(&reader_count_lock);

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    printf("Writer %d: Waiting to write...\n", id);
    sem_wait(&write_lock);

    // Writing section
    shared_data += 10;
    printf("Writer %d: Wrote data = %d\n", id, shared_data);
    sleep(1);

    sem_post(&write_lock);

    return NULL;
}

int main() {
    pthread_t readers[3], writers[2];
    int r_ids[3] = {1, 2, 3};
    int w_ids[2] = {1, 2};

    sem_init(&write_lock, 0, 1);
    sem_init(&reader_count_lock, 0, 1);

    // Create readers and writers
    for (int i = 0; i < 3; i++) {
        pthread_create(&readers[i], NULL, reader, &r_ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &w_ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&write_lock);
    sem_destroy(&reader_count_lock);

    return 0;
}
```

---

## 🔄 Semaphore vs Mutex

| Feature       | Mutex                    | Semaphore                           |
| ------------- | ------------------------ | ----------------------------------- |
| **Purpose**   | Lock/Unlock              | Signal/Wait                         |
| **Ownership** | Has owner                | No owner                            |
| **Value**     | Binary (0/1)             | Counter (0 to N)                    |
| **Unlock**    | Same thread must unlock  | Any thread can post                 |
| **Use Case**  | Protect critical section | Synchronize events, limit resources |

**Interview Question:** "When to use mutex vs semaphore?"

**Answer:**

```c
// ✅ Mutex - for mutual exclusion
pthread_mutex_lock(&mutex);
shared_variable++;
pthread_mutex_unlock(&mutex);

// ✅ Semaphore - for signaling/counting
sem_wait(&items_available);  // Wait for item
process_item();
sem_post(&items_processed);  // Signal completion
```

---

## 🐛 Common Mistakes

### 1. Forget to Post

```c
void worker() {
    sem_wait(&sem);

    if (error) {
        return;  // 🐛 BUG! Forgot sem_post()
    }

    sem_post(&sem);
}
```

### 2. Wrong Initial Value

```c
// ❌ Producer-Consumer with wrong init
sem_t empty, full;
sem_init(&empty, 0, 0);  // Wrong! Should be buffer size
sem_init(&full, 0, 5);   // Wrong! Should be 0
```

### 3. Deadlock with Semaphores

```c
sem_t sem1, sem2;
sem_init(&sem1, 0, 1);
sem_init(&sem2, 0, 1);

// Thread 1
sem_wait(&sem1);
sem_wait(&sem2);  // Might deadlock with Thread 2

// Thread 2
sem_wait(&sem2);
sem_wait(&sem1);  // Might deadlock with Thread 1
```

---

## 🎯 Common Interview Questions

### Q1: "Implement a barrier for N threads using semaphores"

```c
#define N 5

sem_t barrier_sem;
sem_t mutex;
int count = 0;

void* worker(void* arg) {
    int id = *(int*)arg;

    // Do some work
    printf("Thread %d: Working...\n", id);
    sleep(1);

    // Barrier
    sem_wait(&mutex);
    count++;
    if (count == N) {
        sem_post(&barrier_sem);  // Release first thread
    }
    sem_post(&mutex);

    sem_wait(&barrier_sem);
    sem_post(&barrier_sem);  // Release next thread

    // Continue after barrier
    printf("Thread %d: Continuing...\n", id);

    return NULL;
}
```

### Q2: "What's the difference between sem_wait and sem_trywait?"

```c
// sem_wait - blocks if counter is 0
sem_wait(&sem);  // Waits forever

// sem_trywait - returns immediately
if (sem_trywait(&sem) == 0) {
    // Got the semaphore
    sem_post(&sem);
} else {
    // Semaphore busy, do something else
    printf("Resource busy\n");
}
```

---

## 📝 Summary for Interview

**When to use Semaphore:**

- ✅ Producer-Consumer problem
- ✅ Limiting concurrent access (connection pool)
- ✅ Signaling between threads
- ✅ Reader-Writer problem

**Key Points:**

1. Counter-based synchronization
2. No ownership (any thread can post)
3. Binary semaphore ≈ mutex (but different!)
4. Counting semaphore for resource pools
5. Always pair wait/post

**Next:** [Condition Variable →](./03_Condition_Variable.md)
