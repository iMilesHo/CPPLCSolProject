/*
g++ -std=c++17 -Wall -Iinclude -I/opt/homebrew/include playground.cpp
-L/opt/homebrew/lib -lgtest -lgtest_main -pthread -o playground_test
*/

#include <gtest/gtest.h>

#include <iostream>
#include <numeric>  // 用于 iota
#include <vector>

using namespace std;

/*
laps:
[["Gina 155","Eddie 160","Joy 161","Harold 163"],
 ["Harold 151","Gina 153","Joy 160","Eddie 160"],
 ["Harold 149","Joy 150","Gina 152","Eddie 154"],
 ["Harold 148","Gina 150","Eddie 151","Joy 155"]]
*/
vector<string> solution(vector<vector<string>> laps)
{
    unordered_map<string, int> nameBestTimePairs{};
    vector<string> ans{};

    for (auto lap : laps)
    {
        int slowest = -1;
        vector<string> eliminateNames{};
        for (auto& str : lap)
        {
            auto spaceIndex = str.find(" ");
            string name = str.substr(0, spaceIndex);
            if (nameBestTimePairs[name] < 0)
            {
                continue;
            }

            int timeI = stoi(str.substr(spaceIndex + 1, str.length()));
            if (nameBestTimePairs[name] != 0)
            {
                nameBestTimePairs[name] = min(nameBestTimePairs[name], timeI);
            }
            else
            {
                nameBestTimePairs[name] = timeI;
            }

            if (slowest < nameBestTimePairs[name])
            {
                eliminateNames.clear();
                eliminateNames.push_back(name);
                slowest = nameBestTimePairs[name];
            }
            else if (slowest == nameBestTimePairs[name])
            {
                eliminateNames.push_back(name);
            }
        }

        sort(eliminateNames.begin(), eliminateNames.end());
        for (auto& name : eliminateNames)
        {
            ans.push_back(name);
            nameBestTimePairs[name] = -nameBestTimePairs[name];
        }
        for (auto& [name, besttime] : nameBestTimePairs)
        {
            cout << name << ": " << besttime;
        }
        cout << endl;
    }

    return ans;
}


TEST(Playground, CirclArray)
{
    string str = "Hello World, Hello World";

    str.replace(6, 5, "C++");  // 将位置6开始的5个字符替换为"C++"
    cout << str << endl;       // 输出 "Hello C++"
    str.replace(str.begin(), str.begin() + 5, "Hi");  // 使用迭代器替换
    cout << str << endl;                              // 输出 "Hi C++"

    // compare strings
    string str1 = "apple";
    string str2 = "apple";
    if (str1 < str2)
    {
        cout << str1 << " is less than " << str2 << endl;
    }
    else if (str1 > str2)
    {
        cout << str1 << " is greater than " << str2 << endl;
    }
    else
    {
        cout << str1 << " is equal to " << str2 << endl;
    }
}