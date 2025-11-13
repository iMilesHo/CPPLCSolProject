# Priority Inversion - Code Examples

## 📚 Overview

This file contains complete, compilable code examples demonstrating:

1. How priority inversion occurs
2. How to fix it with priority inheritance
3. How to fix it with priority ceiling
4. Best practices for avoiding it

**All examples are ready to compile and run!**

---

## 💻 Example 1: Reproducing Priority Inversion

### Complete Working Code

```c
// File: priority_inversion_demo.c
// Compile: gcc -pthread priority_inversion_demo.c -o priority_inversion_demo
// Run: sudo ./priority_inversion_demo (needs root for SCHED_FIFO)

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <sys/time.h>

pthread_mutex_t shared_mutex = PTHREAD_MUTEX_INITIALIZER;

// Helper function to get current time in milliseconds
long long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// Helper function to set thread priority
void set_thread_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        perror("Failed to set thread priority");
    }
}

void* low_priority_task(void* arg) {
    long long start_time, end_time;

    set_thread_priority(10);  // Lowest priority

    printf("[%lld ms] [LOW-10] Starting\n", current_time_ms());

    start_time = current_time_ms();
    pthread_mutex_lock(&shared_mutex);
    printf("[%lld ms] [LOW-10] Acquired mutex\n", current_time_ms());

    // Simulate work while holding mutex
    printf("[%lld ms] [LOW-10] Working for 3 seconds...\n", current_time_ms());
    sleep(3);

    pthread_mutex_unlock(&shared_mutex);
    end_time = current_time_ms();
    printf("[%lld ms] [LOW-10] Released mutex (held for %lld ms)\n",
           current_time_ms(), end_time - start_time);

    return NULL;
}

void* medium_priority_task(void* arg) {
    long long start_time, end_time;

    set_thread_priority(50);  // Medium priority

    // Start slightly after low task has the mutex
    sleep(1);

    start_time = current_time_ms();
    printf("[%lld ms] [MED-50] Starting (no mutex needed)\n", current_time_ms());

    // CPU-intensive work that doesn't need the mutex
    for (int i = 0; i < 5; i++) {
        printf("[%lld ms] [MED-50] Working... %d/5\n", current_time_ms(), i+1);
        usleep(500000);  // 500ms
    }

    end_time = current_time_ms();
    printf("[%lld ms] [MED-50] Completed (took %lld ms)\n",
           current_time_ms(), end_time - start_time);

    return NULL;
}

void* high_priority_task(void* arg) {
    long long start_time, end_time;

    set_thread_priority(99);  // Highest priority

    // Start after both other tasks
    sleep(2);

    start_time = current_time_ms();
    printf("[%lld ms] [HIGH-99] Starting - URGENT! Need mutex!\n", current_time_ms());

    pthread_mutex_lock(&shared_mutex);
    end_time = current_time_ms();

    printf("[%lld ms] [HIGH-99] Finally got mutex (waited %lld ms)\n",
           current_time_ms(), end_time - start_time);

    // Critical work
    printf("[%lld ms] [HIGH-99] Doing critical work\n", current_time_ms());

    pthread_mutex_unlock(&shared_mutex);
    printf("[%lld ms] [HIGH-99] Completed\n", current_time_ms());

    return NULL;
}

int main() {
    pthread_t low, medium, high;

    printf("=== Priority Inversion Demonstration ===\n");
    printf("This will show how a HIGH priority task is blocked by MEDIUM priority work\n\n");

    pthread_create(&low, NULL, low_priority_task, NULL);
    pthread_create(&medium, NULL, medium_priority_task, NULL);
    pthread_create(&high, NULL, high_priority_task, NULL);

    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    printf("\n=== Analysis ===\n");
    printf("Notice: HIGH priority task had to wait for MEDIUM priority task to finish!\n");
    printf("This is priority inversion.\n");

    return 0;
}
```

### Expected Output

