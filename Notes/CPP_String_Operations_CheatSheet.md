# C++ 字符串操作 Cheat Sheet

## 1. 基本概念

### 1.1 字符串类型

```cpp
#include <string>
using namespace std;

string str1 = "Hello";           // 标准字符串
const char* str2 = "World";      // C风格字符串
char arr[] = "Array";            // 字符数组
```

### 1.2 字符串初始化

```cpp
string s1;                       // 空字符串
string s2("Hello");              // 从C字符串初始化
string s3 = "World";             // 赋值初始化
string s4(10, 'A');              // 10个'A'字符
string s5(s2);                   // 拷贝构造
string s6 = s2;                  // 拷贝赋值
```

## 2. 字符串长度和容量

```cpp
string str = "Hello World";

str.size();                      // 返回字符串长度 (11)
str.length();                    // 同 size()，返回长度 (11)
str.empty();                     // 检查是否为空 (false)
str.capacity();                  // 返回当前分配的容量
str.max_size();                  // 返回最大可能长度
```

## 3. 字符串访问

### 3.1 索引访问

```cpp
string str = "Hello";

char c1 = str[0];                // 'H' (不检查边界)
char c2 = str.at(1);             // 'e' (检查边界，可能抛异常)
char& first = str.front();       // 'H' (第一个字符)
char& last = str.back();         // 'o' (最后一个字符)
const char* data = str.data();   // 获取C字符串指针
const char* c_str = str.c_str(); // 获取以null结尾的C字符串
```

### 3.2 迭代器访问

```cpp
string str = "Hello";

auto it = str.begin();           // 指向第一个字符
auto it_end = str.end();         // 指向最后一个字符之后
auto rit = str.rbegin();         // 反向迭代器，指向最后一个字符
auto rit_end = str.rend();       // 反向迭代器，指向第一个字符之前

// 遍历字符串
for (auto it = str.begin(); it != str.end(); ++it) {
    cout << *it << " ";
}
```

## 4. 字符串修改操作

### 4.1 赋值

```cpp
string str;

str = "Hello";                   // 赋值
str.assign("World");             // assign函数
str.assign(10, 'A');             // 10个'A'
str.assign(other_str, 2, 3);     // 从other_str的位置2开始取3个字符
```

### 4.2 追加操作

```cpp
string str = "Hello";

str += " World";                 // 操作符+=
str.append(" !");                // append函数
str.append(3, '!');              // 追加3个'!'
str.push_back('?');              // 在末尾添加单个字符
```

### 4.3 插入操作

```cpp
string str = "Hello World";

str.insert(5, " Beautiful");     // 在位置5插入字符串
str.insert(0, 1, 'X');           // 在位置0插入1个'X'
str.insert(str.begin() + 2, 'Y'); // 使用迭代器插入
```

### 4.4 删除操作

```cpp
string str = "Hello World";

str.erase(5, 6);                 // 从位置5删除6个字符
str.erase(5);                    // 删除位置5之后的所有字符
str.erase(str.begin() + 2);      // 使用迭代器删除单个字符
str.pop_back();                  // 删除最后一个字符
str.clear();                     // 清空字符串
```

### 4.5 替换操作

```cpp
string str = "Hello World";

str.replace(6, 5, "C++");        // 将位置6开始的5个字符替换为"C++"
str.replace(str.begin(), str.begin() + 5, "Hi"); // 使用迭代器替换
```

## 5. 子字符串操作

### 5.1 substr() 函数 - ⭐ 重点

```cpp
string str = "Hello World";

string sub1 = str.substr();      // 整个字符串 "Hello World"
string sub2 = str.substr(6);     // 从位置6到末尾 "World"
string sub3 = str.substr(0, 5);  // 从位置0开始5个字符 "Hello"
string sub4 = str.substr(6, 3);  // 从位置6开始3个字符 "Wor"

// ❌ 错误用法：substr不接受迭代器
// str.substr(str.begin(), 5);   // 编译错误！

// ✅ 正确用法：substr接受位置索引和长度
string correct = str.substr(0, spaceIndex);  // 你代码中的正确用法
```

## 6. 查找操作

### 6.1 find() 系列函数

