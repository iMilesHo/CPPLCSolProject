#include "Solution.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <sys/resource.h>
#include <sys/time.h>
#include <chrono>




/**
 * @brief Finds two indices in `nums` whose values sum up to `target`
 * 
 * Constraints:
 * must has only one solution
 * 
 * Method 1: hash map
 * Time complexity: ideally O(1)
 * Space complexity: O(n) but may not continous
 * 
 * Method 2: vector of value and index pairs, sort the vector based on the value
 * Time complexity: O(nlog(n))
 * Space complexity: O(n)
 */
std::vector<int> Solution::twoSum(std::vector<int>& nums, int target)
{
    std::unordered_map<int, int> value_to_index;
    std::vector<int> res {-1,-1};

    for (int i=0; i < nums.size(); i++)
    {
        int compensate = target - nums[i];
        if (value_to_index.find(compensate) != value_to_index.end())
        {
            return {value_to_index[compensate], i};
        }
        else
        {
            value_to_index[nums[i]] = i;
        }
    }

    return res;
}

std::vector<int> Solution::twoSumSpaceSaving(std::vector<int>& nums, int target)
{
    std::vector res {-1, -1};

    // in order to save the space
    // use vector of pairs instead of hash map
    std::vector<std::pair<int, int>> ValueIndexPairsVector;
    for (int i = 0; i < (int)nums.size(); i++)
    {
        ValueIndexPairsVector.push_back({nums[i], i});
    }

    std::sort(ValueIndexPairsVector.begin(), 
              ValueIndexPairsVector.end(),
              [](auto &a, auto &b){
                return a.first < b.first;
              });

    int left = 0;
    int right = (int)nums.size() - 1;
    
    while (left < right)
    {
        int tempSum = ValueIndexPairsVector[left].first + ValueIndexPairsVector[right].first;
        if (target == tempSum)
        {
            return {ValueIndexPairsVector[left].second, ValueIndexPairsVector[right].second};
        }
        else if (target > tempSum)
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return res;
}

/*
[0,0,1,1,1,2,2,3,3,4]
0,1,2,3,4
*/
int Solution::removeDuplicates(std::vector<int>& nums)
{
    int duplcateNum = 0;
    int curruentIndex = 0;
    for (int i = 1; i < nums.size(); i++)
    {
        if (nums[i-1] == nums[i])
        {
            duplcateNum++;
        }
        else
        {
            curruentIndex++;
            nums[curruentIndex] = nums[i];
        }
    }
    
    return static_cast<int>(nums.size()) - duplcateNum;
}
