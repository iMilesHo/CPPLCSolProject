#ifndef SOLUTION_0027_H
#define SOLUTION_0027_H

#include <vector>
#include <algorithm>

using namespace std;

/*
edge cases
[], 3

[2], 3

[3], 3

[2,2,2] 3


*/
class Solution
{
public:
    int removeElement(vector<int> &nums, int val)
    {
        int k = 0;
        for (int x : nums)
        {
            if (x != val)
                nums[k++] = x;
        }
        return k;
    }
};

#endif