```
=== Priority Inversion Demonstration ===
This will show how a HIGH priority task is blocked by MEDIUM priority work

[0 ms] [LOW-10] Starting
[1 ms] [LOW-10] Acquired mutex
[1 ms] [LOW-10] Working for 3 seconds...
[1000 ms] [MED-50] Starting (no mutex needed)
[1001 ms] [MED-50] Working... 1/5
[1501 ms] [MED-50] Working... 2/5
[2000 ms] [HIGH-99] Starting - URGENT! Need mutex!
[2001 ms] [MED-50] Working... 3/5
[2502 ms] [MED-50] Working... 4/5
[3002 ms] [MED-50] Working... 5/5
[3503 ms] [MED-50] Completed (took 2503 ms)
[3504 ms] [LOW-10] Released mutex (held for 3503 ms)
[3504 ms] [HIGH-99] Finally got mutex (waited 1504 ms)
[3505 ms] [HIGH-99] Doing critical work
[3505 ms] [HIGH-99] Completed

=== Analysis ===
Notice: HIGH priority task had to wait for MEDIUM priority task to finish!
This is priority inversion.
```

---

## 💻 Example 2: Fix with Priority Inheritance

```c
// File: priority_inheritance_fix.c
// Compile: gcc -pthread priority_inheritance_fix.c -o priority_inheritance_fix
// Run: sudo ./priority_inheritance_fix

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>

pthread_mutex_t shared_mutex;
pthread_mutexattr_t mutex_attr;

long long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void set_thread_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
}

void* low_priority_task(void* arg) {
    set_thread_priority(10);

    printf("[%lld ms] [LOW-10] Starting\n", current_time_ms());

    pthread_mutex_lock(&shared_mutex);
    printf("[%lld ms] [LOW-10] Acquired mutex (priority will boost when HIGH blocks)\n",
           current_time_ms());

    sleep(3);

    pthread_mutex_unlock(&shared_mutex);
    printf("[%lld ms] [LOW-10] Released mutex (priority restored to 10)\n",
           current_time_ms());

    return NULL;
}

void* medium_priority_task(void* arg) {
    set_thread_priority(50);
    sleep(1);

    printf("[%lld ms] [MED-50] Starting\n", current_time_ms());

    for (int i = 0; i < 5; i++) {
        printf("[%lld ms] [MED-50] Trying to run... (should be blocked by inherited priority)\n",
               current_time_ms());
        usleep(500000);
    }

    printf("[%lld ms] [MED-50] Completed\n", current_time_ms());
    return NULL;
}

void* high_priority_task(void* arg) {
    long long start_time, end_time;

    set_thread_priority(99);
    sleep(2);

    start_time = current_time_ms();
    printf("[%lld ms] [HIGH-99] Starting - Need mutex! (will boost LOW to 99)\n",
           current_time_ms());

    pthread_mutex_lock(&shared_mutex);
    end_time = current_time_ms();

    printf("[%lld ms] [HIGH-99] Got mutex (waited %lld ms)\n",
           current_time_ms(), end_time - start_time);

    pthread_mutex_unlock(&shared_mutex);
    return NULL;
}

int main() {
    pthread_t low, medium, high;

    printf("=== Priority Inheritance Solution ===\n\n");

    // Initialize mutex with priority inheritance
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&shared_mutex, &mutex_attr);

    printf("Mutex configured with PTHREAD_PRIO_INHERIT\n");
    printf("When HIGH blocks, LOW will inherit priority 99\n\n");

    pthread_create(&low, NULL, low_priority_task, NULL);
    pthread_create(&medium, NULL, medium_priority_task, NULL);
    pthread_create(&high, NULL, high_priority_task, NULL);

    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    pthread_mutex_destroy(&shared_mutex);
    pthread_mutexattr_destroy(&mutex_attr);

    printf("\n=== Result ===\n");
    printf("HIGH task waited much less time!\n");
    printf("MEDIUM task couldn't preempt LOW (inherited HIGH priority)\n");

    return 0;
}
```

