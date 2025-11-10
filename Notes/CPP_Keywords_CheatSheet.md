# C++ 关键字速查表 (Keywords CheatSheet)

## 目录

- [类型转换相关](#类型转换相关)
- [类与对象相关](#类与对象相关)
- [常量与修饰符](#常量与修饰符)
- [存储类说明符](#存储类说明符)
- [函数相关](#函数相关)
- [模板相关](#模板相关)
- [异常处理](#异常处理)
- [内存管理](#内存管理)
- [其他重要关键字](#其他重要关键字)

---

## 类型转换相关

### `static_cast`

**用途**: 编译时类型转换，最常用的显式转换

```cpp
// 基本类型转换
int i = 10;
double d = static_cast<double>(i);  // int -> double

// 指针类型转换（上下行转换）
Base* basePtr = new Derived();
Derived* derivedPtr = static_cast<Derived*>(basePtr);  // 向下转型（不安全）

// void* 转换
void* voidPtr = malloc(sizeof(int));
int* intPtr = static_cast<int*>(voidPtr);
```

### `dynamic_cast`

**用途**: 运行时类型检查的安全向下转型（仅用于多态类型）

```cpp
class Base { virtual void foo() {} };
class Derived : public Base {};

Base* basePtr = new Derived();
Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);  // 成功，返回有效指针
if (derivedPtr) {
    // 转换成功
}

Base* basePtr2 = new Base();
Derived* derivedPtr2 = dynamic_cast<Derived*>(basePtr2);  // 失败，返回 nullptr
```

### `const_cast`

**用途**: 添加或移除 const/volatile 限定符

```cpp
const int x = 10;
int* ptr = const_cast<int*>(&x);  // 移除 const（危险操作！）
*ptr = 20;  // 未定义行为

// 常见合法用途：调用非 const 的第三方库函数
void legacyFunc(char* str);  // 老接口，不接受 const
const char* myStr = "Hello";
legacyFunc(const_cast<char*>(myStr));  // 只要函数不修改，就安全
```

### `reinterpret_cast`

**用途**: 底层位模式的重新解释（最危险的转换）

```cpp
int i = 65;
char* charPtr = reinterpret_cast<char*>(&i);  // 把 int* 当作 char*

// 指针与整数互转
uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
void* ptr2 = reinterpret_cast<void*>(addr);
```

---

## 类与对象相关

### `explicit`

**用途**: 禁止隐式类型转换，防止意外的构造函数调用

```cpp
class MyClass {
public:
    // ❌ 没有 explicit：允许隐式转换
    MyClass(int x) { }
};
MyClass obj1 = 5;  // OK，隐式调用 MyClass(5)
void func(MyClass obj) { }
func(10);  // OK，隐式调用 MyClass(10)

class MyClass2 {
public:
    // ✅ 有 explicit：禁止隐式转换
    explicit MyClass2(int x) { }
};
MyClass2 obj2 = 5;        // ❌ 编译错误
MyClass2 obj3(5);         // ✅ OK，显式调用
MyClass2 obj4{5};         // ✅ OK，显式调用
func(MyClass2(10));       // ✅ OK，显式构造
```

**最佳实践**: 单参数构造函数应该加 `explicit`，除非你确实需要隐式转换。

### `virtual`

**用途**: 声明虚函数，实现运行时多态

```cpp
class Base {
public:
    virtual void foo() { cout << "Base::foo" << endl; }
    virtual void bar() = 0;  // 纯虚函数（抽象函数）
    virtual ~Base() { }      // 虚析构函数（重要！）
};

class Derived : public Base {
public:
    void foo() override { cout << "Derived::foo" << endl; }  // 重写
    void bar() override { cout << "Derived::bar" << endl; }
};

Base* ptr = new Derived();
ptr->foo();  // 输出 "Derived::foo"（动态绑定）
delete ptr;  // 调用 Derived 的析构函数（因为基类析构是虚函数）
```

### `override` (C++11)

**用途**: 明确标记重写基类虚函数，编译器会检查

```cpp
class Base {
public:
    virtual void foo() { }
};

class Derived : public Base {
public:
    void foo() override { }     // ✅ 正确重写
    void fooo() override { }    // ❌ 编译错误：基类没有 fooo()
    void foo() const override { }  // ❌ 编译错误：签名不匹配
};
```

### `final` (C++11)

**用途**: 禁止继承类或重写虚函数

```cpp
// 1. 禁止继承
class Base final {
    // ...
};
class Derived : public Base { };  // ❌ 编译错误

// 2. 禁止重写虚函数
class Base {
public:
    virtual void foo() final { }
};
class Derived : public Base {
public:
    void foo() override { }  // ❌ 编译错误
};
```

### `friend`

**用途**: 授予非成员函数或其他类访问私有成员的权限

```cpp
class MyClass {
private:
    int secret = 42;

    friend void friendFunc(MyClass& obj);  // 友元函数
    friend class FriendClass;              // 友元类
};

void friendFunc(MyClass& obj) {
    cout << obj.secret << endl;  // ✅ 可以访问私有成员
}

class FriendClass {
public:
    void accessSecret(MyClass& obj) {
        cout << obj.secret << endl;  // ✅ 可以访问
    }
};
```

---

## 常量与修饰符

### `const`

**用途**: 声明常量，防止修改

#### 1. const 变量

```cpp
const int x = 10;
x = 20;  // ❌ 编译错误

const int* ptr1;       // 指向常量的指针（不能通过 ptr1 修改值）
int* const ptr2;       // 常量指针（ptr2 本身不能改，但可以修改指向的值）
const int* const ptr3; // 指向常量的常量指针（都不能改）
```

#### 2. const 成员函数

```cpp
class MyClass {
private:
    int value;
public:
    int getValue() const { return value; }  // const 成员函数：不修改对象
    void setValue(int v) { value = v; }     // 非 const 成员函数
};

const MyClass obj;
obj.getValue();  // ✅ OK
obj.setValue(5); // ❌ 编译错误：const 对象不能调用非 const 成员函数
```

#### 3. const 参数

```cpp
void func(const int x) { }           // 按值传递的 const（没啥用）
void func(const int& x) { }          // const 引用（高效，防止修改）
void func(const int* x) { }          // 指向 const 的指针
void func(int* const x) { }          // const 指针
```

### `constexpr` (C++11)

**用途**: 编译时常量表达式

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int result = factorial(5);  // 编译时计算
int arr[factorial(4)];  // ✅ 可以用于数组大小

// const vs constexpr
const int x = 10;        // 运行时或编译时常量
constexpr int y = 10;    // 必须是编译时常量

int input;
cin >> input;
const int a = input;     // ✅ OK，运行时常量
constexpr int b = input; // ❌ 编译错误，不是编译时常量
```

### `mutable`

**用途**: 允许 const 成员函数修改成员变量

```cpp
class Counter {
private:
    mutable int accessCount = 0;  // mutable 成员
    int value;
public:
    int getValue() const {
        accessCount++;  // ✅ const 函数可以修改 mutable 成员
        return value;
    }
};
```

### `volatile`

**用途**: 告诉编译器变量可能被外部修改，禁止优化

```cpp
volatile int hardwareRegister;  // 硬件寄存器
hardwareRegister = 1;
hardwareRegister = 2;  // 编译器不会优化掉第一次赋值

// 常见场景：
// 1. 硬件寄存器映射
// 2. 多线程共享变量（但现代 C++ 应该用 std::atomic）
// 3. 信号处理函数
```

---

## 存储类说明符

### `static`

**用途**: 改变变量/函数的作用域和生命周期

#### 1. 静态局部变量

```cpp
void func() {
    static int count = 0;  // 只初始化一次
    count++;
    cout << count << endl;
}
func();  // 输出 1
func();  // 输出 2
func();  // 输出 3
```

#### 2. 静态成员变量

```cpp
class MyClass {
public:
    static int sharedValue;  // 所有对象共享
};
int MyClass::sharedValue = 0;  // 类外定义

MyClass obj1, obj2;
obj1.sharedValue = 10;
cout << obj2.sharedValue << endl;  // 输出 10
```

#### 3. 静态成员函数

```cpp
class MyClass {
public:
    static void staticFunc() {
        // 不能访问非静态成员
    }
};
MyClass::staticFunc();  // 通过类名调用
```

#### 4. 文件作用域的 static（C 风格，C++ 建议用匿名命名空间）

```cpp
static int fileVar = 0;  // 仅在当前文件可见

// C++ 风格
namespace {
    int fileVar = 0;  // 仅在当前文件可见
}
```

### `extern`

**用途**: 声明外部变量或函数

```cpp
// file1.cpp
int globalVar = 10;

// file2.cpp
extern int globalVar;  // 声明，不定义
cout << globalVar << endl;  // 使用 file1.cpp 中的变量

// extern "C"：按 C 语言方式链接（禁止 name mangling）
extern "C" {
    void cFunction();
}
```

### `thread_local` (C++11)

**用途**: 线程局部存储，每个线程有独立副本

```cpp
thread_local int threadVar = 0;

void threadFunc() {
    threadVar++;  // 每个线程有自己的 threadVar
}
```

---

## 函数相关

### `inline`

**用途**: 建议编译器内联展开函数

```cpp
inline int add(int a, int b) {
    return a + b;
}
// 调用 add(1, 2) 可能被展开为 1 + 2

// 现代 C++：编译器会自动决定是否内联，inline 主要用于允许多次定义
// 头文件中的函数定义应该加 inline
```

### `noexcept` (C++11)

**用途**: 声明函数不抛出异常

```cpp
void func() noexcept {
    // 如果抛出异常，程序会调用 std::terminate()
}

void func2() noexcept(true) { }   // 不抛异常
void func3() noexcept(false) { }  // 可能抛异常

// 条件 noexcept
template<typename T>
void swap(T& a, T& b) noexcept(noexcept(T(std::move(a)))) {
    // 如果 T 的移动构造不抛异常，则 swap 也不抛
}
```

### `delete` (C++11)

**用途**: 禁用函数

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;             // 禁用拷贝构造
    NonCopyable& operator=(const NonCopyable&) = delete;  // 禁用拷贝赋值
};

void func(int) { }
void func(double) = delete;  // 禁止 double 版本
func(1);    // ✅ OK
func(1.0);  // ❌ 编译错误
```

### `default` (C++11)

**用途**: 显式要求编译器生成默认实现

```cpp
class MyClass {
public:
    MyClass() = default;                    // 默认构造
    MyClass(const MyClass&) = default;      // 默认拷贝构造
    MyClass& operator=(const MyClass&) = default;  // 默认拷贝赋值
    ~MyClass() = default;                   // 默认析构
};
```

---

## 模板相关

### `template`

**用途**: 定义模板

```cpp
// 函数模板
template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 类模板
template<typename T, int Size>
class Array {
    T data[Size];
};

// 模板特化
template<>
class Array<bool, 8> {
    // bool 的特化实现
};
```

### `typename` vs `class`

**用途**: 模板参数声明（大多数情况下可互换）

```cpp
template<typename T>  // 推荐使用 typename
void func1() { }

template<class T>     // 也可以用 class
void func2() { }

// typename 的另一个用途：指示依赖类型名
template<typename T>
void func() {
    typename T::value_type x;  // 告诉编译器 value_type 是类型
}
```

### `decltype` (C++11)

**用途**: 推导表达式的类型

```cpp
int x = 10;
decltype(x) y = 20;  // y 的类型是 int

auto func() -> decltype(x + y) {  // 返回类型推导
    return x + y;
}

// 常用于模板
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}
```

### `auto` (C++11)

**用途**: 自动类型推导

```cpp
auto x = 10;        // int
auto y = 3.14;      // double
auto z = "hello";   // const char*

// 迭代器
auto it = vec.begin();  // 替代 vector<int>::iterator it

// lambda
auto lambda = [](int x) { return x * 2; };

// 注意
auto& ref = x;        // 引用
const auto& cref = x; // const 引用
auto* ptr = &x;       // 指针
```

---

## 异常处理

### `try` / `catch` / `throw`

**用途**: 异常处理机制

```cpp
try {
    throw runtime_error("错误信息");
} catch (const runtime_error& e) {
    cout << "捕获异常: " << e.what() << endl;
} catch (const exception& e) {
    cout << "捕获基类异常: " << e.what() << endl;
} catch (...) {
    cout << "捕获所有异常" << endl;
}
```

### `throw` (函数声明)

```cpp
// C++98 风格（已弃用）
void func() throw(int, runtime_error);  // 可能抛出 int 或 runtime_error

// C++11+ 风格
void func() noexcept;  // 不抛异常
void func();           // 可能抛任何异常
```

---

## 内存管理

### `new` / `delete`

**用途**: 动态内存分配

```cpp
int* ptr = new int(10);        // 分配单个对象
delete ptr;                    // 释放

int* arr = new int[100];       // 分配数组
delete[] arr;                  // 释放数组

// placement new
char buffer[sizeof(MyClass)];
MyClass* obj = new (buffer) MyClass();  // 在指定内存构造对象
obj->~MyClass();  // 手动调用析构函数
```

### `nullptr` (C++11)

**用途**: 空指针字面量，替代 NULL

```cpp
int* ptr1 = NULL;      // C 风格（实际是 0）
int* ptr2 = nullptr;   // C++11 风格

void func(int) { cout << "int" << endl; }
void func(int*) { cout << "int*" << endl; }

func(NULL);     // ❌ 可能调用 func(int)，歧义！
func(nullptr);  // ✅ 调用 func(int*)
```

---

## 其他重要关键字

### `sizeof`

**用途**: 获取类型或对象的字节大小

```cpp
sizeof(int);           // 4（通常）
sizeof(char);          // 1
sizeof(MyClass);       // 类的大小

int arr[10];
sizeof(arr);           // 40（10 * sizeof(int)）
sizeof(arr) / sizeof(arr[0]);  // 数组元素个数
```

### `alignof` / `alignas` (C++11)

**用途**: 查询/指定内存对齐

```cpp
alignof(int);          // 4（通常）
alignof(double);       // 8（通常）

struct alignas(16) AlignedStruct {
    int x;
};
```

### `using` (C++11)

**用途**: 类型别名、命名空间、继承构造函数

```cpp
// 1. 类型别名（替代 typedef）
using IntVector = vector<int>;
typedef vector<int> IntVector;  // 旧风格

// 2. 命名空间
using namespace std;
using std::cout;

// 3. 继承构造函数
class Derived : public Base {
public:
    using Base::Base;  // 继承 Base 的所有构造函数
};
```

### `namespace`

**用途**: 命名空间

```cpp
namespace MyNamespace {
    void func() { }
}

MyNamespace::func();

// 嵌套命名空间 (C++17)
namespace A::B::C {
    void func() { }
}
```

### `consteval` (C++20)

**用途**: 强制编译时求值

```cpp
consteval int square(int x) {
    return x * x;
}

int result = square(5);  // 必须在编译时计算
int y;
cin >> y;
int result2 = square(y); // ❌ 编译错误：y 不是编译时常量
```

### `constinit` (C++20)

**用途**: 确保静态变量编译时初始化

```cpp
constinit int x = 10;  // 必须编译时初始化
x = 20;  // ✅ 运行时可以修改（与 const 不同）
```

### `concept` (C++20)

**用途**: 定义模板约束

```cpp
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}
```

---

## 快速对比表

| 关键字      | 作用             | 示例                     |
| ----------- | ---------------- | ------------------------ |
| `const`     | 常量，防止修改   | `const int x = 10;`      |
| `constexpr` | 编译时常量       | `constexpr int y = 20;`  |
| `mutable`   | const 函数可修改 | `mutable int count;`     |
| `static`    | 静态存储         | `static int x;`          |
| `extern`    | 外部链接         | `extern int globalVar;`  |
| `inline`    | 内联建议         | `inline int add() {}`    |
| `explicit`  | 禁止隐式转换     | `explicit MyClass(int);` |
| `virtual`   | 虚函数           | `virtual void foo();`    |
| `override`  | 明确重写         | `void foo() override;`   |
| `final`     | 禁止重写/继承    | `class A final {};`      |
| `noexcept`  | 不抛异常         | `void func() noexcept;`  |
| `auto`      | 类型推导         | `auto x = 10;`           |
| `decltype`  | 类型推导         | `decltype(x) y;`         |
| `nullptr`   | 空指针           | `int* ptr = nullptr;`    |

---

## 最佳实践建议

1. ✅ **单参数构造函数加 `explicit`**，防止意外转换
2. ✅ **优先使用 `const`**，让编译器帮你检查错误
3. ✅ **虚析构函数**：基类有虚函数时必须有虚析构
4. ✅ **用 `override`** 标记重写函数，避免拼写错误
5. ✅ **用 `nullptr`** 代替 NULL
6. ✅ **用 `auto`** 简化复杂类型
7. ✅ **用 `using`** 代替 `typedef`
8. ✅ **用 `delete`** 禁用函数，而不是声明为 private
9. ✅ **用 `noexcept`** 标记不抛异常的函数（特别是移动操作）
10. ✅ **用智能指针** 代替 `new/delete`

---

## 延伸阅读

- [C++ Reference](https://en.cppreference.com/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Effective Modern C++ (Scott Meyers)](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)
