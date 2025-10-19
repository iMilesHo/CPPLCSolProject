# C++ 算术运算与数据类型转换 Cheat Sheet

## 1. 基本数据类型范围

### 整数类型

```cpp
int           // 32位: -2,147,483,648 到 2,147,483,647
long          // 64位: -9,223,372,036,854,775,808 到 9,223,372,036,854,775,807
long long     // 64位: 同上
unsigned int  // 32位: 0 到 4,294,967,295
```

### 浮点类型

```cpp
float         // 32位: ~7位有效数字, ±3.4 × 10^±38
double        // 64位: ~15位有效数字, ±1.7 × 10^±308
long double   // 80位或128位: 更高精度
```

### sizeof 运算符结果（常见系统）

```cpp
sizeof(char)      // 1 字节 (总是1，按定义)
sizeof(short)     // 2 字节
sizeof(int)       // 4 字节 (32位和64位系统都是4)
sizeof(long)      // 4 字节 (32位系统) 或 8 字节 (64位系统)
sizeof(long long) // 8 字节
sizeof(float)     // 4 字节
sizeof(double)    // 8 字节
sizeof(long double) // 8, 12, 或 16 字节 (取决于系统)

// 指针大小
sizeof(int*)      // 4 字节 (32位系统) 或 8 字节 (64位系统)
sizeof(void*)     // 同上
```

**重要注意事项：**

- `sizeof` 返回的是字节数，类型为 `size_t`
- `sizeof(int)` 在大多数现代系统上都是 4，即使是 64 位系统
- 可以通过 `#include <climits>` 使用 `INT_MAX`, `LONG_MAX` 等常量

### 检查你系统上的实际大小

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << "sizeof(int): " << sizeof(int) << " bytes" << std::endl;
    std::cout << "sizeof(long): " << sizeof(long) << " bytes" << std::endl;
    std::cout << "sizeof(long long): " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "sizeof(double): " << sizeof(double) << " bytes" << std::endl;

    std::cout << "INT_MAX: " << INT_MAX << std::endl;
    std::cout << "LONG_MAX: " << LONG_MAX << std::endl;

    return 0;
}
```

## 2. 整数溢出 (Integer Overflow)

### 什么时候会发生溢出？

```cpp
int a = 2147483647;  // int的最大值
int b = a + 1;       // 溢出！结果是 -2147483648 (未定义行为)

// 乘法溢出示例
int x = 100000;
int y = 30000;
int result = x * y;  // 3,000,000,000 > int最大值，溢出！
```

### 如何避免溢出？

```cpp
// 方法1: 使用更大的数据类型
long long result = (long long)x * y;

// 方法2: 在乘法前检查
if (x > INT_MAX / y) {
    // 会溢出，需要特殊处理
}

// 方法3: 使用无符号类型（如果不需要负数）
unsigned int result = (unsigned int)x * y;
```

## 3. 算术运算中的自动类型提升

### 基本规则（从高到低优先级）

1. 如果一个操作数是 `long double` → 结果是 `long double`
2. 如果一个操作数是 `double` → 结果是 `double`
3. 如果一个操作数是 `float` → 结果是 `float`
4. 否则，整数提升规则适用

### 整数提升规则

```cpp
// 小于int的类型自动提升为int
char a = 100;
short b = 200;
int result = a + b;  // char和short都提升为int，结果是int

// 不同大小整数运算
int a = 10;
long long b = 20;
auto result = a + b;  // 结果是long long类型
```

## 4. 混合类型算术运算示例

### int 和 double 运算

```cpp
int a = 5;
double b = 2.0;

auto result1 = a + b;    // 结果: 7.0 (double类型)
auto result2 = a / b;    // 结果: 2.5 (double类型)
auto result3 = a * b;    // 结果: 10.0 (double类型)

// 重要：整数除法 vs 浮点除法
int x = 5, y = 2;
int result4 = x / y;        // 结果: 2 (整数除法，舍去小数)
double result5 = x / y;     // 结果: 2.0 (先做整数除法，再转换为double)
double result6 = (double)x / y;  // 结果: 2.5 (浮点除法)
double result7 = x / (double)y;  // 结果: 2.5 (浮点除法)
double result8 = x * 1.0 / y;    // 结果: 2.5 (浮点除法)
```

### 实际例子

```cpp
// 错误的平均值计算
int sum = 10;
int count = 3;
double avg1 = sum / count;        // 结果: 3.0 (错误！)

// 正确的平均值计算
double avg2 = (double)sum / count;  // 结果: 3.333...
double avg3 = sum / (double)count;  // 结果: 3.333...
double avg4 = sum * 1.0 / count;   // 结果: 3.333...
```

## 5. 类型转换和数据接收

### 隐式转换（自动转换）

```cpp
double d = 3.14;
int i = d;          // 隐式转换: i = 3 (小数部分丢失)

