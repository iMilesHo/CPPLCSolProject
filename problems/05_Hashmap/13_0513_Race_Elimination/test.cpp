#include <gtest/gtest.h>

#include "solution.hpp"

TEST(RaceElimination, Example1)
{
    Solution solution;
    vector<vector<string>> input{
        {"Gina 155", "Eddie 160", "Joy 161", "Harold 163"},
        {"Harold 151", "Gina 153", "Joy 160", "Eddie 160"},
        {"Harold 149", "Joy 150", "Gina 152", "Eddie 154"},
        {"Harold 148", "Gina 150", "Eddie 151", "Joy 155"}};
    vector<string> expected{"Harold", "Eddie", "Joy", "Gina"};
    EXPECT_EQ(solution.eliminateRacers(input), expected);
}


TEST(RaceElimination, Example2)
{
    // test emplace and []
    // unordered_map<string, RacerInfo> racers;
    // racers.emplace("Alice", RacerInfo("Alice", 120));
    // racers.emplace(
    //     "Alice",
    //     RacerInfo("Alice", 130));  // This won't overwrite the existing entry
    // // print the bestTime of Alice
    // std::cout << "Alice's best time: " << racers["Alice"].bestTime
    //           << std::endl;  // Should print 120
    // EXPECT_EQ(racers["Alice"].bestTime, 120);
}