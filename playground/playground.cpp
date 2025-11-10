#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * 有向加权图（邻接矩阵实现）
 * 
 * 成员变量：
 * - vertices: 顶点数量
 * - adjMatrix: 邻接矩阵（二维vector）
 * - INF: 表示无穷大（无连接）
 * 
 * 公有成员函数：
 * 【基本操作】
 * 1. 构造函数 DirectedWeightedGraph(int v)
 * 2. addEdge(from, to, weight) - 添加有向边
 * 3. removeEdge(from, to) - 删除有向边
 * 4. hasEdge(from, to) - 检查边是否存在
 * 5. getWeight(from, to) - 获取边的权重
 * 
 * 【图的属性】
 * 6. getInDegree(vertex) - 获取顶点入度
 * 7. getOutDegree(vertex) - 获取顶点出度
 * 8. printMatrix() - 打印邻接矩阵
 * 9. printGraphInfo() - 打印图的基本信息
 * 
 * 【图遍历算法】
 * 10. dfsRecursive(start) - DFS递归版本
 * 11. dfsIterative(start) - DFS迭代版本
 * 12. bfs(start) - BFS广度优先搜索
 * 
 * 【路径查找】
 * 13. findPathDFS(start, end) - 使用DFS查找路径
 * 14. findPathBFS(start, end) - 使用BFS查找最短路径（边数最少）
 * 
 * 【最短路径算法】
 * 15. dijkstra(start) - Dijkstra算法求单源最短路径
 * 16. floydWarshall() - Floyd-Warshall算法求所有顶点对最短路径
 * 17. printShortestPaths(start) - 打印从指定顶点的最短路径
 * 18. printAllShortestPaths() - 打印所有顶点对最短路径
 * 
 * 【图的性质检测】
 * 19. hasCycle() - 检测是否存在环
 * 20. hasNegativeCycle() - 检测是否存在负权重环
 * 21. isStronglyConnected() - 检查是否强连通
 * 22. topologicalSort() - 拓扑排序（仅对DAG有效）
 * 
 * 【辅助打印函数】
 * 23. printAllTraversals(start) - 打印所有遍历结果
 * 24. printPathFinding(start, end) - 打印路径查找结果
 * 
 * 私有辅助函数：
 * - isValidVertex(vertex) - 检查顶点索引是否有效
 * - dfsRecursiveHelper(vertex, visited) - DFS递归辅助函数
 * - findPathDFSHelper(...) - DFS路径查找辅助函数
 * - topologicalSortHelper(...) - 拓扑排序辅助函数
 * - dfsHasCycle(...) - 环检测辅助函数
 * - minDistance(...) - 找到未访问顶点中距离最小的顶点
 */
class DirectedWeightedGraph {
private:
    size_t vertices;                              // 顶点数量
    vector<vector<int>> adjMatrix;      // 邻接矩阵
    static const int INF = INT_MAX;               // 表示无穷大（无连接）

public:
    //  1. 构造函数 
    explicit DirectedWeightedGraph(int v) : vertices(static_cast<size_t>(v)) {
        adjMatrix.assign(vertices, vector<int>(vertices, INF));
        for (size_t i = 0; i < vertices; i++) {
            adjMatrix[i][i] = 0;
        }
    }

    //  2. 添加有向边 
    void addEdge(int from, int to, int weight) {
        if (isValidVertex(from) && isValidVertex(to)) {
            adjMatrix[static_cast<size_t>(from)][static_cast<size_t>(to)] = weight;
        }
    }

    //  3. 删除有向边 
    void removeEdge(int from, int to) {
        if (isValidVertex(from) && isValidVertex(to)) {
            adjMatrix[static_cast<size_t>(from)][static_cast<size_t>(to)] = (from == to) ? 0 : INF;
        }
    }

    //  4. 检查是否存在边 
    bool hasEdge(int from, int to) const {
        if (isValidVertex(from) && isValidVertex(to)) {
            int weight = adjMatrix[static_cast<size_t>(from)][static_cast<size_t>(to)];
            return weight != INF && !(from == to && weight == 0);
        }
        return false;
    }

