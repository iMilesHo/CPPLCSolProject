#ifndef SOLUTION_0042_H
#define SOLUTION_0042_H

#include <vector>
using namespace std;

class Solution { public: int trap(vector<int>& height) { // edge cases 
    int n = height.size(); // 
    int leftMin = 0, rightMax = 0; 
    for (int i = 0; i < n; i++) { int cI = i; 
        // center index 
        // search left 
        int lI = cI; 
        while(lI >= 0) 
        { 
            if (height[lI-1] <= height[lI] && height[lI] >= ) // search right 
        } 
    } 
};

#endif
