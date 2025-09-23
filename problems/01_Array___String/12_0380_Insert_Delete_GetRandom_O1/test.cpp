#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;


class RandomizedSet {
public:
    vector<int> values;
    unordered_map<int, int> valueIndexPair;
    mt19937 gen;

    
    RandomizedSet():gen(random_device{}()) {}
    
    bool insert(int val) {
        if(valueIndexPair.count(val)) return false;

        values.push_back(val);
        valueIndexPair[val] = values.size() - 1;
        return true;
    }
    
    bool remove(int val) {
        if (!valueIndexPair.count(val)) return false;
        int index = valueIndexPair[val];
        int lastValue = values[values.size() - 1];
        values[index] = lastValue;
        values.pop_back();
        valueIndexPair[lastValue] = index;
        return true;
    }
    
    int getRandom() {
        uniform_int_distribution<int> distr(0, values.size()-1);
        return distr(gen);
    }
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */


TEST(Problem380Test, BasicTest) {
    // Solution sol;
    std::mt19937 gen(12345);
    std::uniform_int_distribution<int> dist(1, 10);

    for(int i = 0; i < 1; ++i) {
        std::cout << gen() << " "; // () 调用 operator() 生成下一个随机数
    }
    std::cout << std::endl;
}