int a = 1000000;
float f = a;        // 可能丢失精度 (float只有7位有效数字)

long long big = 9223372036854775807LL;
int small = big;    // 数据截断，可能得到错误结果
```

### 显式转换（强制转换）

```cpp
double d = 3.8;
int i = (int)d;           // C风格转换: i = 3
int j = static_cast<int>(d);  // C++风格转换: j = 3

// 更安全的转换
#include <cmath>
int k = std::round(d);    // k = 4 (四舍五入)
```

## 6. 常见陷阱和最佳实践

### 陷阱 1: 整数除法

```cpp
// 错误
double average = sum / count;  // 如果sum和count都是int

// 正确
double average = static_cast<double>(sum) / count;
// 或者
double average = sum / static_cast<double>(count);
```

### 陷阱 2: 溢出检查

```cpp
// 危险：可能溢出
if (a + b > limit) { ... }

// 安全：避免溢出
if (a > limit - b) { ... }
```

### 陷阱 3: 浮点比较

```cpp
// 错误：浮点数不应该直接比较相等
if (a == b) { ... }

// 正确：使用epsilon比较
const double EPSILON = 1e-9;
if (std::abs(a - b) < EPSILON) { ... }
```

## 7. 针对原题的解决方案分析

### 错误的整数方法

```cpp
// 为什么失败：可能的溢出和精度问题
if (previousSum < responseTimes[i] * (long long)i) {
    // responseTimes[i] * i 可能非常大
    // 即使用long long，也可能有边界情况
}
```

### 正确的浮点方法

```cpp
// 为什么成功：避免溢出，精确计算平均值
double previousSum = responseTimes[0];  // 使用double存储
for(int i = 1; i < n; ++i){
    double avg = previousSum / i;       // 精确的浮点除法
    if (responseTimes[i] > avg) {       // 直接比较
        count++;
    }
    previousSum += responseTimes[i];    // double累加
}
```

## 8. 最佳实践总结

1. **算术运算时**：明确知道结果的数据类型
2. **除法运算时**：确保至少一个操作数是浮点类型（如果需要小数结果）
3. **大数运算时**：考虑使用`long long`或`double`避免溢出
4. **精度要求高时**：使用`double`而不是`float`
5. **类型转换时**：优先使用`static_cast`而不是 C 风格转换
6. **浮点比较时**：使用 epsilon 比较而不是直接相等比较

## 9. 快速参考

```cpp
// 安全的除法
double result = static_cast<double>(a) / b;

// 安全的乘法（避免溢出）
long long result = static_cast<long long>(a) * b;

// 浮点比较
bool isEqual = std::abs(a - b) < 1e-9;

// 四舍五入
int rounded = static_cast<int>(std::round(doubleValue));
```

## 10. Struct 大小计算与内存对齐

### 内存对齐基本规则

1. **自然对齐**：每个成员按其大小对齐（char 按 1 字节，int 按 4 字节，double 按 8 字节）
2. **结构体总大小**：必须是最大成员大小的倍数
3. **填充字节**：编译器会在成员之间插入填充字节以满足对齐要求

### 实际例子

#### 例子 1：不同顺序的影响

```cpp
// 顺序1：浪费内存
struct BadOrder {
    char c1;      // 1 byte
    // 3 bytes padding
    int i;        // 4 bytes
    char c2;      // 1 byte
    // 3 bytes padding (为了整体大小是4的倍数)
};
// sizeof(BadOrder) = 12 bytes

// 顺序2：节省内存
struct GoodOrder {
    int i;        // 4 bytes
    char c1;      // 1 byte
    char c2;      // 1 byte
    // 2 bytes padding
};
// sizeof(GoodOrder) = 8 bytes
```

#### 例子 2：包含数组的结构体

```cpp
struct WithArray {
    char s[4];    // 4 bytes (连续存储，无填充)
    int i;        // 4 bytes
    char c;       // 1 byte
    // 3 bytes padding
};
// sizeof(WithArray) = 12 bytes

struct OptimizedWithArray {
    int i;        // 4 bytes
    char s[4];    // 4 bytes
    char c;       // 1 byte
    // 3 bytes padding
};
// sizeof(OptimizedWithArray) = 12 bytes (这个例子中优化效果不明显)
```

#### 例子 3：更复杂的例子

```cpp
struct Complex {
    char c1;      // 1 byte
    // 3 bytes padding
    int i1;       // 4 bytes
    char s[4];    // 4 bytes (无填充，因为已经对齐)
    char c2;      // 1 byte
    // 3 bytes padding
    int i2;       // 4 bytes
    char c3;      // 1 byte
    // 3 bytes padding (结构体总大小必须是4的倍数)
};
// sizeof(Complex) = 24 bytes
```

### 实际测试代码

```cpp
#include <iostream>
using namespace std;

