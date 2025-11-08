/*
g++ -std=c++17 -Wall -Iinclude -I/opt/homebrew/include playground.cpp
-L/opt/homebrew/lib -lgtest -lgtest_main -pthread -o playground_test
*/

#include <gtest/gtest.h>

#include <climits>   // 用于 INT_MAX, LONG_MAX
#include <iostream>  // 用于 std::cout
#include <numeric>   // 用于 iota
#include <unordered_map>
#include <vector>

using namespace std;

// build Binayy Tree from vector
struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

TreeNode* buildTree(const vector<int>& nodes, int index)
{
    if (index >= nodes.size() || nodes[index] == INT_MIN)
    {
        return nullptr;
    }
    TreeNode* root = new TreeNode(nodes[index]);
    root->left = buildTree(nodes, 2 * index + 1);
    root->right = buildTree(nodes, 2 * index + 2);
    return root;
}

// Function to print the tree in-order (for testing purposes)
void printInOrder(TreeNode* root)
{
    if (root == nullptr)
    {
        return;
    }
    printInOrder(root->left);
    cout << root->val << " ";
    printInOrder(root->right);
}

// Test case for building and printing the tree
TEST(BuildTreeTest, BasicTest)
{
    // vector<int> nodes = {[2,null,3,null,4,null,5,null,6]};
    // [2,null,3,null,4,null,5,null,6]
    vector<int> nodes = {2, INT_MIN, 3, INT_MIN, 4, INT_MIN, 5, INT_MIN, 6};
    TreeNode* root = buildTree(nodes, 0);
    cout << "In-order traversal of the tree: ";
    printInOrder(root);
    cout << endl;
}