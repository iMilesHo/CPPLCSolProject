## Note

decimal part 小数部分

## 1. `nums.size() / 2` 在 C++ 中是什么类型？

- `nums.size()` 的返回值类型是 `std::vector<T>::size_type`，实际上是 **无符号整数类型**（`std::size_t`，通常是 `unsigned long` 或 `unsigned long long`，取决于平台）。
- `2` 是一个 `int` 字面量。
  当一个 `size_t`（无符号）和一个 `int`（有符号）进行运算时，C++ 会进行 **整型提升** 和 **类型转换**。

  - 通常情况：`int` 会被提升成 `size_t`（无符号，可能 64 位）。
  - 所以最终运算是 **无符号整数除法**。

- 除法结果依然是无符号整数，**小数部分会被直接截断（取整，floor toward zero）**，不会四舍五入。

举例：

```cpp
std::vector<int> nums(5);
auto result = nums.size() / 2; // 5 / 2 = 2
// result 类型是 size_t，值为 2
int half = nums.size() / 2;    // 自动从 size_t 转换为 int
// half = 2
```

所以：

- **不会四舍五入**。
- 结果会“向零取整”，即保留整数部分，丢掉小数。

---

## 2. 不同类型做除法的场景

| 场景                | 示例         | 结果类型           | 小数处理方式                        |
| ------------------- | ------------ | ------------------ | ----------------------------------- |
| int ÷ int           | `5 / 2`      | int                | 结果 `2`，小数被截断                |
| unsigned ÷ unsigned | `5u / 2u`    | unsigned           | 结果 `2`，小数被截断                |
| int ÷ double        | `5 / 2.0`    | double             | 结果 `2.5`                          |
| double ÷ int        | `5.0 / 2`    | double             | 结果 `2.5`                          |
| float ÷ double      | `5.0f / 2.0` | double             | 结果 `2.5`（float 会提升成 double） |
| long long ÷ int     | `5LL / 2`    | long long          | 结果 `2`                            |
| size_t ÷ int        | `5ull / 2`   | unsigned long long | 结果 `2`                            |

---

## 3. 如何“去掉小数部分” vs “四舍五入”

### 去掉小数部分（truncate toward zero）

- 整数除法天然就是这样：`5 / 2 == 2`
- 对浮点数也可以用：

  ```cpp
  double x = 5.7;
  int y = static_cast<int>(x); // y == 5, 小数直接丢弃
  ```

  或者

  ```cpp
  std::trunc(x); // 返回 5.0，<cmath>
  ```

### 四舍五入

- 使用 `std::round`：

  ```cpp
  double x = 5.7;
  int y = static_cast<int>(std::round(x)); // y == 6
  double z = std::round(2.3); // z == 2.0
  ```

- 或者手写逻辑：

  ```cpp
  int rounded = (a + b/2) / b; // 当 a,b 是正整数时，模拟四舍五入
  ```

### 向下取整（floor）/ 向上取整（ceil）

- `std::floor(5.7)` → 5
- `std::ceil(5.1)` → 6
- 注意：floor/ceil 结果是浮点数，再转成 int。

---

## 4. 面试中常见的坑点

1. **整数除法**：`7 / 2 == 3`，而不是 `3.5`。
2. **强制类型转换**：如果想要浮点结果，至少一个操作数得是浮点：

   ```cpp
   double x = nums.size() / 2.0; // OK, 得到浮点
   ```

3. **size_t → int 的隐式转换**：当数组非常大时（> 2^31），`nums.size()/2` 可能超出 `int` 范围，导致截断或溢出。
   在 LeetCode 这种规模一般没事，但在生产环境里要注意。

---

✅ 总结一下：

- `nums.size() / 2` 是无符号整数除法，结果是 `size_t`，小数部分直接去掉，不会四舍五入。
- 如果赋值给 `int`，会发生隐式转换（可能有溢出风险）。
- 四舍五入需要显式用 `std::round`；向下/向上取整用 `std::floor` / `std::ceil`。

---

要不要我帮你画一个 **“整数 vs 浮点除法行为对比表”**（一页 cheat sheet）？这样你面试前背一下就能快速答出来。
