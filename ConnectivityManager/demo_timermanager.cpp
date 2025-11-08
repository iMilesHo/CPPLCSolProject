#include "TimerManager_Before.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <atomic>

// 用于演示的全局计数器
std::atomic<int> taskExecutionCount{0};

// 辅助函数：打印当前时间戳
void printTimestamp(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::cout << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") 
              << "." << std::setfill('0') << std::setw(3) << ms.count() 
              << "] " << message << std::endl;
}

/**
 * 模拟车载系统中的典型场景
 */
void demonstrateTimerManagerUsage() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TimerManager Before Optimization Demo" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    TimerManager_Before timerMgr;
    timerMgr.start();
    
    std::cout << "\n--- Scenario 1: 单次定时器 ---" << std::endl;
    printTimestamp("Registering one-shot timers");
    
    // 场景1: 模拟娱乐系统延迟启动
    auto timer1 = timerMgr.registerTimer(500, []() {
        printTimestamp("🎵 Entertainment System: Starting media service");
        taskExecutionCount++;
    });
    
    // 场景2: 模拟诊断系统定时数据上报
    auto timer2 = timerMgr.registerTimer(1000, []() {
        printTimestamp("🔧 Diagnostic System: Uploading vehicle data");
        taskExecutionCount++;
    });
    
    // 场景3: 模拟网络连接超时检测
    auto timer3 = timerMgr.registerTimer(1500, []() {
        printTimestamp("📡 Network Manager: Connection timeout check");
        taskExecutionCount++;
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
    std::cout << "\n--- Scenario 2: 周期性定时器 ---" << std::endl;
    printTimestamp("Registering periodic timers");
    
    // 场景4: 模拟网络心跳检测 (每800ms)
    auto periodicTimer1 = timerMgr.registerPeriodicTimer(800, []() {
        printTimestamp("💓 Network Heartbeat: Checking connectivity");
        taskExecutionCount++;
    });
    
    // 场景5: 模拟TCU模块保活 (每1200ms)
    auto periodicTimer2 = timerMgr.registerPeriodicTimer(1200, []() {
        printTimestamp("📶 TCU Module: Keep-alive signal");
        taskExecutionCount++;
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    std::cout << "\n--- Scenario 3: 动态取消定时器 ---" << std::endl;
    printTimestamp("Cancelling periodic timer #" + std::to_string(periodicTimer1));
    timerMgr.cancelTimer(periodicTimer1);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
    std::cout << "\n--- Scenario 4: 高负载场景 (多个定时器) ---" << std::endl;
    printTimestamp("Registering multiple timers to demonstrate polling overhead");
    
    // 注册多个定时器模拟高负载
    for (int i = 0; i < 5; i++) {
        timerMgr.registerTimer(3000 + i * 100, [i]() {
            printTimestamp("⚙️  Task #" + std::to_string(i) + " executed");
            taskExecutionCount++;
        });
    }
    
    std::cout << "\n💡 Notice: Even with timers set for 3+ seconds later," << std::endl;
    std::cout << "    the polling thread wakes up every 100ms to check." << std::endl;
    std::cout << "    This is the CPU waste we want to optimize!\n" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    
    std::cout << "\n--- Final Statistics ---" << std::endl;
    std::cout << "Active timers remaining: " << timerMgr.getActiveTimerCount() << std::endl;
    std::cout << "Total tasks executed: " << taskExecutionCount.load() << std::endl;
    
    std::cout << "\n--- Stopping TimerManager ---" << std::endl;
    timerMgr.stop();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Demo Completed" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    std::cout << "\n📊 Performance Issues with Polling Approach:" << std::endl;
    std::cout << "  ❌ Thread wakes up every 100ms regardless of timer schedule" << std::endl;
    std::cout << "  ❌ CPU cycles wasted on unnecessary checks" << std::endl;
    std::cout << "  ❌ Increased context switching overhead" << std::endl;
    std::cout << "  ❌ Poor power efficiency (critical for automotive)" << std::endl;
    std::cout << "\n✅ Next Step: Optimize with condition_variable mechanism" << std::endl;
    std::cout << "  - Sleep until next actual timer expiration" << std::endl;
    std::cout << "  - Wake up only when necessary (new timer or expiration)" << std::endl;
    std::cout << "  - Dramatically reduce CPU usage\n" << std::endl;
}

int main() {
    try {
        demonstrateTimerManagerUsage();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
