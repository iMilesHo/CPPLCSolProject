# 快速要点（30 秒读完）

- **优先级（从安全到危险）：** `static_cast` → `dynamic_cast`（运行时检查） → `const_cast`（仅去/加限定符） → `reinterpret_cast`（生猛底层，能少用就少用）。
- **能不用转换就不用**：先用更精确的类型与接口设计规避强转。
- **`static_cast` 的三大好处：**

  1. 语义明确（读代码即知“编译期可验证的转换”）
  2. 比 C 风格更安全（很多错误会在编译期被拒）
  3. 便于全局搜索与审计（`grep static_cast`）

---

# 四种 `cast` 对比表

| Cast                     | 目的/语义                       | 允许的转换（常见）                                                                                   | 安全性/检查                        | 失败时行为                                                 | 典型用法                                                          | 忌用场景                                            |
| ------------------------ | ------------------------------- | ---------------------------------------------------------------------------------------------------- | ---------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------------- | --------------------------------------------------- |
| `static_cast<T>(x)`      | 编译期可验证的**静态**转换      | 数值类型之间；`void* ↔ T*`（**仅当原本就是 T**\*）；基 ↔ 派（**上行安全、下行不检验**）；枚举 ↔ 整数 | **编译期**检查；不做 RTTI 实时校验 | 非法转换直接**编译错误**                                   | 数值换算、枚举 ↔ 整型、已知安全的上行/受控下行、`size_t→int` 明示 | 真实类型不确定时的下行（可能 UB），跨不相关类型指针 |
| `dynamic_cast<T>(p)`     | 多态场景的**运行时**安全下行    | 仅用于**多态**类型（有虚函数）；基 → 派安全下行；交叉/上行                                           | **运行时 RTTI** 检查               | 指针：失败返回 **`nullptr`**；引用：抛 **`std::bad_cast`** | 安全下行、判断对象实际类型、接口分派                              | 非多态类型（无虚析构/虚函数）；RTTI 被禁用的场景    |
| `const_cast<T>(x)`       | 添加/移除 `const/volatile` 限定 | `const T* ↔ T*`，`const T& ↔ T&`                                                                     | 仅修饰限定符，不改底层类型         | 编译期允许；**若真的修改了本就 const 的对象 → UB**         | 第三方 API 需要非常量参数、逻辑上“可变视图”                       | 去 const 后修改真正的常量对象                       |
| `reinterpret_cast<T>(x)` | 重新解释位模式（极低层）        | 几乎任何指针/整数间重解释（实现相关）；函数指针间重解释                                              | 无检查，**最不安全**，可移植性差   | 编译能过；**运行期易 UB**                                  | 与平台/ABI 绑定的底层优化、序列化框架的标签式指针等               | 类型别名惩罚、跨对象模型、函数/对象指针随意互转     |

> 口诀：**能 `static_cast` 就别 `reinterpret_cast`；下行要安全就用 `dynamic_cast`；仅改限定符用 `const_cast`。**

---

# 常见场景与写法

### 1) 数值类型转换（浮点 ↔ 整数、有符号 ↔ 无符号）

```cpp
double d = 3.14;
int i = static_cast<int>(d);            // 3，截断
size_t n = 100;
int m = static_cast<int>(n);            // 明示潜在范围缩减
```

### 2) 容器大小与索引（避免无符号/有符号坑）

```cpp
int half = static_cast<int>(nums.size()) / 2;
// C++20：std::ssize(nums) 返回有符号
int half2 = std::ssize(nums) / 2;
```

### 3) 基类/派生类转换

```cpp
struct Base { virtual ~Base() = default; };
struct Derived : Base {};

Base* b = new Derived;
// 上行（派→基）安全
Base* up = static_cast<Base*>(b);
// 下行（基→派）：dynamic_cast 安全，有 RTTI 检查
if (Derived* down = dynamic_cast<Derived*>(b)) { /* ok */ }
```

### 4) `void*` 恢复到原类型

```cpp
int x = 42;
void* p = &x;
int* px = static_cast<int*>(p);     // ✅ 仅当 p 原本指向 int
// double* pd = static_cast<double*>(p); // ❌ 编译时报错（好事！）
```

### 5) 去掉 const（仅界面适配，别乱改真常量）

```cpp
void takes_mutable(char* buf);
void wrapper(const std::string& s) {
    takes_mutable(const_cast<char*>(s.data())); // 仅当被调方不写入
}
```

### 6) 低层重解释（慎用）

```cpp
std::uintptr_t bits = reinterpret_cast<std::uintptr_t>(px); // 指针→整数
auto* back = reinterpret_cast<int*>(bits);                  // 效果/可移植性依赖平台
```

---

# 典型易错与面试陷阱

- **`nums.size() / -2`**：无符号与有符号混算，`-2` 被转成巨大无符号 → 结果怪异。
  ✅ 先转有符号：`static_cast<int>(nums.size()) / -2` 或 `std::ssize(nums) / -2`。
- **错误下行用 `static_cast`**：真实对象不是该派生类 → **UB**。
  ✅ 用 `dynamic_cast`，失败返回 `nullptr`。
- **`const_cast` 后修改常量对象**：**UB**。仅在你确定底层对象本就可变时使用。
- **把 C 风格 `(T)x` 当万能钥匙**：会隐式做多种 cast，掩盖危险。
  ✅ 用显式的四种 `cast` 区分语义。
- **`reinterpret_cast` 滥用**：跨类型别名、对象模型差异 → 易**未定义行为**且不可移植。

---

# 选型速决（决策树）

1. 只是**数值换算 / 枚举 ↔ 整数 / 上行**？→ **`static_cast`**
2. 需要在**多态**层次结构里**安全下行**？→ **`dynamic_cast`**（失败可检测）
3. 只是**去/加 const/volatile** 限定？→ **`const_cast`**（别改真常量）
4. 必须**按位重解释**（平台相关，最后手段）？→ **`reinterpret_cast`**

> 若你还在犹豫：十有八九该用 `static_cast` 或者改代码设计让强转消失。

---

# “为什么用 `static_cast`（面试版 20 秒）”

> C 风格转换太宽泛，可能悄悄做危险转换而不报错；`static_cast` 语义清晰、编译期校验、失败就报错，既安全又可读、也便于搜索审计。在工程里，几乎所有“正常的类型转换”都应优先用 `static_cast`。
