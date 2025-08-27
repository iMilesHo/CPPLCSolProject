#ifndef SOLUTION_0169_H
#define SOLUTION_0169_H

#include <vector>
#include <unordered_map>
#include <map>


using namespace std;

class Solution {
public:
        int majorityElement(vector<int>& nums) {
        // map
        int halfLength = nums.size() / 2;
        map<int, int> elementCount{};
        for(auto i: nums)
        {
            elementCount[i] += 1;
        }
        for(auto pair: elementCount)
        {
            if(pair.second > halfLength)
            {
                return pair.first;
            }
        }
        return 0;
    }
};

#endif