```cpp
string str = "Hello World Hello";

size_t pos1 = str.find("World");         // 查找子字符串，返回6
size_t pos2 = str.find('o');             // 查找字符，返回4
size_t pos3 = str.find("o", 5);          // 从位置5开始查找，返回7
size_t pos4 = str.find("xyz");           // 未找到，返回string::npos

// 反向查找
size_t rpos1 = str.rfind("Hello");       // 从后向前查找，返回12
size_t rpos2 = str.rfind('l');           // 返回15

// 查找任意字符
size_t any1 = str.find_first_of("aeiou"); // 查找第一个元音，返回1('e')
size_t any2 = str.find_last_of("lo");     // 查找最后一个'l'或'o'，返回15
size_t not1 = str.find_first_not_of("Hel"); // 查找第一个不是'H','e','l'的字符

// 检查是否找到
if (pos1 != string::npos) {
    cout << "Found at position: " << pos1 << endl;
}
```

## 7. 字符串比较

```cpp
string str1 = "apple";
string str2 = "banana";

// 操作符比较
bool equal = (str1 == str2);             // false
bool not_equal = (str1 != str2);         // true
bool less = (str1 < str2);               // true (字典序)
bool greater = (str1 > str2);            // false

// compare函数
int result = str1.compare(str2);         // < 0 (str1 < str2)
int result2 = str1.compare(0, 3, str2, 0, 3); // 比较子字符串
```

## 8. 字符串转换

### 8.1 字符串转数值 - ⭐ 重点

```cpp
#include <string>

string numStr = "123";
string floatStr = "45.67";
string hexStr = "0x1A";

// 转换为整数
int i = stoi(numStr);                    // 123
long l = stol(numStr);                   // 123L
long long ll = stoll(numStr);            // 123LL

// 转换为浮点数
float f = stof(floatStr);                // 45.67f
double d = stod(floatStr);               // 45.67
long double ld = stold(floatStr);        // 45.67L

// 带进制转换
int hex = stoi(hexStr, nullptr, 16);     // 26 (十六进制转换)
int bin = stoi("1010", nullptr, 2);      // 10 (二进制转换)

// 你代码中的用法示例
string record = "Alice 125";
auto spaceIndex = record.find(" ");
int time = stoi(record.substr(spaceIndex + 1)); // 125
```

### 8.2 数值转字符串

```cpp
int num = 123;
double pi = 3.14159;

string str1 = to_string(num);            // "123"
string str2 = to_string(pi);             // "3.141590" (默认精度)
```

## 9. 字符串流操作

```cpp
#include <sstream>

// 字符串流
stringstream ss;
ss << "Hello " << 123 << " World";
string result = ss.str();                // "Hello 123 World"

// 从字符串读取
string data = "10 20 30";
stringstream input(data);
int a, b, c;
input >> a >> b >> c;                    // a=10, b=20, c=30
```

## 10. STL 算法与字符串操作 - ⭐ 重点扩展

### 10.1 必须包含的头文件

```cpp
#include <algorithm>    // 大部分STL算法
#include <numeric>      // accumulate, iota等
#include <functional>   // greater, less等函数对象
#include <cctype>       // toupper, tolower, isalnum等
#include <iterator>     // back_inserter, distance等
```

### 10.2 排序相关算法

```cpp
string str = "dcba";
vector<string> words = {"banana", "apple", "cherry"};

// 基本排序
sort(str.begin(), str.end());                    // "abcd"
sort(words.begin(), words.end());                // 字典序排序

// 自定义排序
sort(words.begin(), words.end(), [](const string& a, const string& b) {
    return a.length() < b.length();              // 按长度排序
});

// 部分排序
string s = "fedcba";
partial_sort(s.begin(), s.begin() + 3, s.end()); // 只排序前3个："abcfed"

// 稳定排序
stable_sort(words.begin(), words.end());         // 保持相等元素的相对顺序

// 检查是否已排序
bool sorted = is_sorted(str.begin(), str.end()); // true/false
auto pos = is_sorted_until(str.begin(), str.end()); // 返回第一个乱序位置
```

### 10.3 查找算法

