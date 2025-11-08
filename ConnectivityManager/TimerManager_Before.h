#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <memory>

/**
 * TimerManager - 优化前版本
 * 使用轮询(polling)机制不断检查定时器是否超时
 * 存在CPU资源浪费问题
 */
class TimerManager_Before {
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
    
    TimerManager_Before();
    ~TimerManager_Before();
    
    // 禁止拷贝
    TimerManager_Before(const TimerManager_Before&) = delete;
    TimerManager_Before& operator=(const TimerManager_Before&) = delete;
    
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
     * 轮询线程主循环 - 这是优化前的实现
     * 不断循环检查所有定时器是否超时
     */
    void pollingLoop();
    
    /**
     * 检查并触发超时的定时器
     */
    void checkAndFireTimers();
    
    TimerId generateTimerId();
    
private:
    std::map<TimerId, std::shared_ptr<TimerInfo>> timers_;
    mutable std::mutex timersMutex_;
    
    std::thread pollingThread_;
    std::atomic<bool> running_;
    
    std::atomic<TimerId> nextTimerId_;
    
    // 轮询间隔 - 固定100ms检查一次,造成CPU浪费
    static constexpr uint32_t POLLING_INTERVAL_MS = 100;
};