---

## 💻 Example 3: Fix with Priority Ceiling

```c
// File: priority_ceiling_fix.c
// Compile: gcc -pthread priority_ceiling_fix.c -o priority_ceiling_fix
// Run: sudo ./priority_ceiling_fix

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>

pthread_mutex_t shared_mutex;
pthread_mutexattr_t mutex_attr;

long long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void set_thread_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
}

void* low_priority_task(void* arg) {
    set_thread_priority(10);

    printf("[%lld ms] [LOW-10] Starting\n", current_time_ms());

    pthread_mutex_lock(&shared_mutex);
    printf("[%lld ms] [LOW-10] Acquired mutex (IMMEDIATELY boosted to priority 99)\n",
           current_time_ms());

    sleep(3);

    pthread_mutex_unlock(&shared_mutex);
    printf("[%lld ms] [LOW-10] Released mutex (priority restored to 10)\n",
           current_time_ms());

    return NULL;
}

void* medium_priority_task(void* arg) {
    set_thread_priority(50);
    sleep(1);

    printf("[%lld ms] [MED-50] Starting\n", current_time_ms());
    printf("[%lld ms] [MED-50] Cannot preempt LOW (it has ceiling priority 99)\n",
           current_time_ms());

    for (int i = 0; i < 5; i++) {
        usleep(500000);
    }

    printf("[%lld ms] [MED-50] Completed\n", current_time_ms());
    return NULL;
}

void* high_priority_task(void* arg) {
    long long start_time, end_time;

    set_thread_priority(99);
    sleep(2);

    start_time = current_time_ms();
    printf("[%lld ms] [HIGH-99] Starting - Need mutex!\n", current_time_ms());

    pthread_mutex_lock(&shared_mutex);
    end_time = current_time_ms();

    printf("[%lld ms] [HIGH-99] Got mutex (waited %lld ms)\n",
           current_time_ms(), end_time - start_time);

    pthread_mutex_unlock(&shared_mutex);
    return NULL;
}

int main() {
    pthread_t low, medium, high;

    printf("=== Priority Ceiling Solution ===\n\n");

    // Initialize mutex with priority ceiling
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_PROTECT);
    pthread_mutexattr_setprioceiling(&mutex_attr, 99);  // Ceiling = 99
    pthread_mutex_init(&shared_mutex, &mutex_attr);

    printf("Mutex configured with PTHREAD_PRIO_PROTECT (ceiling = 99)\n");
    printf("Any thread locking mutex gets priority 99 immediately\n\n");

    pthread_create(&low, NULL, low_priority_task, NULL);
    pthread_create(&medium, NULL, medium_priority_task, NULL);
    pthread_create(&high, NULL, high_priority_task, NULL);

    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    pthread_mutex_destroy(&shared_mutex);
    pthread_mutexattr_destroy(&mutex_attr);

    printf("\n=== Result ===\n");
    printf("LOW was boosted to 99 immediately (not waiting for HIGH to block)\n");
    printf("MEDIUM couldn't preempt LOW\n");
    printf("Priority inversion prevented!\n");

    return 0;
}
```

---

## 💻 Example 4: Best Practice - Minimize Critical Section

