#include "Solution.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <sys/resource.h>
#include <sys/time.h>
#include <chrono>

/**
 * @brief Given an array of integers `nums` and an integer `target`, 
 *        return indices of the two numbers such that they add up to `target`.
 * 
 * If no two such indices exist, returns {-1, -1}.
 *
 * Time Complexity: O(n), where n is the length of nums.
 * Space Complexity: O(n).
 */
std::vector<int> Solution::twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> value_to_index;
    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];
        // Check if the complement exists in the map
        if (value_to_index.find(complement) != value_to_index.end()) {
            return {value_to_index[complement], i};
        }
        // Record the index of the current value
        value_to_index[nums[i]] = i;
    }
    // If no pair found
    return {-1, -1};
}