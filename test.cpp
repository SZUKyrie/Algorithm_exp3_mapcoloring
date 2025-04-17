#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <numeric>
#include <queue>
#include <algorithm>

using namespace std;

struct Graph {
    int nodes;
    vector<vector<int>> adj;
    vector<int> colors;
    vector<int> domain; // 颜色域

    Graph(int n) : nodes(n), adj(n + 1), colors(n + 1, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool isSafe(int node, int color) {
        for (int neighbor : adj[node]) {
            if (colors[neighbor] == color) return false;
        }
        return true;
    }

    // 普通回溯法
    bool backtrack(int maxColors) {
        int node = selectUncoloredNode();
        if (node == -1) return true; // 所有节点都已填色

        for (int color = 1; color <= maxColors; color++) {
            if (isSafe(node, color)) {
                colors[node] = color;
                if (backtrack(maxColors)) return true;
                colors[node] = -1;
            }
        }
        return false;
    }

    // MRV优化
    bool backtrackMRV(int maxColors) {
        int node = selectUncoloredNodeMRV();
        if (node == -1) return true; // 所有节点都已填色

        vector<int> availableColors;
        for (int color = 1; color <= maxColors; color++) {
            if (isSafe(node, color)) {
                availableColors.push_back(color);
            }
        }

        for (int color : availableColors) {
            colors[node] = color;
            if (backtrackMRV(maxColors)) return true;
            colors[node] = -1;
        }
        return false;
    }

    // DH优化
    bool backtrackDH(int maxColors) {
        int node = selectUncoloredNodeDH();
        if (node == -1) return true; // 所有节点都已填色

        for (int color = 1; color <= maxColors; color++) {
            if (isSafe(node, color)) {
                colors[node] = color;
                if (backtrackDH(maxColors)) return true;
                colors[node] = -1;
            }
        }
        return false;
    }

    // MRV+DH优化
    bool backtrackMRVDH(int maxColors) {
        int node = selectUncoloredNodeMRVDH();
        if (node == -1) return true; // 所有节点都已填色

        vector<int> availableColors;
        for (int color = 1; color <= maxColors; color++) {
            if (isSafe(node, color)) {
                availableColors.push_back(color);
            }
        }

        for (int color : availableColors) {
            colors[node] = color;
            if (backtrackMRVDH(maxColors)) return true;
            colors[node] = -1;
        }
        return false;
    }

    // MRV+DH+向前探测优化
    bool backtrackMRVDHForward(int maxColors) {
        int node = selectUncoloredNodeMRVDH();
        if (node == -1) return true; // 所有节点都已填色

        vector<int> availableColors;
        for (int color = 1; color <= maxColors; color++) {
            if (isSafe(node, color)) {
                availableColors.push_back(color);
            }
        }

        for (int color : availableColors) {
            if (forwardChecking(node, color, maxColors)) {
                colors[node] = color;
                if (backtrackMRVDHForward(maxColors)) return true;
                colors[node] = -1;
            }
        }
        return false;
    }

    // 选择未填色的节点（普通回溯法）
    int selectUncoloredNode() {
        for (int i = 1; i <= nodes; i++) {
            if (colors[i] == -1) return i;
        }
        return -1;
    }

    // MRV策略：选择颜色最少的未填色节点
    int selectUncoloredNodeMRV() {
        int selectedNode = -1;
        int minColors = numeric_limits<int>::max();

        for (int i = 1; i <= nodes; i++) {
            if (colors[i] == -1) {
                int count = 0;
                for (int color = 1; color <= domain.size(); color++) {
                    if (isSafe(i, color)) count++;
                }
                if (count < minColors) {
                    minColors = count;
                    selectedNode = i;
                }
            }
        }
        return selectedNode;
    }

    // DH策略：选择约束最多的节点
    int selectUncoloredNodeDH() {
        int selectedNode = -1;
        int maxDegree = -1;

        for (int i = 1; i <= nodes; i++) {
            if (colors[i] == -1) {
                int degree = adj[i].size();
                if (degree > maxDegree) {
                    maxDegree = degree;
                    selectedNode = i;
                }
            }
        }
        return selectedNode;
    }

    // MRV+DH策略：结合MRV和DH
    int selectUncoloredNodeMRVDH() {
        int selectedNode = selectUncoloredNodeMRV();
        if (selectedNode == -1) {
            selectedNode = selectUncoloredNodeDH();
        }
        return selectedNode;
    }

    // 向前探测：检查当前选择是否会导致后续节点无法填色
    bool forwardChecking(int node, int color, int maxColors) {
        colors[node] = color;
        bool valid = true;

        for (int neighbor : adj[node]) {
            if (colors[neighbor] == -1) {
                vector<int> availableColors;
                for (int c = 1; c <= maxColors; c++) {
                    bool conflict = false;
                    for (int adjNode : adj[neighbor]) {
                        if (colors[adjNode] == c) {
                            conflict = true;
                            break;
                        }
                    }
                    if (!conflict) {
                        availableColors.push_back(c);
                    }
                }
                if (availableColors.empty()) {
                    valid = false;
                    break;
                }
            }
        }

        colors[node] = -1; // 回溯
        return valid;
    }
};

void testAlgorithm(const string& filename, const vector<int>& domains) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件: " << filename << endl;
        return;
    }

    string line;
    int nodes = 0, edges = 0;
    Graph graph(0);

    while (getline(file, line)) {
        if (line[0] == 'p') {
            sscanf(line.c_str(), "p edge %d %d", &nodes, &edges);
            graph = Graph(nodes);
            graph.domain.resize(nodes + 1);
            for (int i = 1; i <= nodes; i++) {
                graph.domain[i] = i;
            }
        } else if (line[0] == 'e') {
            int u, v;
            sscanf(line.c_str(), "e %d %d", &u, &v);
            graph.addEdge(u, v);
        }
    }

    file.close();

    cout << "测试文件: " << filename << ", 节点数: " << nodes << ", 边数: " << edges << endl;

    for (int maxColors : domains) {
        Graph g = graph; // 复制图
        cout << "颜色数: " << maxColors << endl;

        // 普通回溯法
        auto start = chrono::high_resolution_clock::now();
        bool success = g.backtrack(maxColors);
        auto end = chrono::high_resolution_clock::now();

        double timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "普通回溯法: ";
        if (success) {
            cout << "成功，用时 " << timeTaken << " 毫秒" << endl;
        } else {
            cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
        }

        // MRV优化
        g = graph;
        start = chrono::high_resolution_clock::now();
        success = g.backtrackMRV(maxColors);
        end = chrono::high_resolution_clock::now();

        timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "MRV优化: ";
        if (success) {
            cout << "成功，用时 " << timeTaken << " 毫秒" << endl;
        } else {
            cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
        }

        // DH优化
        g = graph;
        start = chrono::high_resolution_clock::now();
        success = g.backtrackDH(maxColors);
        end = chrono::high_resolution_clock::now();

        timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "DH优化: ";
        if (success) {
            cout << "成功，用时 " << timeTaken << " 毫秒" << endl;
        } else {
            cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
        }

        // MRV+DH优化
        g = graph;
        start = chrono::high_resolution_clock::now();
        success = g.backtrackMRVDH(maxColors);
        end = chrono::high_resolution_clock::now();

        timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "MRV+DH优化: ";
        if (success) {
            cout << "成功，用时 " << timeTaken << " 毫秒" << endl;
        } else {
            cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
        }

        // MRV+DH+向前探测优化
        g = graph;
        start = chrono::high_resolution_clock::now();
        success = g.backtrackMRVDHForward(maxColors);
        end = chrono::high_resolution_clock::now();

        timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "MRV+DH+向前探测优化: ";
        if (success) {
            cout << "成功，用时 " << timeTaken << " 毫秒" << endl;
        } else {
            cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
        }

        cout << "----------------------------------------" << endl;
    }
}

int main() {
    vector<string> filenames = {"le450_5a.col", "le450_15b.col", "le450_25a.col"};
    vector<int> domains = {140, 250, 360}; // 颜色数

    for (const string& filename : filenames) {
        testAlgorithm(filename, domains);
    }

    return 0;
}