    //  5. 获取边的权重 
    int getWeight(int from, int to) const {
        if (isValidVertex(from) && isValidVertex(to)) {
            return adjMatrix[static_cast<size_t>(from)][static_cast<size_t>(to)];
        }
        return INF;
    }

    //  6. 获取顶点入度 
    int getInDegree(int vertex) const {
        if (!isValidVertex(vertex)) return -1;
        int degree = 0;
        size_t v = static_cast<size_t>(vertex);
        for (size_t i = 0; i < vertices; i++) {
            if (i != v && adjMatrix[i][v] != INF) {
                degree++;
            }
        }
        return degree;
    }

    //  7. 获取顶点出度 
    int getOutDegree(int vertex) const {
        if (!isValidVertex(vertex)) return -1;
        int degree = 0;
        size_t v = static_cast<size_t>(vertex);
        for (size_t i = 0; i < vertices; i++) {
            if (i != v && adjMatrix[v][i] != INF) {
                degree++;
            }
        }
        return degree;
    }

    //  8. 打印邻接矩阵 
    void printMatrix() const {
        cout << "\n邻接矩阵 (∞ 表示无连接):\n";
        cout << setw(4) << " ";
        for (size_t i = 0; i < vertices; i++) {
            cout << setw(4) << i;
        }
        cout << endl;
        for (size_t i = 0; i < vertices; i++) {
            cout << setw(3) << i << ":";
            for (size_t j = 0; j < vertices; j++) {
                if (adjMatrix[i][j] == INF) {
                    cout << setw(4) << "∞";
                } else {
                    cout << setw(4) << adjMatrix[i][j];
                }
            }
            cout << endl;
        }
    }

    //  9. 打印图的基本信息 
    void printGraphInfo() const {
        cout << "\n图的信息:\n";
        cout << "顶点数: " << vertices << endl;
        int edgeCount = 0;
        for (size_t i = 0; i < vertices; i++) {
            for (size_t j = 0; j < vertices; j++) {
                if (i != j && adjMatrix[i][j] != INF) {
                    edgeCount++;
                }
            }
        }
        cout << "边数: " << edgeCount << endl;
        cout << "顶点度数:\n";
        for (size_t i = 0; i < vertices; i++) {
            cout << "  顶点 " << i << " - 入度: " << getInDegree(static_cast<int>(i)) 
                      << ", 出度: " << getOutDegree(static_cast<int>(i)) << endl;
        }
    }

    //  10. DFS递归版本 
    void dfsRecursive(int start) const {
        if (!isValidVertex(start)) {
            cout << "无效的起始顶点!" << endl;
            return;
        }
        vector<bool> visited(vertices, false);
        cout << "\nDFS (递归) 从顶点 " << start << " 开始: ";
        dfsRecursiveHelper(start, visited);
        cout << endl;
    }

