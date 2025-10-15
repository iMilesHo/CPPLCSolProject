# C++ 数据类型和对象概念 Cheat Sheet

## 1. 字符串类型详解

### C 风格字符串 vs C++字符串 vs 字符数组

```cpp
// 1. C++标准字符串 (对象)
string str1 = "Hello";           // std::string对象，有丰富的成员函数
str1.length();                   // 可以调用成员函数
str1.append(" World");           // 动态修改

// 2. C风格字符串 (指针)
const char* str2 = "World";      // 指向字符串字面量的指针
// str2[0] = 'w';                // 错误！字符串字面量不可修改
strlen(str2);                    // 使用C库函数

// 3. 字符数组 (数组)
char arr[] = "Array";            // 字符数组，可以修改
arr[0] = 'a';                    // 可以修改
sizeof(arr);                     // 返回数组大小（包含'\0'）
```

### 关键区别

| 类型          | 本质 | 可修改性 | 内存管理  | 函数调用 |
| ------------- | ---- | -------- | --------- | -------- |
| `string`      | 对象 | 可修改   | 自动管理  | 成员函数 |
| `const char*` | 指针 | 不可修改 | 静态/手动 | C 库函数 |
| `char[]`      | 数组 | 可修改   | 栈上分配  | C 库函数 |

## 2. 基本数据类型 vs 对象

### 基本数据类型 (Built-in Types / Primitive Types)

```cpp
// 整数类型
int a = 42;
short b = 100;
long c = 1000L;
long long d = 10000LL;
unsigned int e = 42U;

// 浮点类型
float f = 3.14f;
double g = 3.14159;
long double h = 3.14159L;

// 字符类型
char ch = 'A';
wchar_t wch = L'中';
char16_t ch16 = u'A';
char32_t ch32 = U'A';

// 布尔类型
bool flag = true;

// 指针类型
int* ptr = &a;
void* void_ptr = nullptr;
```

**特点：**

- 不是对象，是语言内置的基本类型
- 没有成员函数
- 直接存储值
- 性能最优

### 对象类型 (Class Types)

```cpp
// 标准库对象
string str = "Hello";            // std::string对象
vector<int> vec = {1, 2, 3};     // std::vector对象
map<string, int> m;              // std::map对象

// 自定义类对象
class MyClass {
public:
    void func() {}
};
MyClass obj;                     // 用户定义的对象
```

**特点：**

- 是类的实例
- 有成员函数和成员变量
- 可能有构造/析构函数
- 支持封装、继承、多态

## 3. 容器 (Containers) 详解

### 所有 STL 容器都是对象

```cpp
// 序列容器 (Sequence Containers)
vector<int> vec;                 // 动态数组对象
deque<int> dq;                   // 双端队列对象
list<int> lst;                   // 链表对象
array<int, 5> arr;               // 固定大小数组对象

// 关联容器 (Associative Containers)
set<int> s;                      // 集合对象 （有序？因为红黑树实现）
map<string, int> m;              // 映射对象 （有序？因为红黑树实现）
multiset<int> ms;                // 多重集合对象
multimap<string, int> mm;        // 多重映射对象

// 无序容器 (Unordered Containers)
unordered_set<int> us;           // 无序集合对象
unordered_map<string, int> um;   // 无序映射对象

// 适配器容器 (Container Adapters)
stack<int> stk;                  // 栈对象
queue<int> q;                    // 队列对象
priority_queue<int> pq;          // 优先队列对象
```

**容器对象特点：**

- 都有构造函数和析构函数
- 有丰富的成员函数 (size(), empty(), begin(), end()等)
- 自动管理内存
- 支持迭代器

## 4. 字面量 (Literals) 的默认类型

### 整数字面量

```cpp
42          // int
42L         // long
42LL        // long long
42U         // unsigned int
42UL        // unsigned long
42ULL       // unsigned long long

// 二进制、八进制、十六进制
0b1010      // int (二进制)
0755        // int (八进制)
0xFF        // int (十六进制)
```

