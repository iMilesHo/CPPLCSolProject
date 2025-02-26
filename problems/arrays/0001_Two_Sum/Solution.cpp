// problems/arrays/0001_Two_Sum/Solution.cpp
#include "Solution.h"
#include <unordered_map>

std::vector<int> Solution::twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> value_to_index;
    for (int i = 0; i < nums.size(); i++) {
        int compensate = target - nums[i];
        if (value_to_index.find(compensate) != value_to_index.end()) {
            return {value_to_index[compensate], i};
        }
        value_to_index[nums[i]] = i;
    }
    return {};
}