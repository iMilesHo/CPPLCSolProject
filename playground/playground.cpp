/*
g++ -std=c++17 -Wall -Iinclude -I/opt/homebrew/include playground.cpp
-L/opt/homebrew/lib -lgtest -lgtest_main -pthread -o playground_test
*/

#include <gtest/gtest.h>

#include <iostream>
#include <numeric>  // 用于 iota
#include <vector>

using namespace std;

template <typename T>
class CircleArray
{
  private:
    vector<int>
};

TEST(Playground, CirclArray) {}