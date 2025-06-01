# Minimum Window Substring

- **ID:** 76
- **Difficulty:** HARD
- **Topic Tags:** Hash Table, String, Sliding Window
- **Link:** [LeetCode Problem](https://leetcode.com/problems/minimum-window-substring/description/)

## Problem Description

Given two strings s and t of lengths m and n respectively, return the minimum window substring of s such that every character in t (including duplicates) is included in the window. If there is no such substring, return the empty string "".

The testcases will be generated such that the answer is unique.

Example 1:

Input: s = "ADOBECODEBANC", t = "ABC"
Output: "BANC"
Explanation: The minimum window substring "BANC" includes 'A', 'B', and 'C' from string t.
Example 2:

Input: s = "a", t = "a"
Output: "a"
Explanation: The entire string s is the minimum window.
Example 3:

Input: s = "a", t = "aa"
Output: ""
Explanation: Both 'a's from t must be included in the window.
Since the largest window of s only has one 'a', return empty string.

Constraints:

m == s.length
n == t.length
1 <= m, n <= 105
s and t consist of uppercase and lowercase English letters.

```cpp
/*
given:
- string s -> search space (if brute-force, all substrings: m^2*m*n)
- string t -> characters we need to search

objective:
- find the smallest substring in the search space
    - all characters should have in the substring including duplicates
- the order doesn't matter

example:
s = "AAAADOBECODEBANC"
                    ^
                    ^
t = "ABC" {"A": 1, "B": 1, "C": 1}
{"A": 0, "B": 0, "C": 0} remainningRequired = 0 -> found one, ADOBEC

{"A": 0, "B": 1, "C": 0} remainningRequired = 0 CODEBAN

time complexity:
O(m+n) -> m is the length of s, n is the length of t

space complexity:
O(n)
*/

class Solution
{
public:
    void updateBestWindow(int left, int right, int &bestStart, int &bestLen)
    {
        if (static_cast<int>(right - left + 1) < bestLen)
        {
            bestLen = static_cast<int>(right - left + 1);
            bestStart = left;
        }
    }

    string minWindow(string s, string t)
    {
        // edge cases
        if (s.length() < t.length() || t.empty())
            return "";

        size_t left = 0;
        size_t right = 0;
        int bestLen = INT_MAX;
        int bestStart = 0;

        // remainningRequired number of characters we need to find in the search space
        int remainningRequired = t.length();
        // count how many times each character in t mush appear in the window
        std::unordered_map<char, int> needCount;
        for (int i = 0; i < t.length(); i++)
        {
            ++needCount[t[i]];
        }

        /*
        int needCount[128] = {0};
        for (char c: t)
        {
            needCount[static_cast<int>(c)]++;
        }
        //or
        std::vector<int> freq(52, 0);

        */

        while (right < s.length())
        {
            // early continue
            if (needCount.find(s[right]) == needCount.end())
            {
                // Character not required: just expand right
                right++;
                continue;
            }
            // Otherwise, character is in the target; process it
            if (needCount[s[right]] > 0)
            {
                remainningRequired--;
            }
            needCount[s[right]]--;

            if (remainningRequired == 0)
            {
                updateBestWindow(left, right, bestStart, bestLen);

                // shrink-loop
                while (left <= right)
                {
                    auto it = needCount.find(s[left]);
                    if (it == needCount.end())
                    {
                        // not present in the target string, so we skip
                        left++;
                        updateBestWindow(left, right, bestStart, bestLen);
                    }
                    else
                    {
                        int &count = it->second;
                        if (count == 0)
                        {
                            // loosing a required char
                            count++;
                            remainningRequired++;
                            left++;
                            right++;
                            break;
                        }
                        else if (count < 0)
                        {
                            // surplus char; still valid window if we remove it
                            count++;
                            left++;
                            updateBestWindow(left, right, bestStart, bestLen);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                right++;
            }
        }

        if (bestLen < INT_MAX)
        {
            return s.substr(bestStart, bestLen);
        }
        else
        {
            return "";
        }
    }
};
```

## Standard Solution

```cpp
#include <string>
#include <unordered_map>
#include <climits>
using namespace std;

class Solution {
public:
    string minWindow(string s, string t) {
        if (s.empty() || t.empty()) return "";

        unordered_map<char, int> need, window;
        for (char c : t) need[c]++;

        int left = 0, right = 0;
        int valid = 0; // number of chars meeting required frequency
        int minLen = INT_MAX, start = 0;

        while (right < s.size()) {
            char c = s[right];
            right++;

            // If this character is needed, update window map
            if (need.count(c)) {
                window[c]++;
                if (window[c] == need[c])
                    valid++;
            }

            // When all requirements are met, try to shrink
            while (valid == need.size()) {
                // Update answer if better
                if (right - left < minLen) {
                    minLen = right - left;
                    start = left;
                }

                // Remove leftmost character
                char d = s[left];
                left++;
                if (need.count(d)) {
                    if (window[d] == need[d])
                        valid--;
                    window[d]--;
                }
            }
        }

        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};
```
