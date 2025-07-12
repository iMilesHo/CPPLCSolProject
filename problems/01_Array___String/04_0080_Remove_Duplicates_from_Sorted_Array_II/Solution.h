#ifndef SOLUTION_0080_H
#define SOLUTION_0080_H

#include <vector>

using namespace std;

/*
1, 1, 2, 2, 3, 3, 2, 2, 2, 3, 3
               ^
                              ^
*/
class Solution
{
public:
    int removeDuplicates(vector<int> &nums)
    {
        size_t write = 0;
        size_t count = 1;
        for (size_t read = 0; read < nums.size(); read++)
        {
            if (write == read)
            {
                continue;
            }

            if (nums[write] == nums[read] && count == 1)
            {
                count++;
                write++;
                nums[write] = nums[read];
            }
            else if (nums[write] != nums[read])
            {
                nums[++write] = nums[read];
                count = 1;
            }
        }

        return static_cast<int>(write) + 1;
    }
};

#endif
