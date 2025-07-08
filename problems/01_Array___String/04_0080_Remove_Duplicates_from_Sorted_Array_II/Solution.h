#ifndef SOLUTION_0080_H
#define SOLUTION_0080_H

#include <vector>

using namespace std;

/*
Input: nums = [1,1,1,2,2,3]
1,1,1,2,2,3
  ^
  ^
count  = 1
Output: 5, nums = [1,1,2,2,3,_]

Input: nums = [0,0,1,1,1,1,2,3,3]
Output: 7, nums = [0,0,1,1,2,3,3,_,_]
*/
class Solution
{
public:
    int removeDuplicates(vector<int> &nums)
    {
        size_t write = 0;
        size_t read = 0;
        size_t count = 0;
        while (read < nums.size())
        {
            if (nums[write] == nums[read] && count == 0) // overlap
            {
                write++;
                count++;
            }
            else if (nums[write] == nums[read] && count == 1)
            {
                count++;
            }
            else if (nums[write] == nums[read] && count == 2)
            {
                count++;
            }
            else if (nums[write] != nums[read])
            {
                nums[++write] = nums[read];
                count = 1;
            }
            read++;
        }

        return static_cast<int>(write) + 1;
    }
};

#endif