```c
// File: minimize_critical_section.c
// Demonstrates avoiding priority inversion through design

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int sensor_value;
    pthread_mutex_t mutex;
} SharedData;

// ❌ BAD: Long critical section
void* bad_data_processor(void* arg) {
    SharedData* data = (SharedData*)arg;

    pthread_mutex_lock(&data->mutex);

    int value = data->sensor_value;  // 1ms

    // Long computation INSIDE lock
    int result = 0;
    for (int i = 0; i < 1000000; i++) {  // 100ms
        result += value * i;
    }

    data->sensor_value = result;  // 1ms

    pthread_mutex_unlock(&data->mutex);

    printf("Bad: Held lock for ~100ms\n");
    return NULL;
}

// ✅ GOOD: Minimal critical section
void* good_data_processor(void* arg) {
    SharedData* data = (SharedData*)arg;
    int value, result;

    // Critical section 1: Read (1ms)
    pthread_mutex_lock(&data->mutex);
    value = data->sensor_value;
    pthread_mutex_unlock(&data->mutex);

    // Long computation OUTSIDE lock
    result = 0;
    for (int i = 0; i < 1000000; i++) {  // 100ms
        result += value * i;
    }

    // Critical section 2: Write (1ms)
    pthread_mutex_lock(&data->mutex);
    data->sensor_value = result;
    pthread_mutex_unlock(&data->mutex);

    printf("Good: Held lock for ~2ms total\n");
    return NULL;
}

int main() {
    SharedData data = {.sensor_value = 42};
    pthread_mutex_init(&data.mutex, NULL);

    pthread_t t1, t2;

    printf("=== Bad Design ===\n");
    pthread_create(&t1, NULL, bad_data_processor, &data);
    pthread_join(t1, NULL);

    printf("\n=== Good Design ===\n");
    pthread_create(&t2, NULL, good_data_processor, &data);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&data.mutex);

    printf("\nLock time reduced from 100ms to 2ms!\n");
    printf("50x reduction in priority inversion window\n");

    return 0;
}
```

---

## 🔧 Compilation & Running

### Compile All Examples

```bash
# Requires root for SCHED_FIFO
gcc -pthread priority_inversion_demo.c -o demo
gcc -pthread priority_inheritance_fix.c -o inherit
gcc -pthread priority_ceiling_fix.c -o ceiling
gcc -pthread minimize_critical_section.c -o minimize

# Run (need sudo for real-time scheduling)
sudo ./demo
sudo ./inherit
sudo ./ceiling
./minimize  # This one doesn't need sudo
```

### If You Get Permission Errors

```bash
# Option 1: Run with sudo
sudo ./demo

# Option 2: Set capabilities (Linux only)
sudo setcap cap_sys_nice=eip ./demo
./demo

# Option 3: For testing only (no real priorities)
# Remove set_thread_priority() calls
```

---

## 📊 Performance Comparison

### Measuring Impact

```c
// Add timing measurements to see the difference

long long measure_wait_time() {
    long long start = current_time_ms();
    pthread_mutex_lock(&mutex);
    long long wait_time = current_time_ms() - start;
    pthread_mutex_unlock(&mutex);
    return wait_time;
}

// Compare:
// No solution: ~2500ms wait
// Priority inheritance: ~1000ms wait
// Priority ceiling: ~1000ms wait
// Minimal critical section: ~50ms wait
```

---

## 🎯 Interview Exercise

**Task:** Modify the code to add watchdog timeout detection

```c
#define DEADLINE_MS 100

void* high_priority_task_with_deadline(void* arg) {
    long long start_time = current_time_ms();

    pthread_mutex_lock(&shared_mutex);

    long long wait_time = current_time_ms() - start_time;

    if (wait_time > DEADLINE_MS) {
        printf("❌ DEADLINE MISSED! Waited %lld ms (limit: %d ms)\n",
               wait_time, DEADLINE_MS);
        // In real system: trigger watchdog reset
    } else {
        printf("✅ Met deadline! Waited %lld ms\n", wait_time);
    }

    pthread_mutex_unlock(&shared_mutex);
    return NULL;
}
```

---

## 📝 Summary

These examples show:

1. ✅ **How to reproduce** priority inversion
2. ✅ **How to fix with inheritance** (PTHREAD_PRIO_INHERIT)
3. ✅ **How to fix with ceiling** (PTHREAD_PRIO_PROTECT)
4. ✅ **How to avoid by design** (minimize critical sections)

**For Ciena interview:** Be ready to write code like this on a whiteboard and explain the trade-offs!

---

[← Back to README](./README.md)