```cpp
string str = "Hello World";
vector<string> words = {"apple", "banana", "cherry"};

// 线性查找
auto it = find(str.begin(), str.end(), 'o');     // 找到第一个'o'
auto it2 = find(words.begin(), words.end(), "banana"); // 找到"banana"

// 条件查找
auto it3 = find_if(str.begin(), str.end(), [](char c) {
    return c >= 'A' && c <= 'Z';                 // 找第一个大写字母
});

auto it4 = find_if_not(str.begin(), str.end(), ::isalpha); // 找第一个非字母

// 查找子序列
string pattern = "llo";
auto it5 = search(str.begin(), str.end(), pattern.begin(), pattern.end());

// 查找多个连续相同元素
string s = "aabbcc";
auto it6 = search_n(s.begin(), s.end(), 2, 'b'); // 查找连续2个'b'

// 二分查找（需要已排序）
sort(words.begin(), words.end());
bool found = binary_search(words.begin(), words.end(), "banana");
auto lb = lower_bound(words.begin(), words.end(), "banana"); // 第一个>=的位置
auto ub = upper_bound(words.begin(), words.end(), "banana"); // 第一个>的位置
```

### 10.4 计数和统计算法

```cpp
string str = "Hello World";

// 计数
int count_l = count(str.begin(), str.end(), 'l'); // 统计'l'的个数：3
int count_vowels = count_if(str.begin(), str.end(), [](char c) {
    return string("aeiouAEIOU").find(c) != string::npos;
}); // 统计元音个数

// 最值
auto min_char = min_element(str.begin(), str.end()); // 最小字符
auto max_char = max_element(str.begin(), str.end()); // 最大字符
auto [min_it, max_it] = minmax_element(str.begin(), str.end()); // C++17

// 累积操作
string letters = "abc";
string result = accumulate(letters.begin(), letters.end(), string(""));
// 结果："abc"

// 字符串长度累加
vector<string> words = {"a", "bb", "ccc"};
int total_len = accumulate(words.begin(), words.end(), 0,
    [](int sum, const string& s) { return sum + s.length(); }); // 6
```

### 10.5 变换算法

```cpp
string str = "Hello World";

// 大小写转换
transform(str.begin(), str.end(), str.begin(), ::toupper); // "HELLO WORLD"
transform(str.begin(), str.end(), str.begin(), ::tolower); // "hello world"

// 自定义转换
transform(str.begin(), str.end(), str.begin(), [](char c) {
    return (c == ' ') ? '_' : c;                 // 空格转下划线
});

// 双序列转换
string s1 = "abc";
string s2 = "123";
string result(3, ' ');
transform(s1.begin(), s1.end(), s2.begin(), result.begin(),
    [](char c1, char c2) { return c1 + c2; });  // 结果：字符相加
```

### 10.6 复制和移动算法

```cpp
string source = "Hello";
string dest(10, ' ');

// 基本复制
copy(source.begin(), source.end(), dest.begin());

// 条件复制
copy_if(source.begin(), source.end(), dest.begin(), ::isalpha);

// 反向复制
copy_backward(source.begin(), source.end(), dest.end());

// 复制n个元素
copy_n(source.begin(), 3, dest.begin());        // 复制前3个字符

// 使用back_inserter动态增长
vector<char> result;
copy(source.begin(), source.end(), back_inserter(result));
```

### 10.7 替换和填充算法

```cpp
string str = "Hello World";

// 替换
replace(str.begin(), str.end(), 'l', 'L');       // "HeLLo WorLd"

// 条件替换
replace_if(str.begin(), str.end(), ::islower, 'X'); // 小写字母替换为'X'

// 填充
fill(str.begin(), str.end(), 'A');               // 全部填充为'A'
fill_n(str.begin(), 5, 'B');                     // 前5个填充为'B'
```

### 10.8 去重和分区算法

```cpp
string str = "aabbccaabb";

// 排序后去重
sort(str.begin(), str.end());                    // "aabbbbccaa"
auto new_end = unique(str.begin(), str.end());   // 去除连续重复
str.erase(new_end, str.end());                   // "abc"

// 分区（不改变相对顺序使用stable_partition）
string s = "Hello123World456";
auto it = partition(s.begin(), s.end(), ::isalpha); // 字母在前，数字在后

// 检查分区
bool is_partitioned = is_partitioned(s.begin(), s.end(), ::isalpha);
```

### 10.9 序列操作算法

