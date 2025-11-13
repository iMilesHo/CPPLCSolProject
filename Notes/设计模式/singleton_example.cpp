#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>

// 1. 经典的懒汉式单例（线程不安全）
class Singleton {
private:
    static Singleton* instance;
    
    Singleton() {
        std::cout << "Singleton instance created" << std::endl;
    }
    
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
    
    void doSomething() {
        std::cout << "Singleton is doing something..." << std::endl;
    }
    
    static void destroyInstance() {
        delete instance;
        instance = nullptr;
    }
};

Singleton* Singleton::instance = nullptr;

// 2. 线程安全的懒汉式单例（双重检查锁定）
class ThreadSafeSingleton {
private:
    static ThreadSafeSingleton* instance;
    static std::mutex mtx;
    
    ThreadSafeSingleton() {
        std::cout << "ThreadSafeSingleton instance created" << std::endl;
    }
    
    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

public:
    static ThreadSafeSingleton* getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) {
                instance = new ThreadSafeSingleton();
            }
        }
        return instance;
    }
    
    void doSomething() {
        std::cout << "ThreadSafeSingleton is doing something..." << std::endl;
    }
    
    static void destroyInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        delete instance;
        instance = nullptr;
    }
};

ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
std::mutex ThreadSafeSingleton::mtx;

// 3. Meyers单例（推荐方式）
class MeyersSingleton {
private:
    MeyersSingleton() {
        std::cout << "MeyersSingleton instance created" << std::endl;
    }
    
    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;

public:
    static MeyersSingleton& getInstance() {
        static MeyersSingleton instance;
        return instance;
    }
    
    void doSomething() {
        std::cout << "MeyersSingleton is doing something..." << std::endl;
    }
};

// 4. 饿汉式单例
class EagerSingleton {
private:
    static EagerSingleton instance;
    
    EagerSingleton() {
        std::cout << "EagerSingleton instance created at startup" << std::endl;
    }
    
    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;

public:
    static EagerSingleton& getInstance() {
        return instance;
    }
    
    void doSomething() {
        std::cout << "EagerSingleton is doing something..." << std::endl;
    }
};

EagerSingleton EagerSingleton::instance;

// 5. 模板化单例基类
template<typename T>
class SingletonBase {
protected:
    SingletonBase() = default;
    virtual ~SingletonBase() = default;
    
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

public:
    static T& getInstance() {
        static T instance;
        return instance;
    }
};

// 使用模板化单例的具体类
class DatabaseManager : public SingletonBase<DatabaseManager> {
private:
    friend class SingletonBase<DatabaseManager>;
    
    DatabaseManager() {
        std::cout << "DatabaseManager initialized" << std::endl;
    }

public:
    void connect() {
        std::cout << "Database connected" << std::endl;
    }
    
    void query(const std::string& sql) {
        std::cout << "Executing query: " << sql << std::endl;
    }
};

class Logger : public SingletonBase<Logger> {
private:
    friend class SingletonBase<Logger>;
    
    Logger() {
        std::cout << "Logger initialized" << std::endl;
    }

public:
    void log(const std::string& message) {
        std::cout << "[LOG] " << message << std::endl;
    }
};

// 测试函数
void testMeyersSingleton(int threadId) {
    auto& singleton = MeyersSingleton::getInstance();
    std::cout << "Thread " << threadId << " got singleton at address: " << &singleton << std::endl;
    singleton.doSomething();
}

void testThreadSafeSingleton(int threadId) {
    auto* singleton = ThreadSafeSingleton::getInstance();
    std::cout << "Thread " << threadId << " got thread-safe singleton at address: " << singleton << std::endl;
    singleton->doSomething();
}

int main() {
    std::cout << "==================== C++ 单例模式演示 ====================" << std::endl;
    
    std::cout << "\n1. 饿汉式单例（程序启动时已创建）：" << std::endl;
    auto& eager1 = EagerSingleton::getInstance();
    auto& eager2 = EagerSingleton::getInstance();
    std::cout << "eager1 地址: " << &eager1 << std::endl;
    std::cout << "eager2 地址: " << &eager2 << std::endl;
    std::cout << "是否为同一实例: " << (&eager1 == &eager2 ? "是" : "否") << std::endl;
    
    std::cout << "\n2. Meyers单例测试：" << std::endl;
    auto& meyers1 = MeyersSingleton::getInstance();
    auto& meyers2 = MeyersSingleton::getInstance();
    std::cout << "meyers1 地址: " << &meyers1 << std::endl;
    std::cout << "meyers2 地址: " << &meyers2 << std::endl;
    std::cout << "是否为同一实例: " << (&meyers1 == &meyers2 ? "是" : "否") << std::endl;
    
    std::cout << "\n3. 多线程测试 - Meyers单例：" << std::endl;
    std::vector<std::thread> threads1;
    for (int i = 0; i < 5; ++i) {
        threads1.emplace_back(testMeyersSingleton, i);
    }
    for (auto& t : threads1) {
        t.join();
    }
    
    std::cout << "\n4. 多线程测试 - 线程安全单例：" << std::endl;
    std::vector<std::thread> threads2;
    for (int i = 0; i < 5; ++i) {
        threads2.emplace_back(testThreadSafeSingleton, i);
    }
    for (auto& t : threads2) {
        t.join();
    }
    
    std::cout << "\n5. 模板化单例测试：" << std::endl;
    auto& dbManager1 = DatabaseManager::getInstance();
    auto& dbManager2 = DatabaseManager::getInstance();
    std::cout << "DatabaseManager实例是否相同: " << (&dbManager1 == &dbManager2 ? "是" : "否") << std::endl;
    dbManager1.connect();
    dbManager1.query("SELECT * FROM users");
    
    auto& logger1 = Logger::getInstance();
    auto& logger2 = Logger::getInstance();
    std::cout << "Logger实例是否相同: " << (&logger1 == &logger2 ? "是" : "否") << std::endl;
    logger1.log("Application started");
    logger2.log("This is the same logger instance");
    
    std::cout << "\n6. 经典单例测试（注意：线程不安全）：" << std::endl;
    auto* classic1 = Singleton::getInstance();
    auto* classic2 = Singleton::getInstance();
    std::cout << "classic1 地址: " << classic1 << std::endl;
    std::cout << "classic2 地址: " << classic2 << std::endl;
    std::cout << "是否为同一实例: " << (classic1 == classic2 ? "是" : "否") << std::endl;
    
    // 清理资源
    std::cout << "\n7. 清理资源：" << std::endl;
    Singleton::destroyInstance();
    ThreadSafeSingleton::destroyInstance();
    std::cout << "手动管理的单例实例已销毁" << std::endl;
    
    std::cout << "\n==================== 演示结束 ====================" << std::endl;
    
    return 0;
}