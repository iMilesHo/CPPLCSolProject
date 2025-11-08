#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>

/**
 * RAII (Resource Acquisition Is Initialization) 实战示例
 * 编译: g++ -std=c++14 -pthread raii_demo.cpp -o raii_demo
 */

// ============================================================
// 示例 1: 传统方式 vs RAII 方式对比
// ============================================================

// ❌ 传统 C 风格 - 容易出错
void bad_file_handling() {
    std::cout << "\n=== ❌ 传统方式（容易出错）===" << std::endl;
    
    FILE* file = fopen("test.txt", "w");
    if (!file) {
        std::cout << "文件打开失败" << std::endl;
        return;
    }
    
    std::cout << "文件已打开" << std::endl;
    
    // 如果这里抛异常或提前 return，文件永远不会关闭！
    fprintf(file, "Hello World\n");
    
    // 必须记得关闭（容易忘记）
    fclose(file);
    std::cout << "文件已关闭（手动）" << std::endl;
}

// ✅ RAII 方式 - 自动管理
void good_file_handling() {
    std::cout << "\n=== ✅ RAII 方式（自动管理）===" << std::endl;
    
    {
        std::ofstream file("test.txt");  // 构造时打开
        std::cout << "文件已打开（自动）" << std::endl;
        
        file << "Hello World\n";
        
        // 即使这里 return 或抛异常，文件也会自动关闭
        
        std::cout << "即将离开作用域..." << std::endl;
    }  // 析构时自动关闭
    
    std::cout << "文件已关闭（自动）✨" << std::endl;
}

// ============================================================
// 示例 2: 自定义 RAII 类 - 文件管理器
// ============================================================

class FileRAII {
public:
    explicit FileRAII(const char* filename) {
        file_ = fopen(filename, "w");
        if (file_) {
            std::cout << "  ✅ [FileRAII] 文件已打开: " << filename << std::endl;
        }
    }
    
    ~FileRAII() {
        if (file_) {
            fclose(file_);
            std::cout << "  ✅ [FileRAII] 文件已关闭（析构函数自动调用）" << std::endl;
        }
    }
    
    // 禁止拷贝
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
    
    void write(const char* text) {
        if (file_) {
            fprintf(file_, "%s", text);
        }
    }
    
private:
    FILE* file_;
};

void demo_custom_raii() {
    std::cout << "\n=== 示例 2: 自定义 RAII 类 ===" << std::endl;
    
    FileRAII file("demo.txt");
    file.write("RAII is awesome!\n");
    
    std::cout << "  正在使用文件..." << std::endl;
    
    // 不需要手动关闭，离开作用域时自动关闭
}

// ============================================================
// 示例 3: RAII 锁管理
// ============================================================

std::mutex g_mutex;
int g_counter = 0;

// ❌ 手动锁管理 - 容易死锁
void bad_lock_increment() {
    g_mutex.lock();
    std::cout << "  [Bad] 已加锁" << std::endl;
    
    g_counter++;
    
    // 如果这里 return 或抛异常，锁永远不会释放！→ 死锁
    
    g_mutex.unlock();
    std::cout << "  [Bad] 已解锁（手动）" << std::endl;
}

// ✅ RAII 锁管理 - 自动解锁
void good_lock_increment() {
    std::lock_guard<std::mutex> lock(g_mutex);  // 构造时加锁
    std::cout << "  [Good] 已加锁（自动）" << std::endl;
    
    g_counter++;
    
    // 即使这里 return 或抛异常，锁也会自动释放
    
    std::cout << "  [Good] 即将离开作用域..." << std::endl;
}  // 析构时自动解锁

void demo_lock_raii() {
    std::cout << "\n=== 示例 3: RAII 锁管理 ===" << std::endl;
    
    std::cout << "\n传统方式：" << std::endl;
    bad_lock_increment();
    
    std::cout << "\nRAII 方式：" << std::endl;
    good_lock_increment();
    std::cout << "  ✅ [Good] 已解锁（自动）" << std::endl;
}

// ============================================================
// 示例 4: RAII 计时器
// ============================================================

class Timer {
public:
    explicit Timer(const std::string& name) : name_(name) {
        start_ = std::chrono::steady_clock::now();
        std::cout << "  ⏱️  [" << name_ << "] 开始计时..." << std::endl;
    }
    
    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start_);
        std::cout << "  ⏱️  [" << name_ << "] 耗时: " 
                  << duration.count() << "ms ✨" << std::endl;
    }
    
private:
    std::string name_;
    std::chrono::steady_clock::time_point start_;
};

void slow_function() {
    Timer t("slow_function");  // 构造时开始计时
    
    std::cout << "  执行一些耗时操作..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
}  // 析构时自动打印耗时

