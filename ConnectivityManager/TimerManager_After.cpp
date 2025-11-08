#include "TimerManager_After.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

TimerManager_After::TimerManager_After()
    : running_(false)
    , nextTimerId_(1) {
}

TimerManager_After::~TimerManager_After() {
    stop();
}

void TimerManager_After::start() {
    if (running_.exchange(true)) {
        return; // 已经在运行
    }
    
    std::cout << "[TimerManager_After] Starting with CONDITION_VARIABLE mode (smart wait)" << std::endl;
    
    workerThread_ = std::thread(&TimerManager_After::workerLoop, this);
}

void TimerManager_After::stop() {
    if (!running_.exchange(false)) {
        return; // 已经停止
    }
    
    std::cout << "[TimerManager_After] Stopping..." << std::endl;
    
    // 唤醒工作线程让它退出
    cv_.notify_one();
    
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
    
    std::lock_guard<std::mutex> lock(timersMutex_);
    timers_.clear();
}

TimerManager_After::TimerId TimerManager_After::registerTimer(
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
    
    std::cout << "[TimerManager_After] Registered one-shot timer #" << id 
              << " with delay " << delayMs << "ms" << std::endl;
    
    // 关键优化：通知工作线程有新定时器，可能需要提前唤醒
    notifyWorker();
    
    return id;
}

TimerManager_After::TimerId TimerManager_After::registerPeriodicTimer(
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
    
    std::cout << "[TimerManager_After] Registered periodic timer #" << id 
              << " with interval " << intervalMs << "ms" << std::endl;
    
    // 关键优化：通知工作线程有新定时器
    notifyWorker();
    
    return id;
}

bool TimerManager_After::cancelTimer(TimerId id) {
    bool cancelled = false;
    
    {
        std::lock_guard<std::mutex> lock(timersMutex_);
        auto it = timers_.find(id);
        if (it != timers_.end()) {
            std::cout << "[TimerManager_After] Cancelled timer #" << id << std::endl;
            timers_.erase(it);
            cancelled = true;
        }
    }
    
    if (cancelled) {
        // 关键优化：通知工作线程重新计算等待时间
        notifyWorker();
    }
    
    return cancelled;
}

size_t TimerManager_After::getActiveTimerCount() const {
    std::lock_guard<std::mutex> lock(timersMutex_);
    return timers_.size();
}

TimerManager_After::TimerId TimerManager_After::generateTimerId() {
    return nextTimerId_.fetch_add(1);
}

void TimerManager_After::notifyWorker() {
    cv_.notify_one();
}

/**
 * 优化后的核心实现：使用条件变量智能等待
 * 
 * 优化点：
 * 1. 动态计算下一个最近到期的定时器时间
 * 2. 使用 cv_.wait_until() 精确等待到该时间点
 * 3. 当有新定时器注册或取消时，通过 notify_one() 提前唤醒
 * 4. 没有定时器时完全休眠，零 CPU 占用
 * 5. 避免了不必要的轮询和上下文切换
 */
void TimerManager_After::workerLoop() {
    std::cout << "[TimerManager_After] Worker thread started" << std::endl;
    
    uint64_t wakeupCount = 0;
    
    while (running_) {
        std::unique_lock<std::mutex> lock(timersMutex_);
        
        if (timers_.empty()) {
            // 优化点1: 没有定时器时完全休眠，零CPU占用
            std::cout << "[TimerManager_After] No timers, sleeping indefinitely (zero CPU usage)..." << std::endl;
            cv_.wait(lock, [this]() { return !running_ || !timers_.empty(); });
            
            if (!running_) {
                break;
            }
            continue;
        }
        
        // 优化点2: 找到最近到期的定时器
        auto nextExpireTime = getNextExpireTime();
        auto now = std::chrono::steady_clock::now();
        
        if (nextExpireTime > now) {
            // 优化点3: 精确等待到下一个定时器到期时间
            auto waitDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
                nextExpireTime - now);
            
            std::cout << "[TimerManager_After] Sleeping for " << waitDuration.count() 
                      << "ms until next timer expires (smart wait)" << std::endl;
            
            // wait_until 会在以下情况返回：
            // 1. 到达 nextExpireTime（定时器到期）
            // 2. 收到 notify_one()（新定时器注册或取消）
            // 3. 虚假唤醒（spurious wakeup）- 需要重新检查
            cv_.wait_until(lock, nextExpireTime);
            
            if (!running_) {
                break;
            }
        }
        
        wakeupCount++;
        
        // 释放锁，检查并触发到期的定时器
        lock.unlock();
        checkAndFireTimers();
        
        // 为了对比，每5次唤醒打印一次统计
        if (wakeupCount % 5 == 0) {
            std::lock_guard<std::mutex> statLock(timersMutex_);
            std::cout << "[TimerManager_After] Wakeup #" << wakeupCount 
                      << " - Active timers: " << timers_.size() 
                      << " (only wakes when necessary!)" << std::endl;
        }
    }
    
    std::cout << "[TimerManager_After] Worker thread stopped after " 
              << wakeupCount << " wakeups (compare with polling version!)" << std::endl;
}

std::chrono::steady_clock::time_point TimerManager_After::checkAndFireTimers() {
    auto now = std::chrono::steady_clock::now();
    
    std::vector<std::shared_ptr<TimerInfo>> expiredTimers;
    
    // 查找过期的定时器
    {
        std::lock_guard<std::mutex> lock(timersMutex_);
        
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
        std::cout << "[TimerManager_After] Timer #" << timer->id 
                  << " expired, firing callback" << std::endl;
        try {
            timer->callback();
        } catch (const std::exception& e) {
            std::cerr << "[TimerManager_After] Exception in timer #" 
                      << timer->id << " callback: " << e.what() << std::endl;
        }
    }
    
    return getNextExpireTime();
}

std::chrono::steady_clock::time_point TimerManager_After::getNextExpireTime() const {
    // 注意：调用此函数前必须已持有 timersMutex_ 锁
    
    if (timers_.empty()) {
        return std::chrono::steady_clock::time_point::max();
    }
    
    auto minExpireTime = std::chrono::steady_clock::time_point::max();
    
    for (const auto& pair : timers_) {
        if (pair.second->expireTime < minExpireTime) {
            minExpireTime = pair.second->expireTime;
        }
    }
    
    return minExpireTime;
}
