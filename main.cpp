#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

struct Graph {
    int nodes;
    vector<vector<int>> adj;
    vector<int> colors;

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

    int selectUncoloredNode() {
        // MRV策略：选择颜色最少的未填色节点
        int selectedNode = -1;
        int minColors = numeric_limits<int>::max();

        for (int i = 1; i <= nodes; i++) {
            if (colors[i] == -1) {
                int count = 0;
                for (int color = 1; color <= 5; color++) { // 假设最大颜色数为5
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
};

int main() {
<<<<<<< HEAD
    string filename = "le450_5a.col";
=======
    string filename = "D:\\Hyh\\Develop\\Exp3\\Algorithm_exp3_mapcoloring\\le450_5a.col";
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return 1;
    }

    string line;
    int nodes = 0, edges = 0;
    Graph graph(0);

    while (getline(file, line)) {
        if (line[0] == 'p') {
            sscanf(line.c_str(), "p edge %d %d", &nodes, &edges);
            graph = Graph(nodes);
        } else if (line[0] == 'e') {
            int u, v;
            sscanf(line.c_str(), "e %d %d", &u, &v);
            graph.addEdge(u, v);
        }
    }

    cout << "图有 " << nodes << " 个节点和 " << edges << " 条边" << endl;

    int maxColors = 15; // 可以改为15或25
    auto start = chrono::high_resolution_clock::now();
    bool success = graph.backtrack(maxColors);
    auto end = chrono::high_resolution_clock::now();

    double timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "使用时间：" << timeTaken << " 毫秒" << endl;

    if (success) {
        cout << "颜色分配结果：" << endl;
        for (int i = 1; i <= nodes; i++) {
            cout << "节点 " << i << " 颜色: " << graph.colors[i] << endl;
        }
    } else {
        cout << "无法用 " << maxColors << " 种颜色填色" << endl;
    }

    file.close();
    return 0;
}