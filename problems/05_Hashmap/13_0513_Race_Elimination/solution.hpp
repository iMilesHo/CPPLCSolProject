#ifndef RACE_ELIMINATION_HPP
#define RACE_ELIMINATION_HPP

#include <algorithm>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct RacerInfo
{
    string name;
    int bestTime;
    bool eliminated;

    RacerInfo(const string& n, int t) : name(n), bestTime(t), eliminated(false)
    {
    }
};

class Solution
{
  private:
    // 解析单个比赛记录
    pair<string, int> parseRecord(const string& record)
    {
        auto spaceIndex = record.find(" ");
        if (spaceIndex == string::npos)
        {
            throw invalid_argument("Invalid record format: " + record);
        }

        string name = record.substr(0, spaceIndex);
        try
        {
            int time = stoi(record.substr(spaceIndex + 1));
            if (time <= 0)
            {
                throw invalid_argument("Time must be positive: " + record);
            }
            return {name, time};
        }
        catch (const exception& e)
        {
            throw invalid_argument("Invalid time format: " + record);
        }
    }

  public:
    vector<string> eliminateRacers(const vector<vector<string>>& laps)
    {
        if (laps.empty() || laps[0].empty())
        {
            return {};
        }

        // 初始化选手信息
        unordered_map<string, RacerInfo> racers;
        for (const auto& record : laps[0])
        {
            auto [name, time] = parseRecord(record);
            racers.emplace(name, RacerInfo(name, time));
        }

        vector<string> result;

        // 处理每一轮比赛
        for (const auto& lap : laps)
        {
            // 更新最佳成绩
            int maxBestTime = INT_MIN;
            for (const auto& record : lap)
            {
                auto [name, time] = parseRecord(record);
                auto it = racers.find(name);
                if (it == racers.end() || it->second.eliminated)
                {
                    continue;
                }

                it->second.bestTime = min(it->second.bestTime, time);
                maxBestTime = max(maxBestTime, it->second.bestTime);
            }

            // 收集本轮需要淘汰的选手
            vector<string> toEliminate;
            for (const auto& [name, info] : racers)
            {
                if (!info.eliminated && info.bestTime == maxBestTime)
                {
                    toEliminate.push_back(name);
                }
            }

            // 按字母顺序淘汰选手
            sort(toEliminate.begin(), toEliminate.end());
            for (const auto& name : toEliminate)
            {
                auto it = racers.find(name);
                if (it != racers.end())
                {
                    it->second.eliminated = true;
                }
                result.push_back(name);
            }
        }

        return result;
    }
};

#endif