    //  11. DFS迭代版本 
    void dfsIterative(int start) const {
        if (!isValidVertex(start)) {
            cout << "无效的起始顶点!" << endl;
            return;
        }
        vector<bool> visited(vertices, false);
        stack<int> stack;
        stack.push(start);
        cout << "\nDFS (迭代) 从顶点 " << start << " 开始: ";
        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();
            if (!visited[static_cast<size_t>(current)]) {
                visited[static_cast<size_t>(current)] = true;
                cout << current << " ";
                for (int v = static_cast<int>(vertices) - 1; v >= 0; v--) {
                    size_t vIdx = static_cast<size_t>(v);
                    size_t currIdx = static_cast<size_t>(current);
                    if (!visited[vIdx] && vIdx != currIdx && adjMatrix[currIdx][vIdx] != INF) {
                        stack.push(v);
                    }
                }
            }
        }
        cout << endl;
    }

    //  12. BFS广度优先搜索 
    void bfs(int start) const {
        if (!isValidVertex(start)) {
            cout << "无效的起始顶点!" << endl;
            return;
        }
        vector<bool> visited(vertices, false);
        queue<int> queue;
        visited[static_cast<size_t>(start)] = true;
        queue.push(start);
        cout << "\nBFS 从顶点 " << start << " 开始: ";
        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();
            cout << current << " ";
            size_t currIdx = static_cast<size_t>(current);
            for (size_t v = 0; v < vertices; v++) {
                if (!visited[v] && v != currIdx && adjMatrix[currIdx][v] != INF) {
                    visited[v] = true;
                    queue.push(static_cast<int>(v));
                }
            }
        }
        cout << endl;
    }

    //  13. DFS路径查找 
    vector<int> findPathDFS(int start, int end) const {
        if (!isValidVertex(start) || !isValidVertex(end)) return {};
        vector<bool> visited(vertices, false);
        vector<int> path, result;
        if (findPathDFSHelper(start, end, visited, path, result)) {
            return result;
        }
        return {};
    }

    //  14. BFS路径查找 
    vector<int> findPathBFS(int start, int end) const {
        if (!isValidVertex(start) || !isValidVertex(end)) return {};
        if (start == end) return {start};
        vector<bool> visited(vertices, false);
        vector<int> parent(vertices, -1);
        queue<int> queue;
        visited[static_cast<size_t>(start)] = true;
        queue.push(start);
        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();
            size_t currIdx = static_cast<size_t>(current);
            for (size_t v = 0; v < vertices; v++) {
                if (!visited[v] && v != currIdx && adjMatrix[currIdx][v] != INF) {
                    visited[v] = true;
                    parent[v] = current;
                    if (static_cast<int>(v) == end) {
                        vector<int> path;
                        int curr = end;
                        while (curr != -1) {
                            path.push_back(curr);
                            curr = parent[static_cast<size_t>(curr)];
                        }
                        reverse(path.begin(), path.end());
                        return path;
                    }
                    queue.push(static_cast<int>(v));
                }
            }
        }
        return {};
    }

    //  15. Dijkstra算法 
    vector<int> dijkstra(int start) const {
        if (!isValidVertex(start)) return {};
        vector<int> dist(vertices, INF);
        vector<bool> visited(vertices, false);
        dist[static_cast<size_t>(start)] = 0;
        for (size_t count = 0; count < vertices - 1; count++) {
            int u = minDistance(dist, visited);
            if (u == -1) break;
            size_t uIdx = static_cast<size_t>(u);
            visited[uIdx] = true;
            for (size_t v = 0; v < vertices; v++) {
                if (!visited[v] && adjMatrix[uIdx][v] != INF && 
                    dist[uIdx] != INF && dist[uIdx] + adjMatrix[uIdx][v] < dist[v]) {
                    dist[v] = dist[uIdx] + adjMatrix[uIdx][v];
                }
            }
        }
        return dist;
    }

    //  16. Floyd-Warshall算法 
    vector<vector<int>> floydWarshall() const {
        vector<vector<int>> dist = adjMatrix;
        for (size_t k = 0; k < vertices; k++) {
            for (size_t i = 0; i < vertices; i++) {
                for (size_t j = 0; j < vertices; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF &&
                        dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
        return dist;
    }

    //  17. 打印从指定顶点的最短路径 
    void printShortestPaths(int start) const {
        vector<int> distances = dijkstra(start);
        cout << "\n从顶点 " << start << " 的最短距离:\n";
        for (size_t i = 0; i < vertices; i++) {
            cout << "到顶点 " << i << ": ";
            if (distances[i] == INF) {
                cout << "无路径" << endl;
            } else {
                cout << distances[i] << endl;
            }
        }
    }

    //  18. 打印所有顶点对最短路径 
    void printAllShortestPaths() const {
        auto allDist = floydWarshall();
        cout << "\n所有顶点对最短路径:\n";
        cout << setw(8) << "从\\到";
        for (size_t i = 0; i < vertices; i++) {
            cout << setw(6) << i;
        }
        cout << endl;
        for (size_t i = 0; i < vertices; i++) {
            cout << setw(7) << i << ":";
            for (size_t j = 0; j < vertices; j++) {
                if (allDist[i][j] == INF) {
                    cout << setw(6) << "∞";
                } else {
                    cout << setw(6) << allDist[i][j];
                }
            }
            cout << endl;
        }
    }

    //  19. 检测是否存在环 
    bool hasCycle() const {
        vector<int> color(vertices, 0);
        for (size_t i = 0; i < vertices; i++) {
            if (color[i] == 0) {
                if (dfsHasCycle(static_cast<int>(i), color)) {
                    return true;
                }
            }
        }
        return false;
    }

    //  20. 检测是否存在负权重环 
    bool hasNegativeCycle() const {
        auto dist = floydWarshall();
        for (size_t i = 0; i < vertices; i++) {
            if (dist[i][i] < 0) {
                return true;
            }
        }
        return false;
    }

    //  21. 检查是否强连通 
    bool isStronglyConnected() const {
        for (size_t start = 0; start < vertices; start++) {
            vector<bool> visited(vertices, false);
            dfsForConnectivity(static_cast<int>(start), visited);
            for (size_t i = 0; i < vertices; i++) {
                if (!visited[i]) {
                    return false;
                }
            }
        }
        return true;
    }

    //  22. 拓扑排序 
    vector<int> topologicalSort() const {
        vector<bool> visited(vertices, false);
        stack<int> stack;
        for (size_t i = 0; i < vertices; i++) {
            if (!visited[i]) {
                topologicalSortHelper(static_cast<int>(i), visited, stack);
            }
        }
        vector<int> result;
        while (!stack.empty()) {
            result.push_back(stack.top());
            stack.pop();
        }
        return result;
    }

    //  23. 打印所有遍历结果 
    void printAllTraversals(int start) const {
        cout << "\n=== 从顶点 " << start << " 的图遍历 ===";
        dfsRecursive(start);
        dfsIterative(start);
        bfs(start);
    }

    //  24. 打印路径查找结果 
    void printPathFinding(int start, int end) const {
        cout << "\n=== 从 " << start << " 到 " << end << " 的路径查找 ===\n";
        auto dfsPath = findPathDFS(start, end);
        cout << "DFS路径: ";
        if (dfsPath.empty()) {
            cout << "无路径";
        } else {
            for (size_t i = 0; i < dfsPath.size(); i++) {
                cout << dfsPath[i];
                if (i < dfsPath.size() - 1) cout << " -> ";
            }
        }
        cout << endl;
        auto bfsPath = findPathBFS(start, end);
        cout << "BFS路径 (边数最少): ";
        if (bfsPath.empty()) {
            cout << "无路径";
        } else {
            for (size_t i = 0; i < bfsPath.size(); i++) {
                cout << bfsPath[i];
                if (i < bfsPath.size() - 1) cout << " -> ";
            }
        }
        cout << endl;
    }

private:
    //  辅助函数 
    bool isValidVertex(int vertex) const {
        return vertex >= 0 && static_cast<size_t>(vertex) < vertices;
    }

    void dfsRecursiveHelper(int vertex, vector<bool>& visited) const {
        visited[static_cast<size_t>(vertex)] = true;
        cout << vertex << " ";
        size_t vIdx = static_cast<size_t>(vertex);
        for (size_t v = 0; v < vertices; v++) {
            if (!visited[v] && v != vIdx && adjMatrix[vIdx][v] != INF) {
                dfsRecursiveHelper(static_cast<int>(v), visited);
            }
        }
    }

    bool findPathDFSHelper(int current, int target, vector<bool>& visited, 
                          vector<int>& path, vector<int>& result) const {
        visited[static_cast<size_t>(current)] = true;
        path.push_back(current);
        if (current == target) {
            result = path;
            return true;
        }
        size_t currIdx = static_cast<size_t>(current);
        for (size_t v = 0; v < vertices; v++) {
            if (!visited[v] && v != currIdx && adjMatrix[currIdx][v] != INF) {
                if (findPathDFSHelper(static_cast<int>(v), target, visited, path, result)) {
                    return true;
                }
            }
        }
        path.pop_back();
        return false;
    }

    void topologicalSortHelper(int vertex, vector<bool>& visited, stack<int>& stack) const {
        visited[static_cast<size_t>(vertex)] = true;
        size_t vIdx = static_cast<size_t>(vertex);
        for (size_t v = 0; v < vertices; v++) {
            if (!visited[v] && v != vIdx && adjMatrix[vIdx][v] != INF) {
                topologicalSortHelper(static_cast<int>(v), visited, stack);
            }
        }
        stack.push(vertex);
    }

    bool dfsHasCycle(int vertex, vector<int>& color) const {
        size_t vIdx = static_cast<size_t>(vertex);
        color[vIdx] = 1;
        for (size_t v = 0; v < vertices; v++) {
            if (v != vIdx && adjMatrix[vIdx][v] != INF) {
                if (color[v] == 1) {
                    return true;
                }
                if (color[v] == 0 && dfsHasCycle(static_cast<int>(v), color)) {
                    return true;
                }
            }
        }
        color[vIdx] = 2;
        return false;
    }

    void dfsForConnectivity(int vertex, vector<bool>& visited) const {
        visited[static_cast<size_t>(vertex)] = true;
        size_t vIdx = static_cast<size_t>(vertex);
        for (size_t v = 0; v < vertices; v++) {
            if (!visited[v] && v != vIdx && adjMatrix[vIdx][v] != INF) {
                dfsForConnectivity(static_cast<int>(v), visited);
            }
        }
    }

    int minDistance(const vector<int>& dist, const vector<bool>& visited) const {
        int min = INF, minIndex = -1;
        for (size_t v = 0; v < vertices; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = static_cast<int>(v);
            }
        }
        return minIndex;
    }
};

// 定义静态常量
const int DirectedWeightedGraph::INF;

//  主函数：演示所有功能 
int main() {
    cout << "=== 有向加权图演示 ===\n";
    
    // 创建图
    DirectedWeightedGraph graph(5);
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 4, 8);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 4, 2);
    graph.addEdge(2, 3, 1);
    graph.addEdge(3, 4, 3);
    graph.addEdge(4, 2, 1);
    
    // 基本操作测试
    graph.printMatrix();
    graph.printGraphInfo();
    
    // 遍历算法测试
    graph.printAllTraversals(0);
    
    // 路径查找测试
    graph.printPathFinding(0, 3);
    graph.printPathFinding(1, 4);
    
    // 最短路径算法测试
    graph.printShortestPaths(0);
    graph.printAllShortestPaths();
    
    // 图的性质检测
    cout << "\n=== 图的性质 ===\n";
    cout << "是否有环: " << (graph.hasCycle() ? "是" : "否") << endl;
    cout << "是否有负权重环: " << (graph.hasNegativeCycle() ? "是" : "否") << endl;
    cout << "是否强连通: " << (graph.isStronglyConnected() ? "是" : "否") << endl;
    
    // DAG测试
    cout << "\n=== DAG（有向无环图）测试 ===\n";
    DirectedWeightedGraph dag(4);
    dag.addEdge(0, 1, 5);
    dag.addEdge(0, 2, 3);
    dag.addEdge(1, 3, 2);
    dag.addEdge(2, 3, 4);
    
    dag.printMatrix();
    cout << "是否有环: " << (dag.hasCycle() ? "是" : "否") << endl;
    
    if (!dag.hasCycle()) {
        auto topoSort = dag.topologicalSort();
        cout << "拓扑排序: ";
        for (size_t i = 0; i < topoSort.size(); i++) {
            cout << topoSort[i];
            if (i < topoSort.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    
    dag.printAllTraversals(0);
    
    return 0;
}
