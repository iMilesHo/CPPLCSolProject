#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

/**
 * lock_guard vs unique_lock 实战对比
 * 编译: g++ -std=c++14 -pthread lock_comparison.cpp -o lock_comparison
 */

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// ============================================================
// 示例 1: lock_guard - 简单临界区
// ============================================================
void example_lock_guard() {
    std::cout << "\n=== Example 1: std::lock_guard ===" << std::endl;
    
    int shared_counter = 0;
    
    auto increment = [&]() {
        for (int i = 0; i < 5; i++) {
            {
                std::lock_guard<std::mutex> lock(mtx);  // 构造时加锁
                shared_counter++;
                std::cout << "  Counter: " << shared_counter << std::endl;
                
                // lock 作用域结束，自动解锁
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
    
    std::thread t1(increment);
    std::thread t2(increment);
    
    t1.join();
    t2.join();
    
    std::cout << "Final counter: " << shared_counter << std::endl;
}

// ============================================================
// 示例 2: unique_lock - 手动控制
// ============================================================
void example_unique_lock_manual() {
    std::cout << "\n=== Example 2: std::unique_lock (手动控制) ===" << std::endl;
    
    std::unique_lock<std::mutex> lock(mtx);
    
    std::cout << "持有锁，做一些工作..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // 提前解锁（lock_guard 做不到！）
    lock.unlock();
    std::cout << "已解锁，可以做不需要锁保护的工作" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // 重新加锁（lock_guard 做不到！）
    lock.lock();
    std::cout << "重新加锁，继续需要保护的工作" << std::endl;
    
    // lock 析构时会自动解锁
}

// ============================================================
// 示例 3: unique_lock + condition_variable
// ============================================================
void waiter() {
    std::cout << "\n[Waiter] 等待信号..." << std::endl;
    
    std::unique_lock<std::mutex> lock(mtx);  // 必须用 unique_lock！
    
    // wait 会：
    // 1. 释放锁
    // 2. 线程休眠
    // 3. 被 notify 唤醒后重新获取锁
    cv.wait(lock, []{ return ready; });
    
    std::cout << "[Waiter] 收到信号，继续执行！" << std::endl;
}

void signaler() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "[Signaler] 准备发送信号..." << std::endl;
    
    {
        std::lock_guard<std::mutex> lock(mtx);  // 这里用 lock_guard 就够了
        ready = true;
    }
    
    cv.notify_one();  // 唤醒等待的线程
    std::cout << "[Signaler] 信号已发送" << std::endl;
}

void example_condition_variable() {
    std::cout << "\n=== Example 3: unique_lock + condition_variable ===" << std::endl;
    
    ready = false;  // 重置
    
    std::thread t1(waiter);
    std::thread t2(signaler);
    
    t1.join();
    t2.join();
}

// ============================================================
// 示例 4: 错误示范 - lock_guard 不能用于条件变量
// ============================================================
void example_wrong_usage() {
    std::cout << "\n=== Example 4: 为什么 lock_guard 不能用于条件变量 ===" << std::endl;
    
    std::cout << "❌ 以下代码无法编译：" << std::endl;
    std::cout << "   std::lock_guard<std::mutex> lock(mtx);" << std::endl;
    std::cout << "   cv.wait(lock);  // 编译错误！" << std::endl;
    std::cout << "\n原因：wait() 需要能够 unlock 和 lock，但 lock_guard 不支持" << std::endl;
}

// ============================================================
// 示例 5: 延迟加锁
// ============================================================
void example_deferred_lock() {
    std::cout << "\n=== Example 5: unique_lock 延迟加锁 ===" << std::endl;
    
    // std::defer_lock: 构造时不加锁
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    
    std::cout << "unique_lock 已创建，但还没加锁" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "现在手动加锁..." << std::endl;
    lock.lock();
    
    std::cout << "持有锁，进行工作" << std::endl;
    
    // lock_guard 无法延迟加锁！
}

// ============================================================
// 示例 6: 尝试加锁（非阻塞）
// ============================================================
void example_try_lock() {
    std::cout << "\n=== Example 6: unique_lock 尝试加锁 ===" << std::endl;
    
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
    
    if (lock.owns_lock()) {
        std::cout << "成功获取锁" << std::endl;
    } else {
        std::cout << "锁已被占用，稍后重试" << std::endl;
    }
    
    // lock_guard 无法尝试加锁！
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   lock_guard vs unique_lock 实战对比          ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
    
    example_lock_guard();
    example_unique_lock_manual();
    example_condition_variable();
    example_wrong_usage();
    example_deferred_lock();
    example_try_lock();
    
    std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   总结                                         ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\nlock_guard:" << std::endl;
    std::cout << "  ✅ 简单、轻量、性能好" << std::endl;
    std::cout << "  ✅ 适合简单临界区" << std::endl;
    std::cout << "  ❌ 不能手动控制" << std::endl;
    std::cout << "  ❌ 不能用于条件变量" << std::endl;
    
    std::cout << "\nunique_lock:" << std::endl;
    std::cout << "  ✅ 功能强大、灵活" << std::endl;
    std::cout << "  ✅ 可以手动 lock/unlock" << std::endl;
    std::cout << "  ✅ 必须用于条件变量" << std::endl;
    std::cout << "  ✅ 支持延迟加锁、尝试加锁等" << std::endl;
    std::cout << "  ⚠️  性能稍差（有状态开销）" << std::endl;
    
    std::cout << "\n选择建议:" << std::endl;
    std::cout << "  • 简单保护 → lock_guard" << std::endl;
    std::cout << "  • 条件变量 → unique_lock (必须)" << std::endl;
    std::cout << "  • 灵活控制 → unique_lock" << std::endl;
    std::cout << std::endl;
    
    return 0;
}
