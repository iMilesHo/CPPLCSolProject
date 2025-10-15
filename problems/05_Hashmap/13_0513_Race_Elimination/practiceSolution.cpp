#ifndef RACE_ELIMINATION_HPP
#define RACE_ELIMINATION_HPP

#include <iostream>

using namespace std;


class Solution
{
  private:
  public:
    vector<string> eliminateRacers(const vector<vector<string>>& laps)
    {
        // we need to use unorder_map to store everyone's historical
        // best time (fastest time)
        for (auto& lap : laps)
        {
            for (auto& record : lap)
            {
            }
        }
    }
};


#endif  // RACE_ELIMINATION_HPP