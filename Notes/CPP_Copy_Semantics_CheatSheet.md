# C++ Copy Semantics and Shallow Copy Problem Cheat Sheet

# C++ 拷贝语义和浅拷贝问题速查表

## 目录 (Table of Contents)

1. [基本概念 (Basic Concepts)](#基本概念-basic-concepts)
2. [浅拷贝 vs 深拷贝 (Shallow Copy vs Deep Copy)](#浅拷贝-vs-深拷贝-shallow-copy-vs-deep-copy)
3. [Rule of Three/Five/Zero](#rule-of-threefivezero)
4. [实际问题示例 (Problem Examples)](#实际问题示例-problem-examples)
5. [解决方案 (Solutions)](#解决方案-solutions)
6. [最佳实践 (Best Practices)](#最佳实践-best-practices)
7. [常见陷阱 (Common Pitfalls)](#常见陷阱-common-pitfalls)

---

## 基本概念 (Basic Concepts)

### 默认拷贝行为 (Default Copy Behavior)

C++编译器会自动为每个类生成以下函数（如果你没有显式定义）：

```cpp
class MyClass {
public:
    MyClass();                              // 默认构造函数
    MyClass(const MyClass& other);          // 拷贝构造函数
    MyClass& operator=(const MyClass& rhs); // 拷贝赋值运算符
    ~MyClass();                             // 析构函数

    // C++11之后还有：
    MyClass(MyClass&& other);               // 移动构造函数
    MyClass& operator=(MyClass&& rhs);      // 移动赋值运算符
};
```

### 成员拷贝 (Memberwise Copy)

默认的拷贝行为是**逐成员拷贝**：

- 基本类型：值拷贝
- 指针：地址拷贝（危险！）
- 对象：递归调用拷贝构造函数

---

## 浅拷贝 vs 深拷贝 (Shallow Copy vs Deep Copy)

### 浅拷贝 (Shallow Copy) ❌

```cpp
class BadString {
private:
    char* data;
    size_t size;

public:
    BadString(const char* str) {
        size = strlen(str);
        data = new char[size + 1];
        strcpy(data, str);
    }

    ~BadString() {
        delete[] data;  // 可能导致double delete!
    }

    // 没有定义拷贝构造函数和拷贝赋值运算符
    // 编译器生成的默认版本只拷贝指针值！
};

// 使用示例 - 会崩溃！
BadString s1("Hello");
BadString s2 = s1;  // 浅拷贝！s1.data == s2.data
// 析构时，同一块内存被delete两次！
```

### 深拷贝 (Deep Copy) ✅

```cpp
class GoodString {
private:
    char* data;
    size_t size;

public:
    GoodString(const char* str) {
        size = strlen(str);
        data = new char[size + 1];
        strcpy(data, str);
    }

    // 拷贝构造函数 - 深拷贝
    GoodString(const GoodString& other) {
        size = other.size;
        data = new char[size + 1];
        strcpy(data, other.data);
    }

    // 拷贝赋值运算符 - 深拷贝
    GoodString& operator=(const GoodString& rhs) {
        if (this != &rhs) {  // 自赋值检查
            delete[] data;   // 释放旧资源

            size = rhs.size;
            data = new char[size + 1];
            strcpy(data, rhs.data);
        }
        return *this;
    }

    ~GoodString() {
        delete[] data;
    }
};
```

---

## Rule of Three/Five/Zero

### Rule of Three (三法则) - C++98/03

如果一个类需要自定义以下任何一个，那么通常需要自定义所有三个：

1. **析构函数** (`~ClassName()`)
2. **拷贝构造函数** (`ClassName(const ClassName&)`)
3. **拷贝赋值运算符** (`ClassName& operator=(const ClassName&)`)

### Rule of Five (五法则) - C++11+

在 Rule of Three 基础上增加： 4. **移动构造函数** (`ClassName(ClassName&&)`) 5. **移动赋值运算符** (`ClassName& operator=(ClassName&&)`)

### Rule of Zero (零法则) - 现代 C++

尽量避免手动资源管理，使用 RAII 和智能指针：

```cpp
class ModernString {
private:
    std::unique_ptr<char[]> data;
    size_t size;

public:
    ModernString(const char* str)
        : size(strlen(str)), data(std::make_unique<char[]>(size + 1)) {
        strcpy(data.get(), str);
    }

    // 不需要手动定义拷贝/移动/析构函数！
    // 或者显式定义拷贝语义：
    ModernString(const ModernString& other)
        : size(other.size), data(std::make_unique<char[]>(size + 1)) {
        strcpy(data.get(), other.data.get());
    }
};
```

---

## 实际问题示例 (Problem Examples)

### 1. 容器类的浅拷贝问题

```cpp
template<typename T>
class BadVector {
private:
    T* data;
    size_t capacity;
    size_t size;

public:
    BadVector() : data(new T[10]), capacity(10), size(0) {}

    ~BadVector() { delete[] data; }

    // 问题：默认拷贝只拷贝指针！
};

// 使用会出问题：
BadVector<int> v1;
BadVector<int> v2 = v1;  // 浅拷贝
// v1和v2共享同一块内存！
```

### 2. 链表节点的浅拷贝问题

```cpp
struct Node {
    int data;
    Node* next;

    Node(int val) : data(val), next(nullptr) {}
    ~Node() { delete next; }  // 递归删除

    // 默认拷贝构造函数只拷贝指针值
    // 可能导致环形引用或double delete
};
```

### 3. 文件句柄/资源管理

```cpp
class BadFileHandler {
private:
    FILE* file;

public:
    BadFileHandler(const char* filename) {
        file = fopen(filename, "r");
    }

    ~BadFileHandler() {
        if (file) fclose(file);
    }

    // 默认拷贝会复制文件指针
    // 多个对象关闭同一个文件句柄！
};
```

---

## 解决方案 (Solutions)

### 1. 完整的拷贝语义实现

```cpp
template<typename T>
class SafeVector {
private:
    T* data;
    size_t capacity;
    size_t size;

public:
    // 构造函数
    SafeVector(size_t cap = 10)
        : data(new T[cap]), capacity(cap), size(0) {}

    // 拷贝构造函数
    SafeVector(const SafeVector& other)
        : data(new T[other.capacity]),
          capacity(other.capacity),
          size(other.size) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 拷贝赋值运算符
    SafeVector& operator=(const SafeVector& rhs) {
        if (this != &rhs) {
            // 异常安全的实现
            T* new_data = new T[rhs.capacity];
            for (size_t i = 0; i < rhs.size; ++i) {
                new_data[i] = rhs.data[i];
            }

            delete[] data;
            data = new_data;
            capacity = rhs.capacity;
            size = rhs.size;
        }
        return *this;
    }

    // 移动构造函数
    SafeVector(SafeVector&& other) noexcept
        : data(other.data), capacity(other.capacity), size(other.size) {
        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
    }

    // 移动赋值运算符
    SafeVector& operator=(SafeVector&& rhs) noexcept {
        if (this != &rhs) {
            delete[] data;

            data = rhs.data;
            capacity = rhs.capacity;
            size = rhs.size;

            rhs.data = nullptr;
            rhs.capacity = 0;
            rhs.size = 0;
        }
        return *this;
    }

    // 析构函数
    ~SafeVector() {
        delete[] data;
    }
};
```

### 2. 使用智能指针避免手动管理

```cpp
class SmartFileHandler {
private:
    std::unique_ptr<FILE, decltype(&fclose)> file;

public:
    SmartFileHandler(const char* filename)
        : file(fopen(filename, "r"), &fclose) {
        if (!file) {
            throw std::runtime_error("Failed to open file");
        }
    }

    // 自动获得正确的移动语义
    // 禁止拷贝语义（因为文件句柄不应该共享）
    SmartFileHandler(const SmartFileHandler&) = delete;
    SmartFileHandler& operator=(const SmartFileHandler&) = delete;
};
```

### 3. 禁止拷贝的资源类

```cpp
class NonCopyableResource {
private:
    int* resource;

public:
    NonCopyableResource() : resource(new int(42)) {}

    // 显式禁止拷贝
    NonCopyableResource(const NonCopyableResource&) = delete;
    NonCopyableResource& operator=(const NonCopyableResource&) = delete;

    // 允许移动
    NonCopyableResource(NonCopyableResource&& other) noexcept
        : resource(other.resource) {
        other.resource = nullptr;
    }

    NonCopyableResource& operator=(NonCopyableResource&& rhs) noexcept {
        if (this != &rhs) {
            delete resource;
            resource = rhs.resource;
            rhs.resource = nullptr;
        }
        return *this;
    }

    ~NonCopyableResource() {
        delete resource;
    }
};
```

---

## 最佳实践 (Best Practices)

### 1. 优先使用标准库容器

```cpp
// ❌ 自己写动态数组
class MyVector { /* ... */ };

// ✅ 使用标准库
std::vector<int> data;
```

### 2. 使用智能指针

```cpp
// ❌ 原始指针
class BadClass {
    int* ptr;
public:
    BadClass() : ptr(new int(42)) {}
    ~BadClass() { delete ptr; }
    // 需要手动实现拷贝语义
};

// ✅ 智能指针
class GoodClass {
    std::unique_ptr<int> ptr;
public:
    GoodClass() : ptr(std::make_unique<int>(42)) {}
    // 自动获得正确的移动语义
    // 如需拷贝，显式实现
};
```

### 3. 遵循 RAII 原则

```cpp
class RAIIFile {
    std::ifstream file;
public:
    RAIIFile(const std::string& filename) : file(filename) {
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
    }
    // 析构函数自动关闭文件
    // 拷贝语义由std::ifstream定义
};
```

### 4. 明确拷贝语义

```cpp
class ExplicitCopyClass {
public:
    // 方案1：禁止拷贝，只允许移动
    ExplicitCopyClass(const ExplicitCopyClass&) = delete;
    ExplicitCopyClass& operator=(const ExplicitCopyClass&) = delete;

    // 方案2：显式实现深拷贝
    ExplicitCopyClass(const ExplicitCopyClass& other) { /* 深拷贝实现 */ }
    ExplicitCopyClass& operator=(const ExplicitCopyClass& rhs) { /* 深拷贝实现 */ }

    // 方案3：使用编译器生成的默认实现（仅当安全时）
    ExplicitCopyClass(const ExplicitCopyClass&) = default;
    ExplicitCopyClass& operator=(const ExplicitCopyClass&) = default;
};
```

---

## 常见陷阱 (Common Pitfalls)

### 1. 忘记自赋值检查

```cpp
MyClass& operator=(const MyClass& rhs) {
    // ❌ 没有自赋值检查
    delete[] data;
    data = new int[rhs.size];
    // 如果 this == &rhs，这里会使用已删除的内存！

    // ✅ 正确做法
    if (this != &rhs) {
        delete[] data;
        data = new int[rhs.size];
        // ...
    }
    return *this;
}
```

### 2. 异常安全问题

```cpp
MyClass& operator=(const MyClass& rhs) {
    if (this != &rhs) {
        delete[] data;  // ❌ 如果后面抛异常，对象状态不一致
        data = new int[rhs.size];  // 可能抛异常

        // ✅ 异常安全的做法
        int* new_data = new int[rhs.size];  // 先分配
        delete[] data;  // 再删除旧的
        data = new_data;
    }
    return *this;
}
```

### 3. 移动后使用

```cpp
std::vector<int> v1 {1, 2, 3};
std::vector<int> v2 = std::move(v1);
// ❌ v1现在处于"有效但未指定"状态
std::cout << v1.size();  // 可能是0，但不保证
```

### 4. 返回引用的陷阱

```cpp
MyClass& operator=(const MyClass& rhs) {
    // ...
    // ❌ 忘记返回*this
    return *this;  // ✅ 必须返回*this
}
```

---

## 检查清单 (Checklist)

当你的类管理资源时，检查以下项目：

- [ ] 是否需要自定义析构函数？
- [ ] 如果需要析构函数，是否实现了拷贝构造函数？
- [ ] 如果需要析构函数，是否实现了拷贝赋值运算符？
- [ ] 拷贝赋值运算符是否检查自赋值？
- [ ] 拷贝赋值运算符是否异常安全？
- [ ] 是否考虑了移动语义？
- [ ] 是否考虑禁止拷贝而只允许移动？
- [ ] 是否使用了智能指针来避免手动内存管理？

---

## 总结 (Summary)

浅拷贝问题是 C++中最常见的陷阱之一。关键要点：

1. **理解默认行为**：编译器生成的拷贝是逐成员的浅拷贝
2. **识别风险**：任何管理动态资源的类都可能有浅拷贝问题
3. **遵循规则**：Rule of Three/Five/Zero
4. **使用现代 C++**：智能指针、标准库容器、RAII
5. **明确意图**：显式禁止或实现拷贝语义

记住：**当有疑问时，测试你的拷贝语义！**

```cpp
// 简单测试模板
MyClass a;
MyClass b = a;      // 拷贝构造
MyClass c;
c = a;              // 拷贝赋值
MyClass d = std::move(a);  // 移动构造
c = std::move(b);   // 移动赋值
// 所有对象析构时不应崩溃
```
