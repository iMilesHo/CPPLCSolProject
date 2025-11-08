# Binary Tree Level Order Traversal

- **ID:** 102
- **Difficulty:** MEDIUM
- **Topic Tags:** Tree, Breadth-First Search, Binary Tree
- **Link:** [LeetCode Problem](https://leetcode.com/problems/binary-tree-level-order-traversal/description/)

## Problem Description

Given the `root` of a binary tree, return the level order traversal of its nodes' values (i.e., from left to right, level by level).

**Examples:**

- Input: `root = [3,9,20,null,null,15,7]` → Output: `[[3],[9,20],[15,7]]`
- Input: `root = [1]` → Output: `[[1]]`
- Input: `root = []` → Output: `[]`

**Constraints:**

- The number of nodes in the tree is in the range `[0, 2000]`
- `-1000 <= Node.val <= 1000`

## Solution Analysis

### 1. Your Current Solution (DFS-based)

My original solution

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    void bfs(vector<vector<int>>& results, TreeNode* curr, int height) {
        if(results.size() < height+1) {
            results.push_back(vector<int>());
        }

        results[height].push_back(curr->val);
        if(curr->left) {
            bfs(results, curr->left, height+1);
        }
        if(curr->right) {
            bfs(results, curr->right, height+1);
        }
        return;
    }

    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> results{};
        if(root){
            bfs(results, root, 0);
        }

        return results;
    }
};
```

```cpp
class Solution {
public:
    void dfsLevelOrder(vector<vector<int>>& result, TreeNode* node, int level) {
        if (!node) return;

        // Ensure we have enough levels in our result
        if (result.size() <= level) {
            result.emplace_back();
        }

        // Add current node to its level
        result[level].push_back(node->val);

        // Recursively process children
        dfsLevelOrder(result, node->left, level + 1);
        dfsLevelOrder(result, node->right, level + 1);
    }

    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfsLevelOrder(result, root, 0);
        return result;
    }
};
```

**Issues with your original code:**

1. Function name `bfs` is misleading - you're using DFS (depth-first search)
2. Variable naming: `results` vs `result`, `curr` vs `node`, `height` vs `level`
3. Unnecessary `return` statement at end of void function
4. Commented-out code should be removed
5. Size comparison `results.size() < height+1` is less readable than `results.size() <= height`

### 2. Standard BFS Solution (Recommended for interviews)

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) return {};

        vector<vector<int>> result;
        queue<TreeNode*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            int levelSize = nodeQueue.size();
            vector<int> currentLevel;
            currentLevel.reserve(levelSize); // Optimization

            // Process all nodes at current level
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = nodeQueue.front();
                nodeQueue.pop();

                currentLevel.push_back(node->val);

                // Add children for next level
                if (node->left) nodeQueue.push(node->left);
                if (node->right) nodeQueue.push(node->right);
            }

            result.push_back(std::move(currentLevel));
        }

        return result;
    }
};
```

### 3. Production-Ready Solution

```cpp
#include <vector>
#include <queue>
#include <memory>

class Solution {
public:
    /**
     * @brief Performs level-order traversal of a binary tree
     * @param root Pointer to the root of the binary tree
     * @return Vector of vectors containing node values grouped by level
     * @throws None - handles null input gracefully
     */
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        if (!root) {
            return {};
        }

        std::vector<std::vector<int>> result;
        std::queue<TreeNode*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            const int levelSize = nodeQueue.size();
            std::vector<int> currentLevel;
            currentLevel.reserve(levelSize); // Pre-allocate for efficiency

            // Process all nodes at the current level
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* const currentNode = nodeQueue.front();
                nodeQueue.pop();

                currentLevel.push_back(currentNode->val);

                // Enqueue children for the next level
                if (currentNode->left) {
                    nodeQueue.push(currentNode->left);
                }
                if (currentNode->right) {
                    nodeQueue.push(currentNode->right);
                }
            }

            result.push_back(std::move(currentLevel)); // Move semantics for efficiency
        }

        return result;
    }
};
```

## Complexity Analysis

### DFS Approach (Your Solution)

- **Time Complexity:** O(n) - Visit each node exactly once
- **Space Complexity:** O(h + w) where h is tree height (recursion stack) and w is maximum width

### BFS Approach (Standard)

- **Time Complexity:** O(n) - Visit each node exactly once
- **Space Complexity:** O(w) where w is the maximum width of the tree (queue size)

**Note:** BFS is generally preferred for level-order traversal as it's more intuitive and uses less stack space.

## Elevator Pitch (60 seconds)

"This problem asks us to visit a tree level by level, like reading a book from top to bottom, left to right.

Imagine you're a manager organizing employees by their reporting levels - CEO at level 0, VPs at level 1, directors at level 2, etc. You want to list everyone grouped by their level.

I use a queue-based approach: start with the root, then for each level, process all current nodes and add their children to the queue for the next level. It's like processing one floor of a building at a time.

The algorithm is optimal with O(n) time since we visit each node once, and O(w) space where w is the widest level - much better than recursive approaches that use O(height) stack space. This makes it efficient even for very deep or wide trees."

## Interview Follow-ups

1. **Reverse Level Order:** Return levels from bottom to top

   - Solution: Same BFS, then reverse the result or use a deque

2. **Zigzag Level Order:** Alternate left-to-right and right-to-left

   - Solution: Use a flag to determine direction per level

3. **Find nodes at specific level k**

   - Solution: Stop BFS when reaching level k

4. **Average of each level**

   - Solution: Calculate sum/count for each level during traversal

5. **Memory optimization for very wide trees**

   - Solution: Process one level at a time, don't store all levels

6. **Handle very deep trees (stack overflow prevention)**
   - Solution: BFS is already iterative, no recursion depth issues

## C++ Best Practices Used

- ✅ `emplace_back()` for in-place construction
- ✅ `reserve()` for vector pre-allocation
- ✅ `std::move()` for efficient transfers
- ✅ `const` correctness
- ✅ Early return for edge cases
- ✅ Clear variable naming
- ✅ Proper namespace usage (`std::`)
- ✅ RAII principles (automatic memory management)

### Why `emplace_back()` instead of `push_back()`?

```cpp
// ❌ Less efficient - creates temporary vector<int>, then copies/moves
result.push_back(vector<int>());

// ✅ More efficient - constructs vector<int> directly in result's memory
result.emplace_back();
```

**Key Benefits:**

1. **No temporary objects**: Constructs `vector<int>` directly in the container's memory
2. **Perfect forwarding**: Arguments are forwarded directly to the constructor
3. **Better performance**: Eliminates unnecessary copy/move operations

**When it matters most:**

- Complex objects with expensive constructors
- Move-only types (like `unique_ptr`)
- Performance-critical code

For simple types like `int`, the difference is negligible, but it's good practice to use `emplace_back()` consistently.

## Alternative Approaches

1. **Two-Queue Method:** Use two queues alternating between levels
2. **Sentinel Node:** Add nullptr between levels in single queue
3. **DFS with Level Tracking:** Your approach (less preferred for BFS problems)
4. **Morris Traversal:** O(1) space but complex and not practical for this problem