void demo_timer_raii() {
    std::cout << "\n=== 示例 4: RAII 计时器 ===" << std::endl;
    slow_function();
}

// ============================================================
// 示例 5: 异常安全性
// ============================================================

void demo_exception_safety() {
    std::cout << "\n=== 示例 5: RAII 的异常安全性 ===" << std::endl;
    
    try {
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "  已加锁" << std::endl;
        
        std::cout << "  即将抛出异常..." << std::endl;
        throw std::runtime_error("测试异常");
        
        // 这行永远不会执行
        std::cout << "  这行不会打印" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "  ❌ 捕获异常: " << e.what() << std::endl;
        std::cout << "  ✅ 但是锁已经自动释放了（RAII 保证）" << std::endl;
    }
}

// ============================================================
// 示例 6: 智能指针 RAII
// ============================================================

void demo_smart_pointer() {
    std::cout << "\n=== 示例 6: 智能指针（RAII）===" << std::endl;
    
    std::cout << "\n❌ 传统方式：" << std::endl;
    {
        int* ptr = new int(42);
        std::cout << "  分配内存: " << *ptr << std::endl;
        
        // 必须记得 delete（容易忘记）
        delete ptr;
        std::cout << "  手动释放内存" << std::endl;
    }
    
    std::cout << "\n✅ RAII 方式（智能指针）：" << std::endl;
    {
        auto ptr = std::make_unique<int>(42);
        std::cout << "  分配内存: " << *ptr << std::endl;
        
        // 不需要 delete，离开作用域自动释放
        std::cout << "  即将离开作用域..." << std::endl;
    }  // unique_ptr 析构，自动 delete
    std::cout << "  ✅ 内存已自动释放" << std::endl;
}

// ============================================================
// 示例 7: 作用域守卫（Scope Guard）
// ============================================================

class ScopeGuard {
public:
    template<typename Func>
    explicit ScopeGuard(Func&& cleanup) : cleanup_(std::forward<Func>(cleanup)) {
        std::cout << "  [ScopeGuard] 已创建" << std::endl;
    }
    
    ~ScopeGuard() {
        std::cout << "  [ScopeGuard] 执行清理操作..." << std::endl;
        cleanup_();
    }
    
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    
private:
    std::function<void()> cleanup_;
};

void demo_scope_guard() {
    std::cout << "\n=== 示例 7: 作用域守卫（高级 RAII）===" << std::endl;
    
    {
        std::cout << "  进入作用域" << std::endl;
        
        // 创建守卫，离开作用域时自动执行清理
        ScopeGuard guard([]() {
            std::cout << "  ✨ 清理：关闭连接、释放资源..." << std::endl;
        });
        
        std::cout << "  执行主要逻辑..." << std::endl;
        
        std::cout << "  即将离开作用域" << std::endl;
    }  // guard 析构，自动执行清理函数
    
    std::cout << "  已离开作用域" << std::endl;
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║      RAII 实战示例                            ║" << std::endl;
    std::cout << "║      (Resource Acquisition Is Initialization) ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
    
    // 1. 文件管理对比
    bad_file_handling();
    good_file_handling();
    
    // 2. 自定义 RAII 类
    demo_custom_raii();
    
    // 3. 锁管理
    demo_lock_raii();
    
    // 4. 计时器
    demo_timer_raii();
    
    // 5. 异常安全性
    demo_exception_safety();
    
    // 6. 智能指针
    demo_smart_pointer();
    
    // 7. 作用域守卫
    demo_scope_guard();
    
    std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║      RAII 核心思想                            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\n构造函数 → 获取资源（打开文件、加锁、分配内存...）" << std::endl;
    std::cout << "析构函数 → 释放资源（关闭文件、解锁、释放内存...）" << std::endl;
    std::cout << "\n优势：" << std::endl;
    std::cout << "  ✅ 自动管理 - 不需要手动释放" << std::endl;
    std::cout << "  ✅ 异常安全 - 即使抛异常也会释放" << std::endl;
    std::cout << "  ✅ 不易出错 - 编译器保证析构函数被调用" << std::endl;
    std::cout << "  ✅ 代码简洁 - 减少重复的清理代码" << std::endl;
    std::cout << "\n常见 RAII 类：" << std::endl;
    std::cout << "  • std::unique_ptr / shared_ptr (内存)" << std::endl;
    std::cout << "  • std::lock_guard / unique_lock (锁)" << std::endl;
    std::cout << "  • std::ifstream / ofstream (文件)" << std::endl;
    std::cout << "  • std::thread (线程)" << std::endl;
    std::cout << std::endl;
    
    return 0;
}
