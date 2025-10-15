#ifndef RACE_ELIMINATION_HPP
#define RACE_ELIMINATION_HPP

#include <iostream>

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
    pair<string, int> parseRecord(const string& record)
    {
        auto spaceIndex = record.find(" ");
        if (string::npos == spaceIndex)
        {
            throw invalid_argument("Invalid record format:" + record);
        }
        string name = record.substr(0, spaceIndex);

        try
        {
            int timeUsed = stoi(record.substr(spaceIndex + 1));
            if (timeUsed < 0)
            {
                throw invalid_argument("Time must be positive: " + record);
            }

            return {name, timeUsed};
        }
        catch (const exception& e)
        {
            throw invalid_argument("Invalida time format:" + record);
        }
    }

  public:
    vector<string> eliminateRacers(const vector<vector<string>>& laps)
    {
        if (laps.empty())
        {
            return {};
        }
        unordered_map<string, RacerInfo> nameTimePair{};

        vector<string> ans;
        for (auto& lap : laps)
        {
            int MaxTimeUsed = INT_MIN;
            for (auto& record : lap)
            {
                auto [name, timeUsed] = parseRecord(record);

                auto itr = nameTimePair.find(name);
                if (itr == nameTimePair.end())
                {
                    nameTimePair.emplace(name, RacerInfo(name, timeUsed));
                    MaxTimeUsed = max(timeUsed, MaxTimeUsed);
                }
                else if (!itr->second.eliminated)
                {
                    itr->second.bestTime = min(itr->second.bestTime, timeUsed);
                    MaxTimeUsed = max(itr->second.bestTime, MaxTimeUsed);
                }
            }

            // collection racers to be eliminate
            vector<string> readyToEliminate;
            for (auto& [name, racer] : nameTimePair)
            {
                if (!racer.eliminated && racer.bestTime >= MaxTimeUsed)
                {
                    readyToEliminate.push_back(name);
                    racer.eliminated = true;
                }
                cout << racer.name << " " << racer.bestTime << " "
                     << racer.eliminated << endl;
            }
            cout << "MaxTimeUsed: " << MaxTimeUsed << endl;

            // sort by name alphabatic order
            sort(readyToEliminate.begin(), readyToEliminate.end());

            ans.insert(ans.end(),
                       readyToEliminate.begin(),
                       readyToEliminate.end());
        }
        return ans;
    }
};


#endif  // RACE_ELIMINATION_HPP