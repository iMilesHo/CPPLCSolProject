# Minimum Depth of Binary Tree

- **ID:** 111
- **Difficulty:** EASY
- **Topic Tags:** Tree, Depth-First Search, Breadth-First Search, Binary Tree
- **Link:** [LeetCode Problem](https://leetcode.com/problems/minimum-depth-of-binary-tree/description/)

## Problem Description

Given a binary tree, find its minimum depth.

The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.

Note: A leaf is a node with no children.

## Approach

### DFS Solution (Your Implementation) ✅

Your DFS solution is **correct and well-thought-out**! The key insight you captured is crucial:

```cpp
class Solution {
public:
    int minimumDepthDFS(TreeNode* node) {
        if(!node) {
            return 0;
        }

        int leftDepth = minimumDepthDFS(node->left);
        int rightDepth = minimumDepthDFS(node->right);

        // Key insight: if one subtree is null, we must go down the other path
        if((leftDepth == 0 && rightDepth != 0) ||
            (leftDepth != 0 && rightDepth == 0)) {
            return 1 + max(leftDepth, rightDepth);
        }

        return 1 + min(leftDepth, rightDepth);
    }
    int minDepth(TreeNode* root) {
        return minimumDepthDFS(root);
    }
};
```

**Critical Edge Case Handled**: When one child is null, you correctly use `max()` instead of `min()` because we need to find the path to a _leaf_ node (both children null), not just any null pointer.

### BFS Solution (Optimal for This Problem) 🚀

```cpp
int minDepth(TreeNode* root) {
    if (!root) return 0;

    queue<TreeNode*> q;
    q.push(root);
    int depth = 1;

    while (!q.empty()) {
        int levelSize = q.size();

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            // Found the first leaf node - this is minimum depth
            if (!node->left && !node->right) {
                return depth;
            }

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        depth++;
    }
    return depth;
}
```

## 🎯 Elevator Pitch: Why BFS is The Winner

**"For minimum depth problems, BFS is like taking the express elevator - it stops at the first floor with what you need."**

- **Early Termination**: BFS finds the first leaf node immediately, while DFS might waste time exploring deep branches
- **Best Case Scenario**: If the minimum depth is 2 but the tree is very deep on one side, BFS returns in O(few nodes) while DFS explores O(all nodes)
- **Space Trade-off**: BFS uses O(width) space vs DFS's O(height) space - usually width < height for minimum depth cases
- **Intuitive**: Level-by-level exploration naturally aligns with "shortest path" thinking

**Bottom Line**: Your DFS is perfectly correct, but BFS is _optimal_ for minimum depth because it exploits the problem's structure for early termination!

## Complexity

### DFS Solution:

- **Time Complexity:** O(N) - Must visit all nodes in worst case
- **Space Complexity:** O(H) - Recursion stack depth equals tree height

### BFS Solution:

- **Time Complexity:** O(N) worst case, but **O(M)** average case where M = nodes until first leaf
- **Space Complexity:** O(W) where W = maximum width of tree (typically less than height)
