#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target);
    std::vector<int> twoSumSpaceSaving(std::vector<int>& nums, int target);
    int removeDuplicates(std::vector<int>& nums);
};

#endif