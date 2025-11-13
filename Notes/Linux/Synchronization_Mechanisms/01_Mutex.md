# Mutex (Mutual Exclusion Lock)

## 🎯 What is a Mutex?

A mutex is a **lock** that ensures only **one thread** can access a shared resource at a time.

**Key Concept:** "Mutual Exclusion" - if thread A holds the mutex, thread B must wait.

---

## 📖 Basic Operations

```c
pthread_mutex_t mutex;

pthread_mutex_init(&mutex, NULL);      // Initialize
pthread_mutex_lock(&mutex);            // Lock (wait if locked)
// Critical section here
pthread_mutex_unlock(&mutex);          // Unlock
pthread_mutex_destroy(&mutex);         // Cleanup
```

---

## 💻 Interview Example 1: Thread-Safe Counter

### ❌ Without Mutex (Race Condition)

```c
#include <pthread.h>
#include <stdio.h>

int counter = 0;  // Shared variable

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;  // ⚠️ NOT THREAD-SAFE!
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter: %d\n", counter);  // Expected: 200000
    // Actual: Random value < 200000 (e.g., 157432)
    return 0;
}
```

**Why it fails:**

```
Thread 1: Read counter (100)
Thread 2: Read counter (100)  ← Both read same value!
Thread 1: Increment (101)
Thread 2: Increment (101)     ← Lost one increment!
Thread 1: Write back (101)
Thread 2: Write back (101)
```

### ✅ With Mutex (Correct)

```c
#include <pthread.h>
#include <stdio.h>

int counter = 0;
pthread_mutex_t mutex;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);    // 🔒 Lock
        counter++;                      // ✅ Safe!
        pthread_mutex_unlock(&mutex);  // 🔓 Unlock
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter: %d\n", counter);  // ✅ Always 200000

    pthread_mutex_destroy(&mutex);
    return 0;
}
```

**Compile & Run:**

```bash
gcc -pthread mutex_counter.c -o mutex_counter
./mutex_counter
```

---

## 💻 Interview Example 2: Shared Bank Account

**Common Interview Question:** "Implement thread-safe deposit/withdraw"

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int balance;
    pthread_mutex_t lock;
} BankAccount;

void account_init(BankAccount* acc, int initial_balance) {
    acc->balance = initial_balance;
    pthread_mutex_init(&acc->lock, NULL);
}

void deposit(BankAccount* acc, int amount) {
    pthread_mutex_lock(&acc->lock);

    int old_balance = acc->balance;
    usleep(1000);  // Simulate processing time
    acc->balance = old_balance + amount;

    printf("Deposited %d, Balance: %d\n", amount, acc->balance);

    pthread_mutex_unlock(&acc->lock);
}

void withdraw(BankAccount* acc, int amount) {
    pthread_mutex_lock(&acc->lock);

    if (acc->balance >= amount) {
        int old_balance = acc->balance;
        usleep(1000);  // Simulate processing time
        acc->balance = old_balance - amount;
        printf("Withdrew %d, Balance: %d\n", amount, acc->balance);
    } else {
        printf("Insufficient funds! Balance: %d\n", acc->balance);
    }

    pthread_mutex_unlock(&acc->lock);
}

void* customer1(void* arg) {
    BankAccount* acc = (BankAccount*)arg;
    deposit(acc, 100);
    withdraw(acc, 50);
    return NULL;
}

void* customer2(void* arg) {
    BankAccount* acc = (BankAccount*)arg;
    withdraw(acc, 80);
    deposit(acc, 200);
    return NULL;
}

int main() {
    BankAccount account;
    account_init(&account, 100);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, customer1, &account);
    pthread_create(&t2, NULL, customer2, &account);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final balance: %d\n", account.balance);

    pthread_mutex_destroy(&account.lock);
    return 0;
}
```

---

## 🐛 Common Mistakes & Interview Traps

### 1. Deadlock - Two Mutexes

```c
pthread_mutex_t mutex1, mutex2;

// ❌ Thread 1
void* thread1_func(void* arg) {
    pthread_mutex_lock(&mutex1);
    sleep(1);  // Simulate work
    pthread_mutex_lock(&mutex2);  // Wait for Thread 2

    // Critical section

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

// ❌ Thread 2
void* thread2_func(void* arg) {
    pthread_mutex_lock(&mutex2);
    sleep(1);  // Simulate work
    pthread_mutex_lock(&mutex1);  // Wait for Thread 1

    // Critical section

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}
// 💀 DEADLOCK! Thread 1 holds mutex1, waits for mutex2
//              Thread 2 holds mutex2, waits for mutex1
```

**✅ Solution: Lock in same order**

```c
// Thread 1
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);
// work...
pthread_mutex_unlock(&mutex2);
pthread_mutex_unlock(&mutex1);

