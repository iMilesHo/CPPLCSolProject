#!/bin/bash

# 编译单例模式示例
echo "正在编译单例模式示例..."

# 检查编译器
if command -v g++ &> /dev/null; then
    COMPILER="g++"
elif command -v clang++ &> /dev/null; then
    COMPILER="clang++"
else
    echo "错误: 未找到 g++ 或 clang++ 编译器"
    exit 1
fi

echo "使用编译器: $COMPILER"

# 编译命令
$COMPILER -std=c++11 -pthread -o singleton_demo singleton_example.cpp

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功！"
    echo "运行程序："
    echo "=========================================="
    ./singleton_demo
    echo "=========================================="
    echo "程序运行完成"
else
    echo "编译失败！"
    exit 1
fi