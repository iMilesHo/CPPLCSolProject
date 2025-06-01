#ifndef SOLUTION_0076_H
#define SOLUTION_0076_H

#include <string>
#include <unordered_map>
#include <climits>
using namespace std;

class Solution
{
public:
    string minWindow(string s, string t)
    {
        if (s.empty() || t.empty())
            return "";

        unordered_map<char, int> need, window;
        for (char c : t)
            need[c]++;

        int left = 0, right = 0;
        int valid = 0; // number of distinct characters
        int minLen = INT_MAX, start = 0;

        while (right < s.size())
        {
            char c = s[right];
            right++;

            // If this character is needed, update window map
            if (need.count(c))
            {
                window[c]++;
                if (window[c] == need[c])
                    valid++;
            }

            // When all requirements are met, try to shrink
            while (valid == need.size())
            {
                // Update answer if better
                if (right - left < minLen)
                {
                    minLen = right - left;
                    start = left;
                }

                // Remove leftmost character
                char d = s[left];
                left++;
                if (need.count(d))
                {
                    if (window[d] == need[d])
                        valid--;
                    window[d]--;
                }
            }
        }

        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};

#endif
