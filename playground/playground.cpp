/*
g++ -std=c++17 -Wall -Iinclude -I/opt/homebrew/include playground.cpp
-L/opt/homebrew/lib -lgtest -lgtest_main -pthread -o playground_test
*/

#include <gtest/gtest.h>

#include <iostream>
#include <numeric>  // 用于 iota
#include <vector>

using namespace std;

TEST(Playground, IotaExample)
{
    std::cout << "sizeof(int): " << sizeof(int) << " bytes" << std::endl;
    std::cout << "sizeof(long): " << sizeof(long) << " bytes" << std::endl;
    std::cout << "sizeof(long long): " << sizeof(long long) << " bytes"
              << std::endl;
    std::cout << "sizeof(double): " << sizeof(double) << " bytes" << std::endl;

    std::cout << "INT_MAX: " << INT_MAX << std::endl;
    std::cout << "LONG_MAX: " << LONG_MAX << std::endl;
}