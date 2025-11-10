/*
g++ -std=c++17 -Wall -Iinclude -I/opt/homebrew/include playground.cpp
-L/opt/homebrew/lib -lgtest -lgtest_main -pthread -o playground_test
*/

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

using namespace std;

TEST(PlaygroundTest, BasicAssertions)
{
    // Expect two strings to be equal.
    EXPECT_EQ(1 + 1, 2);
    EXPECT_NE(1 + 1, 3);
}