struct Test1 {
    char c;       // 1 + 3 padding
    int i;        // 4
    char c2;      // 1 + 3 padding
};

struct Test2 {
    int i;        // 4
    char c;       // 1
    char c2;      // 1 + 2 padding
};

struct Test3 {
    char s[4];    // 4
    int i;        // 4
    char c;       // 1 + 3 padding
};

int main() {
    cout << "sizeof(Test1): " << sizeof(Test1) << endl;  // 12
    cout << "sizeof(Test2): " << sizeof(Test2) << endl;  // 8
    cout << "sizeof(Test3): " << sizeof(Test3) << endl;  // 12
    return 0;
}
```

## 11. 数组的 sizeof 计算

### 基本规则

```cpp
int a[10];              // 数组声明
sizeof(a);              // 数组总大小：10 * sizeof(int) = 40 bytes
sizeof(*a);             // 第一个元素大小：sizeof(int) = 4 bytes
sizeof(a[0]);           // 同上：sizeof(int) = 4 bytes
sizeof(a) / sizeof(*a); // 数组元素个数：40/4 = 10
```

### 详细例子

```cpp
// 不同类型的数组
char str[20];           // sizeof(str) = 20 * 1 = 20 bytes
int numbers[5];         // sizeof(numbers) = 5 * 4 = 20 bytes
double values[3];       // sizeof(values) = 3 * 8 = 24 bytes

// 字符串数组
char name[] = "Hello";  // sizeof(name) = 6 bytes (包含'\0')
char greeting[10] = "Hi"; // sizeof(greeting) = 10 bytes (固定大小)

// 多维数组
int matrix[3][4];       // sizeof(matrix) = 3 * 4 * 4 = 48 bytes
                        // sizeof(matrix[0]) = 4 * 4 = 16 bytes (一行)
                        // sizeof(matrix[0][0]) = 4 bytes (一个元素)
```

### 数组作为函数参数的陷阱

```cpp
void function(int arr[10]) {
    // 陷阱！arr 在这里实际上是指针
    cout << sizeof(arr) << endl;        // 8 bytes (指针大小，64位系统)
    cout << sizeof(*arr) << endl;       // 4 bytes (int大小)
    // 无法通过 sizeof(arr)/sizeof(*arr) 获得数组长度！
}

void function2(int (&arr)[10]) {
    // 正确！arr 是数组引用
    cout << sizeof(arr) << endl;        // 40 bytes (数组实际大小)
    cout << sizeof(*arr) << endl;       // 4 bytes
    cout << sizeof(arr)/sizeof(*arr) << endl; // 10 (正确的长度)
}

int main() {
    int numbers[10];
    cout << sizeof(numbers) << endl;    // 40 bytes

    function(numbers);    // 传递给函数时退化为指针
    function2(numbers);   // 传递数组引用，保持数组性质
}
```

### 实用宏定义

```cpp
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

int data[15];
int size = ARRAY_SIZE(data);  // size = 15

// 注意：这个宏只能用于真正的数组，不能用于指针！
```

### 字符串相关的 sizeof

```cpp
char str1[] = "Hello";       // sizeof(str1) = 6 (包含'\0')
char str2[10] = "Hello";     // sizeof(str2) = 10 (数组大小)
char *str3 = "Hello";        // sizeof(str3) = 8 (指针大小)
string str4 = "Hello";       // sizeof(str4) = 32 (string对象大小，与内容无关)

cout << strlen(str1) << endl;  // 5 (字符串长度，不包含'\0')
cout << str4.length() << endl; // 5 (string长度)
```

## 12. 内存对齐控制

### 使用 pragma pack 控制对齐

```cpp
#pragma pack(1)  // 按1字节对齐（无填充）
struct Packed {
    char c;      // 1 byte
    int i;       // 4 bytes
    char c2;     // 1 byte
};
// sizeof(Packed) = 6 bytes (无填充)
#pragma pack()   // 恢复默认对齐

struct Normal {
    char c;      // 1 byte + 3 padding
    int i;       // 4 bytes
    char c2;     // 1 byte + 3 padding
};
// sizeof(Normal) = 12 bytes (有填充)
```

### 使用 alignas 指定对齐（C++11）

```cpp
struct Aligned {
    alignas(8) char c;  // 按8字节对齐
    int i;
};
// sizeof(Aligned) 可能更大
```