```cpp
string str = "Hello World";

// 反转
reverse(str.begin(), str.end());                 // "dlroW olleH"

// 旋转
rotate(str.begin(), str.begin() + 5, str.end()); // 将前5个字符移到末尾

// 随机打乱
random_device rd;
mt19937 g(rd());
shuffle(str.begin(), str.end(), g);              // 随机打乱字符顺序

// 下一个/上一个排列
string s = "abc";
next_permutation(s.begin(), s.end());            // "acb"
prev_permutation(s.begin(), s.end());            // "abc"
```

### 10.10 集合操作算法（需要已排序）

```cpp
string s1 = "aabbc";  // 已排序
string s2 = "bbccd";  // 已排序
string result;

// 并集
set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
          back_inserter(result));                // "aabbccd"

// 交集
result.clear();
set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                 back_inserter(result));         // "bbc"

// 差集
result.clear();
set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
               back_inserter(result));           // "aa"

// 对称差集
result.clear();
set_symmetric_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
                        back_inserter(result)); // "aacd"
```

### 10.11 实用的字符检查函数（<cctype>）

```cpp
char c = 'A';

// 字符类型检查
bool is_alpha = isalpha(c);      // 是否是字母
bool is_digit = isdigit(c);      // 是否是数字
bool is_alnum = isalnum(c);      // 是否是字母或数字
bool is_upper = isupper(c);      // 是否是大写字母
bool is_lower = islower(c);      // 是否是小写字母
bool is_space = isspace(c);      // 是否是空白字符
bool is_punct = ispunct(c);      // 是否是标点符号

// 字符转换
char upper_c = toupper(c);       // 转大写
char lower_c = tolower(c);       // 转小写
```

### 10.12 常用算法组合实例

```cpp
// 1. 统计字符串中每个字符出现次数
string str = "hello world";
map<char, int> freq;
for_each(str.begin(), str.end(), [&freq](char c) {
    freq[c]++;
});

// 2. 检查字符串是否是回文
string s = "racecar";
bool is_palindrome = equal(s.begin(), s.begin() + s.size()/2, s.rbegin());

// 3. 移除所有空格
string text = "Hello World";
auto new_end = remove(text.begin(), text.end(), ' ');
text.erase(new_end, text.end());        // "HelloWorld"

// 4. 查找最长的单词
vector<string> words = {"a", "hello", "world", "cpp"};
auto longest = max_element(words.begin(), words.end(),
    [](const string& a, const string& b) {
        return a.length() < b.length();
    });

// 5. 将字符串按长度分组
vector<string> words2 = {"cat", "dog", "bird", "fish"};
stable_sort(words2.begin(), words2.end(),
    [](const string& a, const string& b) {
        return a.length() < b.length();
    });

// 6. 检查字符串是否只包含字母和数字
string input = "Hello123";
bool is_valid = all_of(input.begin(), input.end(), ::isalnum);
```

## 11. 字符串与容器的配合使用

### 11.1 字符串与 vector 的配合

```cpp
#include <vector>
#include <algorithm>

// 字符串转字符数组
string str = "Hello";
vector<char> chars(str.begin(), str.end());

// 字符数组转字符串
vector<char> vec = {'H', 'e', 'l', 'l', 'o'};
string result(vec.begin(), vec.end());

// 处理字符串数组
vector<string> words = {"hello", "world", "cpp"};

// 查找最长字符串
auto longest = max_element(words.begin(), words.end(),
    [](const string& a, const string& b) {
        return a.length() < b.length();
    });

// 按长度排序
sort(words.begin(), words.end(),
    [](const string& a, const string& b) {
        return a.length() < b.length();
    });

// 过滤字符串
vector<string> filtered;
copy_if(words.begin(), words.end(), back_inserter(filtered),
    [](const string& s) { return s.length() > 3; });
```

### 11.2 字符串与 set/map 的配合

```cpp
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

string text = "hello world";

// 统计字符频率
unordered_map<char, int> char_freq;
for (char c : text) {
    char_freq[c]++;
}

// 获取唯一字符
set<char> unique_chars(text.begin(), text.end());

// 字符串去重（保持顺序）
string deduplicate(const string& str) {
    unordered_set<char> seen;
    string result;
    for (char c : str) {
        if (seen.find(c) == seen.end()) {
            seen.insert(c);
            result.push_back(c);
        }
    }
    return result;
}

// 单词统计
vector<string> words = {"apple", "banana", "apple", "cherry"};
map<string, int> word_count;
for (const string& word : words) {
    word_count[word]++;
}
```

