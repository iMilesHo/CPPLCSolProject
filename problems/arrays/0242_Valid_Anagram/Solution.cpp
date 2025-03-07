#include "Solution.h"
#include <unordered_map>

// method 1: using hash map
bool Solution::isAnagram(std::string s, std::string t) {
    // edge case
    if (s.size() != t.size()) {
        return false;
    }

    // declare a hash map (don't need to declare two hash maps)
    std::unordered_map<char, int> hash_map;

    // fill the hash map with the first string
    for (int i = 0; i < s.size(); i++) {
        hash_map[s[i]] += 1;
    }

    // check the second string
    for (int i = 0; i < t.size(); i++) {
        if (hash_map.find(t[i]) == hash_map.end()) {
            return false;
        } else {
            hash_map[t[i]] -= 1;
            if (hash_map[t[i]] < 0) {
                return false;
            }
        }
    }

    return true;
}

// method 2: using sort