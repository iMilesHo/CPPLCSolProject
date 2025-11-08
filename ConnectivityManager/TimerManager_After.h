#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <memory>

/**
 * TimerManager - 优化后版本
 * 使用 std::condition_variable 条件变量机制
 * 动态计算下一个到期时间，只在必要时唤醒线程
 * 显著降低 CPU 使用率和功耗
 */
class TimerManager_After {
public:
    using TimerCallback = std::function<void()>;
    using TimerId = uint64_t;
    
    struct TimerInfo {
        TimerId id;
        std::chrono::steady_clock::time_point expireTime;
        std::chrono::milliseconds interval;
        TimerCallback callback;
        bool isPeriodic;
        bool isActive;
        
        TimerInfo(TimerId id, 
                  std::chrono::steady_clock::time_point expire,
                  std::chrono::milliseconds interval,
                  TimerCallback cb,
                  bool periodic)
            : id(id)
            , expireTime(expire)
            , interval(interval)
            , callback(std::move(cb))
            , isPeriodic(periodic)
            , isActive(true) {}
    };
    
    TimerManager_After();
    ~TimerManager_After();
    
    // 禁止拷贝
    TimerManager_After(const TimerManager_After&) = delete;
    TimerManager_After& operator=(const TimerManager_After&) = delete;
    
    /**
     * 注册单次定时器
     * @param delay 延迟时间(毫秒)
     * @param callback 回调函数
     * @return 定时器ID
     */
    TimerId registerTimer(uint32_t delayMs, TimerCallback callback);
    
    /**
     * 注册周期性定时器
     * @param intervalMs 周期间隔(毫秒)
     * @param callback 回调函数
     * @return 定时器ID
     */
    TimerId registerPeriodicTimer(uint32_t intervalMs, TimerCallback callback);
    
    /**
     * 取消定时器
     * @param id 定时器ID
     * @return 是否成功取消
     */
    bool cancelTimer(TimerId id);
    
    /**
     * 获取活跃定时器数量
     */
    size_t getActiveTimerCount() const;
    
    /**
     * 启动定时器管理器
     */
    void start();
    
    /**
     * 停止定时器管理器
     */
    void stop();

private:
    /**
     * 优化后的工作线程 - 使用条件变量智能等待
     * 只在以下情况唤醒：
     * 1. 最近的定时器到期
     * 2. 新定时器注册（可能需要提前唤醒）
     * 3. 定时器被取消（可能需要重新计算等待时间）
     */
    void workerLoop();
    
    /**
     * 检查并触发超时的定时器
     * @return 下一个最近到期的时间点（如果有的话）
     */
    std::chrono::steady_clock::time_point checkAndFireTimers();
    
    /**
     * 获取下一个最近到期的时间点
     */
    std::chrono::steady_clock::time_point getNextExpireTime() const;
    
    /**
     * 通知工作线程（有新定时器或取消）
     */
    void notifyWorker();
    
    TimerId generateTimerId();
    
private:
    std::map<TimerId, std::shared_ptr<TimerInfo>> timers_;
    mutable std::mutex timersMutex_;
    
    // 条件变量 - 核心优化点
    std::condition_variable cv_;
    
    std::thread workerThread_;
    std::atomic<bool> running_;
    
    std::atomic<TimerId> nextTimerId_;
};