### 11.3 字符串与 queue/stack 的配合

```cpp
#include <queue>
#include <stack>

// 使用栈检查括号匹配
bool isValidParentheses(const string& s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char top = st.top();
            st.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;
            }
        }
    }
    return st.empty();
}

// 使用队列进行字符串反转
string reverseWithQueue(const string& str) {
    queue<char> q;
    for (char c : str) {
        q.push(c);
    }
    string result;
    while (!q.empty()) {
        result = q.front() + result;  // 前插
        q.pop();
    }
    return result;
}
```

## 12. 字符串分割与连接 - 实用函数

### 12.1 多种分割方法

```cpp
#include <vector>
#include <sstream>
#include <regex>

// 方法1: 使用stringstream（单字符分隔符）
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {  // 跳过空字符串
            tokens.push_back(token);
        }
    }
    return tokens;
}

// 方法2: 手动分割（字符串分隔符）
vector<string> splitByString(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        if (end != start) {  // 跳过空字符串
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}

// 方法3: 使用正则表达式（C++11）
vector<string> splitRegex(const string& str, const string& pattern) {
    regex re(pattern);
    sregex_token_iterator first{str.begin(), str.end(), re, -1}, last;
    return {first, last};
}

// 方法4: 按多个字符分割
vector<string> splitByChars(const string& str, const string& delimiters) {
    vector<string> tokens;
    size_t start = str.find_first_not_of(delimiters);

    while (start != string::npos) {
        size_t end = str.find_first_of(delimiters, start);
        if (end == string::npos) {
            tokens.push_back(str.substr(start));
            break;
        }
        tokens.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delimiters, end);
    }
    return tokens;
}

// 使用示例
string text = "apple,banana,orange";
auto parts1 = split(text, ',');                    // {"apple", "banana", "orange"}

string text2 = "apple::banana::orange";
auto parts2 = splitByString(text2, "::");          // {"apple", "banana", "orange"}

string text3 = "apple banana\torange\ngrape";
auto parts3 = splitByChars(text3, " \t\n");        // {"apple", "banana", "orange", "grape"}
```

### 12.2 字符串连接方法

```cpp
#include <algorithm>
#include <numeric>

// 方法1: 使用循环
string join(const vector<string>& strings, const string& delimiter) {
    if (strings.empty()) return "";

    string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }
    return result;
}

// 方法2: 使用stringstream
string joinStream(const vector<string>& strings, const string& delimiter) {
    if (strings.empty()) return "";

    stringstream ss;
    ss << strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        ss << delimiter << strings[i];
    }
    return ss.str();
}

// 方法3: 使用accumulate（C++11）
string joinAccumulate(const vector<string>& strings, const string& delimiter) {
    if (strings.empty()) return "";

    return accumulate(strings.begin() + 1, strings.end(), strings[0],
        [&delimiter](const string& a, const string& b) {
            return a + delimiter + b;
        });
}

// 使用示例
vector<string> words = {"apple", "banana", "orange"};
string result1 = join(words, ", ");                // "apple, banana, orange"
string result2 = joinStream(words, " | ");         // "apple | banana | orange"
```

### 12.3 高级字符串处理

```cpp
// 去除首尾空白字符
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// 去除首空白
string ltrim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    return (start == string::npos) ? "" : str.substr(start);
}

// 去除尾空白
string rtrim(const string& str) {
    size_t end = str.find_last_not_of(" \t\n\r");
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}

// 字符串替换所有出现
string replaceAll(string str, const string& from, const string& to) {
    size_t start = 0;
    while ((start = str.find(from, start)) != string::npos) {
        str.replace(start, from.length(), to);
        start += to.length();
    }
    return str;
}

// 检查字符串开头
bool startsWith(const string& str, const string& prefix) {
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}

// 检查字符串结尾
bool endsWith(const string& str, const string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
```

## 12. 常见错误和注意事项

### 12.1 substr() 常见错误