// Thread 2 - SAME ORDER!
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);
// work...
pthread_mutex_unlock(&mutex2);
pthread_mutex_unlock(&mutex1);
```

### 2. Forgot to Unlock

```c
void risky_function() {
    pthread_mutex_lock(&mutex);

    if (error_condition) {
        return;  // 🐛 BUG! Forgot to unlock!
    }

    pthread_mutex_unlock(&mutex);
}
```

**✅ Solution: Use goto or always unlock**

```c
void safe_function() {
    pthread_mutex_lock(&mutex);

    if (error_condition) {
        pthread_mutex_unlock(&mutex);
        return;  // ✅ Unlocked before return
    }

    pthread_mutex_unlock(&mutex);
}
```

### 3. Double Unlock

```c
pthread_mutex_unlock(&mutex);
pthread_mutex_unlock(&mutex);  // 🐛 Undefined behavior!
```

---

## 🎯 Common Interview Questions

### Q1: "What happens if you lock a mutex twice in the same thread?"

**Answer:**

```c
pthread_mutex_lock(&mutex);
pthread_mutex_lock(&mutex);  // 💀 DEADLOCK!
```

**Result:** The thread deadlocks with itself! It waits forever for the lock it already holds.

**Solution:** Use recursive mutex:

```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
pthread_mutex_init(&mutex, &attr);
```

### Q2: "Can you unlock a mutex from a different thread?"

**Answer:** **NO!** Only the thread that locked it should unlock it. This is undefined behavior and can cause crashes.

### Q3: "What's the difference between trylock and lock?"

```c
// pthread_mutex_lock - waits forever
pthread_mutex_lock(&mutex);  // Blocks until available

// pthread_mutex_trylock - returns immediately
if (pthread_mutex_trylock(&mutex) == 0) {
    // Got the lock!
    pthread_mutex_unlock(&mutex);
} else {
    // Lock is busy, do something else
    printf("Lock busy, will try later\n");
}
```

---

## 📊 Performance Considerations

```c
// ❌ Bad: Lock too often (overhead)
for (int i = 0; i < 1000000; i++) {
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);
}

// ✅ Better: Lock less frequently
pthread_mutex_lock(&mutex);
for (int i = 0; i < 1000000; i++) {
    counter++;
}
pthread_mutex_unlock(&mutex);

// ⚖️ Trade-off: Less parallelism but less overhead
```

---

## 🎯 Interview Coding Challenge

**"Implement a thread-safe ring buffer"**

```c
#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int read_pos;
    int write_pos;
    int count;
    pthread_mutex_t mutex;
} RingBuffer;

void ring_buffer_init(RingBuffer* rb) {
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->count = 0;
    pthread_mutex_init(&rb->mutex, NULL);
}

int ring_buffer_write(RingBuffer* rb, int value) {
    pthread_mutex_lock(&rb->mutex);

    if (rb->count >= BUFFER_SIZE) {
        pthread_mutex_unlock(&rb->mutex);
        return -1;  // Buffer full
    }

    rb->buffer[rb->write_pos] = value;
    rb->write_pos = (rb->write_pos + 1) % BUFFER_SIZE;
    rb->count++;

    pthread_mutex_unlock(&rb->mutex);
    return 0;
}

int ring_buffer_read(RingBuffer* rb, int* value) {
    pthread_mutex_lock(&rb->mutex);

    if (rb->count == 0) {
        pthread_mutex_unlock(&rb->mutex);
        return -1;  // Buffer empty
    }

    *value = rb->buffer[rb->read_pos];
    rb->read_pos = (rb->read_pos + 1) % BUFFER_SIZE;
    rb->count--;

    pthread_mutex_unlock(&rb->mutex);
    return 0;
}
```

---

## 📝 Summary for Interview

**When to use Mutex:**

- ✅ Protecting shared data
- ✅ One resource, exclusive access
- ✅ Critical sections in code

**Key Points to Remember:**

1. Only ONE thread can hold mutex at a time
2. Same thread must lock AND unlock
3. Lock in consistent order to avoid deadlock
4. Keep critical section small (performance)
5. Always unlock (use RAII in C++)

**Next:** [Semaphore →](./02_Semaphore.md)
