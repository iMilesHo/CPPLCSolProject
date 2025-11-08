#!/bin/bash

# 车载网络调度中枢 - TimerManager 编译脚本
# 作者: 李乐贺

set -e  # 遇到错误立即退出

echo "======================================"
echo "  TimerManager Demo - Build Script"
echo "======================================"
echo ""

# 检查编译器
if ! command -v g++ &> /dev/null; then
    echo "❌ Error: g++ compiler not found"
    echo "Please install C++ compiler first"
    exit 1
fi

echo "✓ Found g++ compiler: $(g++ --version | head -n 1)"
echo ""

# 编译选项
CXX_FLAGS="-std=c++14 -pthread -Wall -Wextra -O2"

# 编译优化前版本
echo "🔨 [1/3] Compiling TimerManager (Before Optimization - Polling)..."
g++ $CXX_FLAGS -o demo_timermanager_before \
    demo_timermanager.cpp \
    TimerManager_Before.cpp

if [ $? -eq 0 ]; then
    echo "   ✅ demo_timermanager_before built successfully"
else
    echo "   ❌ Build failed!"
    exit 1
fi

echo ""

# 编译优化后版本
echo "🔨 [2/3] Compiling TimerManager (After Optimization - CondVar)..."
g++ $CXX_FLAGS -o demo_timermanager_after \
    demo_timermanager_after.cpp \
    TimerManager_After.cpp

if [ $? -eq 0 ]; then
    echo "   ✅ demo_timermanager_after built successfully"
else
    echo "   ❌ Build failed!"
    exit 1
fi

echo ""

# 编译对比版本
echo "🔨 [3/3] Compiling Performance Comparison Demo..."
g++ $CXX_FLAGS -o demo_comparison \
    demo_comparison.cpp \
    TimerManager_Before.cpp \
    TimerManager_After.cpp

if [ $? -eq 0 ]; then
    echo "   ✅ demo_comparison built successfully"
else
    echo "   ❌ Build failed!"
    exit 1
fi

echo ""
echo "======================================"
echo "  Build Complete! 🎉"
echo "======================================"
echo ""
echo "Available demos:"
echo "  1. ./demo_timermanager_before  - 优化前版本 (轮询机制)"
echo "  2. ./demo_timermanager_after   - 优化后版本 (条件变量)"
echo "  3. ./demo_comparison           - 性能对比测试"
echo ""
echo "推荐先运行: ./demo_comparison"
echo ""