```cpp
string str = "Hello World";
size_t pos = 5;

// ❌ 错误：substr不接受迭代器
// str.substr(str.begin(), pos);

// ✅ 正确：substr接受索引和长度
str.substr(0, pos);                      // "Hello"
str.substr(pos);                         // "World"
str.substr(pos, 3);                      // "Wor"
```

### 12.2 边界检查

```cpp
string str = "Hello";

// 安全访问
if (pos < str.length()) {
    char c = str[pos];
}

// 或使用at()进行自动边界检查
try {
    char c = str.at(pos);
} catch (const out_of_range& e) {
    cout << "Index out of range" << endl;
}
```

### 12.3 find() 返回值检查

```cpp
string str = "Hello World";
size_t pos = str.find("xyz");

// ✅ 正确检查
if (pos != string::npos) {
    // 找到了
}

// ❌ 错误：size_t是无符号类型，不要与-1比较
// if (pos != -1) { ... }
```

## 13. 性能建议

1. **预分配容量**：如果知道大概长度，使用`reserve()`
2. **避免频繁拼接**：大量字符串拼接时考虑使用`stringstream`
3. **传参使用引用**：`const string&` 避免不必要的拷贝
4. **移动语义**：C++11 后，适当使用`std::move`

```cpp
// 好的做法
void processString(const string& str) {  // 引用传参
    string result;
    result.reserve(str.size() * 2);      // 预分配
    // ... 处理
}

// 移动语义
string createString() {
    string result = "Very long string...";
    return result;  // 自动使用移动语义
}
```

## 14. 你的代码分析

在你的代码中：

```cpp
// 正确的用法
string name = record.substr(0, spaceIndex);
int time = stoi(record.substr(spaceIndex + 1));

// 错误用法（你提到的）
// record.substr(record.begin(), spaceIndex);  // 编译错误
```

**解释**：`substr()`函数的签名是：

- `substr(size_t pos = 0, size_t len = npos)`
- 第一个参数是起始位置（索引），第二个参数是长度
- 它不接受迭代器作为参数，所以`record.begin()`会导致编译错误

## 15. 算法题中的实用技巧

### 15.1 双指针技巧

```cpp
// 回文检查
bool isPalindrome(const string& s) {
    int left = 0, right = s.length() - 1;
    while (left < right) {
        if (s[left] != s[right]) return false;
        left++, right--;
    }
    return true;
}

// 移除重复字符（原地操作）
int removeDuplicates(string& s) {
    if (s.empty()) return 0;

    int slow = 0;
    for (int fast = 1; fast < s.length(); fast++) {
        if (s[fast] != s[slow]) {
            s[++slow] = s[fast];
        }
    }
    return slow + 1;
}

// 反转单词
string reverseWords(string s) {
    // 1. 反转整个字符串
    reverse(s.begin(), s.end());

    // 2. 反转每个单词
    int start = 0;
    for (int i = 0; i <= s.length(); i++) {
        if (i == s.length() || s[i] == ' ') {
            reverse(s.begin() + start, s.begin() + i);
            start = i + 1;
        }
    }
    return s;
}
```

### 15.2 滑动窗口技巧

```cpp
// 最长无重复字符子串
int lengthOfLongestSubstring(const string& s) {
    unordered_set<char> window;
    int left = 0, maxLen = 0;

    for (int right = 0; right < s.length(); right++) {
        while (window.count(s[right])) {
            window.erase(s[left++]);
        }
        window.insert(s[right]);
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}

// 最小覆盖子串
string minWindow(const string& s, const string& t) {
    unordered_map<char, int> need, window;
    for (char c : t) need[c]++;

    int left = 0, right = 0, valid = 0;
    int start = 0, len = INT_MAX;

    while (right < s.length()) {
        char c = s[right];
        right++;

        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) {
                valid++;
            }
        }

        while (valid == need.size()) {
            if (right - left < len) {
                start = left;
                len = right - left;
            }

            char d = s[left];
            left++;

            if (need.count(d)) {
                if (window[d] == need[d]) {
                    valid--;
                }
                window[d]--;
            }
        }
    }

    return len == INT_MAX ? "" : s.substr(start, len);
}
```

### 15.3 字符串匹配算法

