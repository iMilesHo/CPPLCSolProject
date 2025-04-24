#include "Solution.h"

/*
[1,2,3,0,0,0]
   ^
[-1,5,6]
  ^

[1,2,3,3,5,6]
     ^
*/

// -109 <= nums1[i], nums2[j] <= 109
// and the scope that int can represent is [?]

// TODO: Implement your solution methods here
void Solution::merge(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
{
    int length = m + n;
    int index1 = m - 1;
    int index2 = n - 1;
    for (int i = length - 1; i >= 0; i--)
    {
        if (index2 >= 0 && index1 >= 0)
        {
            if (nums1[index1] <= nums2[index2])
            {
                nums1[i] = nums2[index2];
                index2--;
            }
            else
            {
                nums1[i] = nums1[index1];
                index1--;
            }
        }
        else if (index1 < 0 && index2 >= 0)
        {
            nums1[i] = nums2[index2];
            index2--;
        }
        else if (index2 < 0 && index1 >= 0)
        {
            nums1[i] = nums1[index1];
            index1--;
        }
        else
        {
            break;
        }
    }
}
