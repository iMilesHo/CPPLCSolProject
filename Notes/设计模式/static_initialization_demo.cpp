#include <iostream>

// 1. 静态局部变量 - 延迟初始化（Meyers单例）
class LazyStatic {
public:
    static LazyStatic& getInstance() {
        std::cout << "  [INFO] 进入 getInstance() 函数" << std::endl;
        static LazyStatic instance;  // 只在第一次执行到这里时创建
        std::cout << "  [INFO] 返回 instance 引用" << std::endl;
        return instance;
    }
    
private:
    LazyStatic() { 
        std::cout << "  [CONSTRUCTOR] LazyStatic 构造函数被调用" << std::endl; 
    }
};

// 2. 静态成员变量 - 程序启动时初始化（饿汉式）
class EagerStatic {
private:
    static EagerStatic instance;  // 程序启动时初始化
    
public:
    static EagerStatic& getInstance() { 
        std::cout << "  [INFO] 返回已存在的 EagerStatic 实例" << std::endl;
        return instance; 
    }
    
    EagerStatic() { 
        std::cout << "  [CONSTRUCTOR] EagerStatic 构造函数被调用" << std::endl; 
    }
};

// 静态成员变量的定义 - 这会在main()之前执行
EagerStatic EagerStatic::instance;

// 3. 全局静态变量 - 程序启动时初始化
class GlobalStatic {
public:
    GlobalStatic() { 
        std::cout << "  [CONSTRUCTOR] GlobalStatic 构造函数被调用" << std::endl; 
    }
};

static GlobalStatic globalObj;  // 程序启动时创建

int main() {
    std::cout << "\n=== 程序开始执行 main() ===" << std::endl;
    
    std::cout << "\n1. 第一次调用 LazyStatic::getInstance():" << std::endl;
    LazyStatic::getInstance();
    
    std::cout << "\n2. 第二次调用 LazyStatic::getInstance():" << std::endl;
    LazyStatic::getInstance();
    
    std::cout << "\n3. 调用 EagerStatic::getInstance():" << std::endl;
    EagerStatic::getInstance();
    
    std::cout << "\n=== 程序结束 ===" << std::endl;
    return 0;
}

// 在main之前执行的代码演示
class InitOrderDemo {
public:
    InitOrderDemo() {
        std::cout << "[GLOBAL] 全局对象在 main() 之前初始化" << std::endl;
    }
};

// 这个对象会在main()之前创建
static InitOrderDemo initDemo;