```cpp
// KMP算法
vector<int> getNext(const string& pattern) {
    vector<int> next(pattern.length(), 0);
    int j = 0;

    for (int i = 1; i < pattern.length(); i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        next[i] = j;
    }
    return next;
}

int KMPSearch(const string& text, const string& pattern) {
    vector<int> next = getNext(pattern);
    int j = 0;

    for (int i = 0; i < text.length(); i++) {
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (text[i] == pattern[j]) {
            j++;
        }
        if (j == pattern.length()) {
            return i - j + 1;  // 找到匹配
        }
    }
    return -1;  // 未找到
}
```

### 15.4 字符串哈希技巧

```cpp
// Rabin-Karp 滚动哈希
class RollingHash {
private:
    static const int BASE = 31;
    static const int MOD = 1e9 + 7;

public:
    long long computeHash(const string& s) {
        long long hash = 0;
        long long pow = 1;

        for (char c : s) {
            hash = (hash + (c - 'a' + 1) * pow) % MOD;
            pow = (pow * BASE) % MOD;
        }
        return hash;
    }

    // 查找所有长度为len的重复子串
    vector<string> findRepeatedSubstrings(const string& s, int len) {
        unordered_map<long long, vector<int>> hashMap;
        long long hash = 0, pow = 1;

        // 计算第一个窗口的哈希值
        for (int i = 0; i < len; i++) {
            hash = (hash + (s[i] - 'a' + 1) * pow) % MOD;
            if (i < len - 1) pow = (pow * BASE) % MOD;
        }
        hashMap[hash].push_back(0);

        // 滑动窗口
        for (int i = len; i < s.length(); i++) {
            // 移除最左边的字符
            hash = (hash - (s[i - len] - 'a' + 1) + MOD) % MOD;
            hash = (hash * pow % MOD + MOD) % MOD;

            // 添加新字符
            hash = (hash + (s[i] - 'a' + 1)) % MOD;

            hashMap[hash].push_back(i - len + 1);
        }

        vector<string> result;
        for (auto& [h, positions] : hashMap) {
            if (positions.size() > 1) {
                result.push_back(s.substr(positions[0], len));
            }
        }
        return result;
    }
};
```

## 16. 性能优化和最佳实践

### 16.1 性能建议

1. **预分配容量**：使用 `reserve()` 避免重复分配
2. **避免频繁拼接**：大量拼接用 `stringstream`
3. **传参使用引用**：`const string&` 避免拷贝
4. **移动语义**：C++11 后适当使用 `std::move`
5. **字符串视图**：C++17 `string_view` 避免拷贝

```cpp
#include <string_view>  // C++17

// 高效的字符串处理函数
void processString(string_view sv) {  // 不拷贝原字符串
    // 处理逻辑...
}

// 字符串构建优化
string buildString(const vector<string>& parts) {
    size_t total_size = 0;
    for (const auto& part : parts) {
        total_size += part.size();
    }

    string result;
    result.reserve(total_size);  // 预分配

    for (const auto& part : parts) {
        result += part;
    }
    return result;
}
```

### 16.2 内存和异常安全

```cpp
// RAII 和异常安全
class StringBuffer {
private:
    string buffer;

public:
    void append(const string& str) {
        buffer += str;  // 异常安全的追加
    }

    string release() && {
        return move(buffer);  // 移动语义
    }
};
```

## 17. 总结

这个 cheat sheet 涵盖了：

1. **基础操作** - 创建、访问、修改字符串
2. **STL 算法** - sort, find, transform 等核心算法
3. **容器配合** - 与 vector, map, set 等的结合使用
4. **实用函数** - 分割、连接、去空格等工具函数
5. **算法技巧** - 双指针、滑动窗口、字符串匹配
6. **性能优化** - 内存管理和最佳实践

### 常用算法速查：

- **排序**: `sort()`, `partial_sort()`, `stable_sort()`
- **查找**: `find()`, `binary_search()`, `lower_bound()`
- **变换**: `transform()`, `replace()`, `reverse()`
- **统计**: `count()`, `accumulate()`, `max_element()`
- **去重**: `unique()`, `remove()`, `erase()`
- **分区**: `partition()`, `stable_partition()`

### 字符串专用：

- **子串**: `substr()`, `find()`, `rfind()`
- **转换**: `stoi()`, `to_string()`, `toupper()`
- **检查**: `empty()`, `size()`, `compare()`

记住这些内容，你就能在算法题和实际开发中游刃有余地处理字符串了！
