#include "TimerManager_Before.h"
#include <iostream>
#include <iomanip>

// 定义静态常量
constexpr uint32_t TimerManager_Before::POLLING_INTERVAL_MS;

TimerManager_Before::TimerManager_Before()
    : running_(false)
    , nextTimerId_(1) {
}

TimerManager_Before::~TimerManager_Before() {
    stop();
}

void TimerManager_Before::start() {
    if (running_.exchange(true)) {
        return; // 已经在运行
    }
    
    std::cout << "[TimerManager_Before] Starting with POLLING mode (interval: " 
              << POLLING_INTERVAL_MS << "ms)" << std::endl;
    
    pollingThread_ = std::thread(&TimerManager_Before::pollingLoop, this);
}

void TimerManager_Before::stop() {
    if (!running_.exchange(false)) {
        return; // 已经停止
    }
    
    std::cout << "[TimerManager_Before] Stopping..." << std::endl;
    
    if (pollingThread_.joinable()) {
        pollingThread_.join();
    }
    
    std::lock_guard<std::mutex> lock(timersMutex_);
    timers_.clear();
}

TimerManager_Before::TimerId TimerManager_Before::registerTimer(
    uint32_t delayMs, TimerCallback callback) {
    
    TimerId id = generateTimerId();
    auto expireTime = std::chrono::steady_clock::now() + 
                      std::chrono::milliseconds(delayMs);
    
    auto timerInfo = std::make_shared<TimerInfo>(
        id, 
        expireTime, 
        std::chrono::milliseconds(delayMs),
        std::move(callback),
        false  // 非周期性
    );
    
    {
        std::lock_guard<std::mutex> lock(timersMutex_);
        timers_[id] = timerInfo;
    }
    
    std::cout << "[TimerManager_Before] Registered one-shot timer #" << id 
              << " with delay " << delayMs << "ms" << std::endl;
    
    return id;
}

TimerManager_Before::TimerId TimerManager_Before::registerPeriodicTimer(
    uint32_t intervalMs, TimerCallback callback) {
    
    TimerId id = generateTimerId();
    auto expireTime = std::chrono::steady_clock::now() + 
                      std::chrono::milliseconds(intervalMs);
    
    auto timerInfo = std::make_shared<TimerInfo>(
        id, 
        expireTime, 
        std::chrono::milliseconds(intervalMs),
        std::move(callback),
        true  // 周期性
    );
    
    {
        std::lock_guard<std::mutex> lock(timersMutex_);
        timers_[id] = timerInfo;
    }
    
    std::cout << "[TimerManager_Before] Registered periodic timer #" << id 
              << " with interval " << intervalMs << "ms" << std::endl;
    
    return id;
}

bool TimerManager_Before::cancelTimer(TimerId id) {
    std::lock_guard<std::mutex> lock(timersMutex_);
    
    auto it = timers_.find(id);
    if (it != timers_.end()) {
        std::cout << "[TimerManager_Before] Cancelled timer #" << id << std::endl;
        timers_.erase(it);
        return true;
    }
    
    return false;
}

size_t TimerManager_Before::getActiveTimerCount() const {
    std::lock_guard<std::mutex> lock(timersMutex_);
    return timers_.size();
}

TimerManager_Before::TimerId TimerManager_Before::generateTimerId() {
    return nextTimerId_.fetch_add(1);
}

/**
 * 这是优化前的核心问题所在：轮询循环
 * 
 * 问题：
 * 1. 线程会每隔POLLING_INTERVAL_MS(100ms)醒来一次检查
 * 2. 即使没有任何定时器,或者最近的定时器还有很久才到期,线程也会频繁醒来
 * 3. 造成不必要的CPU资源浪费和上下文切换
 * 4. 在车载系统中,这种持续轮询会影响功耗和系统响应
 */
void TimerManager_Before::pollingLoop() {
    std::cout << "[TimerManager_Before] Polling thread started" << std::endl;
    
    uint64_t pollCount = 0;
    
    while (running_) {
        // 问题点1: 固定间隔sleep,不管是否真的需要
        std::this_thread::sleep_for(std::chrono::milliseconds(POLLING_INTERVAL_MS));
        
        pollCount++;
        
        // 问题点2: 每次醒来都要检查所有定时器
        checkAndFireTimers();
        
        // 为了演示问题,每10次轮询打印一次统计
        if (pollCount % 10 == 0) {
            std::lock_guard<std::mutex> lock(timersMutex_);
            std::cout << "[TimerManager_Before] Poll #" << pollCount 
                      << " - Active timers: " << timers_.size() 
                      << " (CPU wasted by polling)" << std::endl;
        }
    }
    
    std::cout << "[TimerManager_Before] Polling thread stopped after " 
              << pollCount << " polls" << std::endl;
}

void TimerManager_Before::checkAndFireTimers() {
    auto now = std::chrono::steady_clock::now();
    
    std::vector<std::shared_ptr<TimerInfo>> expiredTimers;
    
    // 遍历所有定时器查找过期的
    {
        std::lock_guard<std::mutex> lock(timersMutex_);
        
        // 问题点3: 每次轮询都要遍历整个map
        for (auto it = timers_.begin(); it != timers_.end(); ) {
            auto& timer = it->second;
            
            if (now >= timer->expireTime) {
                expiredTimers.push_back(timer);
                
                if (timer->isPeriodic) {
                    // 周期性定时器,更新下次触发时间
                    timer->expireTime = now + timer->interval;
                    ++it;
                } else {
                    // 单次定时器,移除
                    it = timers_.erase(it);
                }
            } else {
                ++it;
            }
        }
    }
    
    // 触发回调 (在锁外执行避免死锁)
    for (auto& timer : expiredTimers) {
        std::cout << "[TimerManager_Before] Timer #" << timer->id 
                  << " expired, firing callback" << std::endl;
        try {
            timer->callback();
        } catch (const std::exception& e) {
            std::cerr << "[TimerManager_Before] Exception in timer #" 
                      << timer->id << " callback: " << e.what() << std::endl;
        }
    }
}
