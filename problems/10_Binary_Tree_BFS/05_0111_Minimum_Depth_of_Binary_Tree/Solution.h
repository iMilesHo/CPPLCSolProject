#ifndef SOLUTION_0111_H
#define SOLUTION_0111_H

#include <algorithm>
#include <queue>
using namespace std;

// Definition for a binary tree node.
struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right)
        : val(x), left(left), right(right)
    {
    }
};

class Solution
{
  public:
    // DFS Solution (from your implementation)
    int minimumDepthDFS(TreeNode* node)
    {
        if (!node)
        {
            return 0;
        }

        int leftDepth = minimumDepthDFS(node->left);
        int rightDepth = minimumDepthDFS(node->right);

        // Key insight: if one subtree is null, we must go down the other path
        if ((leftDepth == 0 && rightDepth != 0) ||
            (leftDepth != 0 && rightDepth == 0))
        {
            return 1 + max(leftDepth, rightDepth);
        }

        return 1 + min(leftDepth, rightDepth);
    }

    // BFS Solution (optimal for minimum depth)
    int minDepth(TreeNode* root)
    {
        if (!root) return 0;

        queue<TreeNode*> q;
        q.push(root);
        int depth = 1;

        while (!q.empty())
        {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode* node = q.front();
                q.pop();

                // Found the first leaf node - this is minimum depth
                if (!node->left && !node->right)
                {
                    return depth;
                }

                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            depth++;
        }

        return depth;
    }
};

#endif