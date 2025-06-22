# Two Sum

- **ID:** 1
- **Difficulty:** EASY
- **Topic Tags:** Array, Hash Table
- **Link:** [LeetCode Problem](https://leetcode.com/problems/two-sum/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

(TODO: Outline your approach here)

## Complexity

- **Time Complexity:** (TODO)
- **Space Complexity:** (TODO)

## variant problem

Merging 2 Packages

Given a package with a weight limit limit and an array arr of item weights, implement a function getIndicesOfItemWeights that finds two items whose sum of weights equals the weight limit limit. Your function should return a pair [i, j] of the indices of the item weights, ordered such that i > j. If such a pair doesn't exist, return an empty array.

Analyze the time and space complexities of your solution.

Example:

input: arr = [4, 6, 10, 15, 16], lim = 21

output: [3, 1] # since these are the indices of the # weights 6 and 15 whose sum equals to 21
Constraints:

[time limit] 5000ms

[input] array.integer arr

0 ≤ arr.length ≤ 100
[input] integer limit

[output] array.integer

```cpp
#include <iostream>
#include <vector>

using namespace std;

vector<int> getIndicesOfItemWeights( const vector<int>& arr, int limit)
{
  // your code goes here
}

int main() {
  return 0;
}
```
