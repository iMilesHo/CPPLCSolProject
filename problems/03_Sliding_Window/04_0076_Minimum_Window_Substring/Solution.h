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
            if (need.count(c))
            {
                window[c]++;
                if (need[c] == window[c])
                    valid++;
            }

            // shrink the window from left bound if found a valid substring
            while (valid == need.size())
            {
                int tempLen = right - left + 1;
                if (tempLen < minLen)
                {
                    minLen = tempLen;
                    start = left;
                }

                if (need.count(s[left]))
                {
                    if (window[s[left]] == need[s[left]])
                    {
                        valid--;
                    }
                    window[s[left]]--;
                }

                left++;
            }

            right++;
        }

        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};

#endif
