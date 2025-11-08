# C++ 多线程完全指南 - 从入门到精通

## 📚 目录

1. [Linux 线程基础](#1-linux-线程基础)
2. [C++11 多线程标准库](#2-c11-多线程标准库)
3. [核心概念与常用类](#3-核心概念与常用类)
4. [面试高频问题](#4-面试高频问题)
5. [实战应用场景](#5-实战应用场景)

---

## 1. Linux 线程基础

### 1.1 从 Linux 原生 API 到 C++11

**历史背景：**

```
Linux 原生 (POSIX threads - pthread):
├── pthread_create()    // 创建线程
├── pthread_join()      // 等待线程结束
├── pthread_mutex_t     // 互斥锁
└── pthread_cond_t      // 条件变量

C++11 之前：
└── 必须直接使用 pthread API（平台相关，不跨平台）

C++11 之后：
└── 标准库封装，跨平台，更易用！
```

**为什么 C++11 是分水岭？**

- ✅ 第一次有了标准的多线程库
- ✅ 跨平台（Linux/Windows/macOS）
- ✅ RAII 风格，自动资源管理
- ✅ 更安全、更易用

---

## 2. C++11 多线程标准库

### 2.1 学习路线图（从易到难）

```
Level 1: 基础线程管理 (必须掌握)
├── std::thread          ⭐⭐⭐⭐⭐
├── std::mutex           ⭐⭐⭐⭐⭐
└── std::lock_guard      ⭐⭐⭐⭐⭐

Level 2: 进阶同步机制 (重要)
├── std::unique_lock     ⭐⭐⭐⭐⭐ (面试高频！)
├── std::condition_variable ⭐⭐⭐⭐
└── std::atomic          ⭐⭐⭐⭐

Level 3: 高级特性 (进阶)
├── std::shared_mutex    ⭐⭐⭐
├── std::promise/future  ⭐⭐⭐
├── std::async           ⭐⭐⭐
└── std::scoped_lock     ⭐⭐⭐

Level 4: 专业级 (可选)
├── std::shared_lock     ⭐⭐
├── std::recursive_mutex ⭐⭐
└── memory_order         ⭐⭐
```

---

## 3. 核心概念与常用类

### 3.1 线程管理 - `std::thread`

**作用：** 创建和管理线程

```cpp
#include <thread>
#include <iostream>

void work(int id) {
    std::cout << "Worker " << id << " running\n";
}

int main() {
    // 创建线程
    std::thread t1(work, 1);
    std::thread t2(work, 2);

    // 等待线程结束
    t1.join();  // 必须 join 或 detach，否则程序崩溃！
    t2.join();

    return 0;
}
```

**关键点：**

- `join()` - 等待线程执行完成
- `detach()` - 分离线程，让它独立运行
- ⚠️ 线程对象析构前必须 join 或 detach，否则 `std::terminate()`

---

### 3.2 互斥锁 - `std::mutex`

**作用：** 保护共享资源，防止数据竞争

```cpp
#include <mutex>
#include <thread>

std::mutex mtx;
int shared_counter = 0;

void increment() {
    for (int i = 0; i < 1000; i++) {
        mtx.lock();           // 获取锁
        shared_counter++;     // 临界区
        mtx.unlock();         // 释放锁
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << shared_counter << std::endl;  // 2000
}
```

**问题：** 如果 `shared_counter++` 抛异常，`unlock()` 不会执行！→ 死锁

---

### 3.3 ⭐⭐⭐ `std::lock_guard` vs `std::unique_lock` (面试重点！)

这就是你面试被问到的！

#### 3.3.1 `std::lock_guard` - 简单自动锁

**特点：**

- ✅ RAII 风格，构造时加锁，析构时自动解锁
- ✅ 最简单、最常用
- ❌ 功能有限，不能手动解锁
- ❌ 不能转移所有权
- ❌ 不能与条件变量配合使用

```cpp
#include <mutex>

std::mutex mtx;
int shared_data = 0;

void safe_increment() {
    std::lock_guard<std::mutex> lock(mtx);  // 构造时加锁
    shared_data++;
    // 即使这里抛异常，lock 析构时也会自动解锁！
}  // lock 析构，自动解锁
```

**使用场景：** 简单的临界区保护，不需要灵活控制

#### 3.3.2 `std::unique_lock` - 灵活智能锁

**特点：**

- ✅ 所有 lock_guard 的功能
- ✅ **可以手动 lock/unlock**（灵活！）
- ✅ **可以转移所有权**（移动语义）
- ✅ **必须与条件变量配合使用**（重要！）
- ✅ 支持延迟加锁、尝试加锁、超时加锁
- ❌ 性能稍差（有一点点开销）

```cpp
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void wait_for_signal() {
    std::unique_lock<std::mutex> lock(mtx);  // 必须用 unique_lock！

    // 条件变量 wait 会：
    // 1. 自动解锁 mtx
    // 2. 线程休眠
    // 3. 被唤醒后重新加锁
    cv.wait(lock, []{ return ready; });

    // 这里可以手动解锁
    lock.unlock();

    // 做一些不需要锁的工作...

    // 再次加锁
    lock.lock();

}  // lock 析构，自动解锁
```

**使用场景：**

- 与条件变量配合（必须！）
- 需要手动控制加锁/解锁时机
- 需要转移锁的所有权

---

### 3.4 ⭐ 面试对比表格 (重点记忆！)

| 特性                           | `std::lock_guard`  | `std::unique_lock` |
| ------------------------------ | ------------------ | ------------------ |
| **自动加锁/解锁**              | ✅                 | ✅                 |
| **RAII 风格**                  | ✅                 | ✅                 |
| **手动 unlock**                | ❌                 | ✅                 |
| **手动 lock**                  | ❌                 | ✅                 |
| **可移动**                     | ❌                 | ✅                 |
| **与 condition_variable 配合** | ❌                 | ✅ (必须)          |
| **延迟加锁**                   | ❌                 | ✅                 |
| **尝试加锁**                   | ❌                 | ✅                 |
| **超时加锁**                   | ❌                 | ✅                 |
| **性能**                       | 更快（无额外开销） | 稍慢（有状态标志） |
| **使用场景**                   | 简单临界区         | 复杂场景、条件变量 |

**记忆口诀：**

- `lock_guard` = 简单粗暴，一锁到底
- `unique_lock` = 灵活强大，必须配合条件变量

---

### 3.5 `std::condition_variable` - 条件变量

**作用：** 线程间通信，等待某个条件成立

**典型场景：** 生产者-消费者模式

```cpp
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;

// 生产者
void producer() {
    for (int i = 0; i < 10; i++) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            data_queue.push(i);
            std::cout << "Produced: " << i << std::endl;
        }
        cv.notify_one();  // 通知消费者
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// 消费者
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);  // 必须用 unique_lock！

        // 等待队列非空
        cv.wait(lock, []{ return !data_queue.empty(); });

        int value = data_queue.front();
        data_queue.pop();
        lock.unlock();  // 提前解锁

        std::cout << "Consumed: " << value << std::endl;

        if (value == 9) break;
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();
}
```

**核心理解：**

```cpp
cv.wait(lock, condition);

// 等价于：
while (!condition()) {
    // 1. 释放锁
    // 2. 线程休眠（不占 CPU）
    // 3. 等待 notify
    // 4. 被唤醒后重新获取锁
}
```

**为什么必须用 `unique_lock`？**

- 因为 `wait()` 需要能够 unlock 和 lock
- `lock_guard` 不支持 unlock，无法使用！

---

### 3.6 `std::atomic` - 原子操作

**作用：** 无锁的原子操作，适合简单数据类型

```cpp
#include <atomic>
#include <thread>

std::atomic<int> counter(0);  // 原子整数

void increment() {
    for (int i = 0; i < 1000; i++) {
        counter++;  // 原子操作，不需要锁！
        // 等价于: counter.fetch_add(1);
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << std::endl;  // 2000
}
```

**什么时候用 atomic？**

- ✅ 简单的计数器、标志位
- ✅ 无锁编程（高性能）
- ❌ 复杂数据结构（用 mutex）

---

## 4. 面试高频问题

### 4.1 ⭐⭐⭐ `lock_guard` vs `unique_lock`？

**标准答案模板：**

> **相同点：**
>
> 1. 都是 RAII 风格的锁管理器
> 2. 构造时自动加锁，析构时自动解锁
> 3. 都可以防止忘记解锁导致的死锁
>
> **不同点：**
>
> 1. **灵活性**：`unique_lock` 可以手动 lock/unlock，`lock_guard` 不行
> 2. **条件变量**：只有 `unique_lock` 可以与 `condition_variable` 配合
> 3. **性能**：`lock_guard` 更轻量，`unique_lock` 有额外状态标志
> 4. **所有权**：`unique_lock` 可以移动，`lock_guard` 不行
>
> **使用建议：**
>
> - 简单临界区 → `lock_guard`（更快）
> - 需要条件变量 → `unique_lock`（必须）
> - 需要灵活控制 → `unique_lock`

### 4.2 为什么 TimerManager 项目用 `unique_lock`？

**答案：**

```cpp
// 因为要配合条件变量！
void TimerManager_After::workerLoop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(timersMutex_);  // 必须！

        if (timers_.empty()) {
            cv_.wait(lock);  // wait 需要能 unlock
        }

        auto nextExpire = getNextExpireTime();
        cv_.wait_until(lock, nextExpire);  // wait_until 也需要

        lock.unlock();  // 提前解锁，避免回调时持有锁
        checkAndFireTimers();
    }
}
```

### 4.3 什么是死锁？如何避免？

**死锁：** 两个或多个线程互相等待对方释放资源

```cpp
// 死锁示例
std::mutex mtx1, mtx2;

void thread1() {
    mtx1.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mtx2.lock();  // 等待 thread2 释放 mtx2
    // ...
    mtx2.unlock();
    mtx1.unlock();
}

void thread2() {
    mtx2.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mtx1.lock();  // 等待 thread1 释放 mtx1 → 死锁！
    // ...
    mtx1.unlock();
    mtx2.unlock();
}
```

**避免死锁的方法：**

1. **固定加锁顺序**（最常用）
2. 使用 `std::lock()` 同时获取多个锁
3. 使用 `std::scoped_lock`（C++17，推荐）
4. 避免嵌套锁
5. 使用超时机制

```cpp
// 解决方案 1: 固定顺序
void thread1() {
    mtx1.lock();  // 总是先锁 mtx1
    mtx2.lock();  // 再锁 mtx2
    // ...
}

void thread2() {
    mtx1.lock();  // 同样先锁 mtx1
    mtx2.lock();  // 再锁 mtx2
    // ...
}

// 解决方案 2: std::scoped_lock (C++17)
void thread1() {
    std::scoped_lock lock(mtx1, mtx2);  // 自动避免死锁
    // ...
}
```

### 4.4 什么是数据竞争（Data Race）？

**数据竞争：** 多个线程同时访问同一数据，至少有一个是写操作，且没有同步

```cpp
int counter = 0;  // 共享变量

void increment() {
    counter++;  // 数据竞争！(读-改-写 不是原子的)
}

// 两个线程同时调用 increment() → 结果不确定！
```

**解决方案：**

```cpp
// 方案 1: 使用 mutex
std::mutex mtx;
void increment() {
    std::lock_guard<std::mutex> lock(mtx);
    counter++;
}

// 方案 2: 使用 atomic
std::atomic<int> counter(0);
void increment() {
    counter++;  // 原子操作
}
```

---

## 5. 实战应用场景

### 5.1 什么时候用哪种锁？

| 场景                   | 推荐方案                   | 原因           |
| ---------------------- | -------------------------- | -------------- |
| 简单计数器             | `std::atomic`              | 无锁，性能最好 |
| 简单临界区（几行代码） | `std::lock_guard`          | 简单高效       |
| 需要提前解锁           | `std::unique_lock`         | 可手动控制     |
| 配合条件变量           | `std::unique_lock`         | 必须           |
| 多个锁同时获取         | `std::scoped_lock` (C++17) | 自动避免死锁   |
| 读多写少               | `std::shared_mutex`        | 允许多个读者   |

### 5.2 TimerManager 中的多线程技术

```cpp
class TimerManager_After {
private:
    std::map<TimerId, std::shared_ptr<TimerInfo>> timers_;
    std::mutex timersMutex_;              // ← 互斥锁
    std::condition_variable cv_;          // ← 条件变量
    std::thread workerThread_;            // ← 工作线程
    std::atomic<bool> running_;           // ← 原子标志

    void workerLoop() {
        while (running_) {                // ← atomic 读取
            std::unique_lock<std::mutex> lock(timersMutex_);  // ← unique_lock（配合 cv）

            if (timers_.empty()) {
                cv_.wait(lock);           // ← 条件变量等待
            }

            auto nextExpire = getNextExpireTime();
            cv_.wait_until(lock, nextExpire);  // ← 超时等待

            lock.unlock();                // ← 手动解锁
            checkAndFireTimers();
        }
    }

public:
    void registerTimer(...) {
        {
            std::lock_guard<std::mutex> lock(timersMutex_);  // ← lock_guard（简单）
            timers_[id] = timerInfo;
        }
        cv_.notify_one();                 // ← 通知工作线程
    }
};
```

**用到的技术：**

1. ✅ `std::thread` - 工作线程
2. ✅ `std::mutex` - 保护共享数据
3. ✅ `std::lock_guard` - 简单临界区
4. ✅ `std::unique_lock` - 配合条件变量
5. ✅ `std::condition_variable` - 线程通信
6. ✅ `std::atomic` - 运行标志

---

## 6. 学习路线建议

### 阶段 1：基础（1-2 周）

```
✅ 掌握 std::thread 基本用法
✅ 理解数据竞争和临界区
✅ 熟练使用 std::mutex + std::lock_guard
✅ 练习简单的多线程程序
```

### 阶段 2：进阶（2-3 周）

```
✅ 深入理解 lock_guard vs unique_lock ⭐
✅ 掌握 std::condition_variable
✅ 实现生产者-消费者模式
✅ 理解死锁及避免方法
✅ 学习 std::atomic 基本用法
```

### 阶段 3：实战（持续）

```
✅ 分析 TimerManager 项目的多线程设计
✅ 实现线程池
✅ 学习 C++17/20 新特性 (scoped_lock, jthread)
✅ 研究无锁数据结构
```

---

## 7. 面试准备清单

### 必须掌握（⭐⭐⭐⭐⭐）

- [ ] `lock_guard` vs `unique_lock` 区别
- [ ] `condition_variable` 使用场景
- [ ] 数据竞争是什么
- [ ] 死锁是什么，如何避免
- [ ] `std::atomic` 适用场景

### 应该了解（⭐⭐⭐⭐）

- [ ] 为什么条件变量必须用 `unique_lock`
- [ ] `notify_one()` vs `notify_all()`
- [ ] 虚假唤醒（spurious wakeup）
- [ ] RAII 在多线程中的应用

### 加分项（⭐⭐⭐）

- [ ] `std::shared_mutex` 读写锁
- [ ] `std::scoped_lock` (C++17)
- [ ] `std::jthread` (C++20)
- [ ] Memory Order 内存序

---

## 8. 推荐资源

### 书籍

1. **《C++ Concurrency in Action》** - Anthony Williams（圣经级）
2. **《Effective Modern C++》** - Scott Meyers（条款 37-40）

### 在线资源

- [cppreference.com](https://en.cppreference.com/w/cpp/thread) - 权威参考
- [现代 C++ 教程](https://changkun.de/modern-cpp/)

### 实战项目

- ✅ 你的 TimerManager 项目（已有！）
- 简单线程池实现
- 生产者-消费者队列

---

## 总结：面试回答模板

**面试官："`lock_guard` 和 `unique_lock` 有什么区别？"**

**你的回答：**

> "这两个都是 C++11 提供的 RAII 风格的锁管理器，主要区别在于灵活性：
>
> `lock_guard` 更简单轻量，构造时加锁，析构时解锁，中间不能手动控制，适合简单的临界区保护。
>
> `unique_lock` 功能更强大，支持手动 lock/unlock，可以转移所有权，最重要的是它能与条件变量配合使用。在我的 TimerManager 项目中，因为需要使用 `condition_variable` 来实现智能等待，所以必须使用 `unique_lock`，因为条件变量的 wait 操作需要能够释放和重新获取锁。
>
> 性能上，`lock_guard` 因为没有额外状态，会稍快一点；但在需要灵活控制或条件变量的场景，`unique_lock` 是必需的。"

---

**加油！多线程是 C++ 进阶的关键技能！** 🚀
