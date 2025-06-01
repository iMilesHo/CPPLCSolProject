#ifndef SOLUTION_0088_H
#define SOLUTION_0088_H

#include <vector>

/*
We merge from the back to front and we never overwrite the unread data

[6,5,4,0,0,0]
[1,2,3]
---
[1,2,3,4,5,6]
     ^
     ^
[1,2,3]
     ^
so the invariant statement is that:
After each iteration, the element at the right is sorted (question?)

Time complexity O(m+n)
Space complexity O(1)
*/

class Solution
{
public:
     void merge(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
     {
     }
};

#endif