### 浮点字面量

```cpp
3.14        // double (默认)
3.14f       // float
3.14L       // long double
1e10        // double (科学计数法)
```

### 字符和字符串字面量

```cpp
'A'         // char
L'中'       // wchar_t
u'A'        // char16_t
U'A'        // char32_t

"Hello"     // const char[6] (C风格字符串)
L"Hello"    // const wchar_t[]
u"Hello"    // const char16_t[]
U"Hello"    // const char32_t[]
u8"Hello"   // const char[] (UTF-8)

"Hello"s    // std::string (需要using namespace std::string_literals)
```

### 其他字面量

```cpp
true, false // bool
nullptr     // std::nullptr_t
```

## 5. struct vs class

### 语法上的区别

```cpp
// struct：成员默认为public
struct Point {
    int x, y;           // 默认public
    void print() {      // 默认public
        cout << x << ", " << y << endl;
    }
};

// class：成员默认为private
class Rectangle {
    int width, height;  // 默认private
public:
    void setSize(int w, int h) {
        width = w;
        height = h;
    }
};
```

### 使用场景

#### 使用 `struct` 的情况：

```cpp
// 1. 简单的数据聚合
struct Point2D {
    double x, y;
};

// 2. POD (Plain Old Data) 类型
struct Color {
    unsigned char r, g, b, a;
};

// 3. 配置或参数结构
struct Config {
    string filename;
    int timeout;
    bool debug_mode;
};

// 4. 与C代码兼容
struct CStyleStruct {
    int data;
    char name[32];
};
```

#### 使用 `class` 的情况：

```cpp
// 1. 需要封装和数据隐藏
class BankAccount {
private:
    double balance;     // 私有数据
public:
    void deposit(double amount);
    double getBalance() const;
};

// 2. 复杂的业务逻辑
class DatabaseConnection {
private:
    string connection_string;
    bool is_connected;
public:
    bool connect();
    void disconnect();
    void execute_query(const string& sql);
};

// 3. 需要继承和多态
class Shape {
public:
    virtual double area() const = 0;    // 纯虚函数
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override {
        return 3.14159 * radius * radius;
    }
};
```

## 6. 总结表格

### 数据类型分类

| 分类     | 类型     | 是否为对象 | 示例                            |
| -------- | -------- | ---------- | ------------------------------- |
| 基本类型 | 内置类型 | 否         | `int`, `char`, `double`, `bool` |
| 指针类型 | 内置类型 | 否         | `int*`, `char*`, `void*`        |
| 数组类型 | 复合类型 | 否         | `int[10]`, `char[]`             |
| 类类型   | 用户定义 | 是         | `string`, `vector`, 自定义类    |
| 枚举类型 | 用户定义 | 否         | `enum`, `enum class`            |

### 内存和性能对比

| 类型         | 内存分配  | 性能 | 功能丰富度 |
| ------------ | --------- | ---- | ---------- |
| 基本类型     | 栈/寄存器 | 最快 | 基本运算   |
| C 风格字符串 | 静态区    | 快   | C 库函数   |
| 字符数组     | 栈        | 快   | C 库函数   |
| STL 对象     | 堆+栈     | 中等 | 功能丰富   |

## 7. 最佳实践

```cpp
// ✅ 推荐用法
string name = "张三";                    // 使用string而不是char*
vector<int> numbers = {1, 2, 3};        // 使用vector而不是数组
auto value = 42;                        // 让编译器推导类型

// ✅ 明确的类型后缀
auto pi = 3.14;                         // double
auto pi_f = 3.14f;                      // float
auto count = 42U;                       // unsigned int

// ✅ 结构体用于数据聚合
struct Person {
    string name;
    int age;
    string email;
};

// ✅ 类用于封装和行为
class Timer {
private:
    chrono::steady_clock::time_point start_time;
public:
    void start();
    void stop();
    double elapsed_seconds() const;
};
```

这样你就能清楚地理解 C++中不同数据类型的本质和使用场景了！
