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
<<<<<<< HEAD
    int nodes;
    vector<vector<int>> adj;
    vector<int> colors;
    vector<int> domain; // 颜色域

    Graph(int n) : nodes(n), adj(n + 1), colors(n + 1, -1) {}
=======
    int Size;
    vector<vector<int>> adj;
    vector<int> colors;
    int maxColors;
    vector<vector<int>> available_color;
    vector<int> sorted_degree;
    vector<int> degree;

    Graph(int n, int c) : Size(n), maxColors(c), adj(n + 1), colors(n + 1, -1), available_color(n + 1, vector<int>(c + 1, 0)), degree(n + 1, 0), sorted_degree(n + 1, 0) {
        for (int i = 1; i <= Size; i++) {
            available_color[i][0] = c;
            sorted_degree[i] = i;
        }
    }
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
<<<<<<< HEAD
    }

    bool isSafe(int node, int color) {
        for (int neighbor : adj[node]) {
=======
        degree[u]++;
        degree[v]++;
    }

    bool is_safe(int num, int color) {
        for (int neighbor : adj[num]) {
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
            if (colors[neighbor] == color) return false;
        }
        return true;
    }

    // 普通回溯法
<<<<<<< HEAD
    bool backtrack(int maxColors) {
        int node = selectUncoloredNode();
        if (node == -1) return true; // 所有节点都已填色

        for (int color = 1; color <= maxColors; color++) {
            if (isSafe(node, color)) {
                colors[node] = color;
                if (backtrack(maxColors)) return true;
=======
    bool simple_backtrack(int node) {
        if (node == Size + 1) return true;
        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                if (simple_backtrack(node + 1)) return true;
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
                colors[node] = -1;
            }
        }
        return false;
    }

<<<<<<< HEAD
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
=======
    // 普通回溯法（统计所有解的总数）
    int simple_backtrack_count(int node) {
        if (node == Size + 1) return 1; // 找到一个解

        int count = 0;
        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                count += simple_backtrack_count(node + 1);
                colors[node] = -1;
            }
        }
        return count;
    }

    // MRV优化
    bool MRV_backtrack() {
        int node = selectMRV();
        if (node == -1) return true;
        if (available_color[node][0] == 0) return false;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            colors[node] = color;
            updateMRV(node, color);
            if (MRV_backtrack()) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }

        return false;
    }

    // MRV优化（统计所有解的总数）
    int MRV_backtrack_count() {
        int node = selectMRV();
        if (node == -1) return 1; // 找到一个解
        if (available_color[node][0] == 0) return 0;

        int count = 0;
        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            colors[node] = color;
            updateMRV(node, color);
            count += MRV_backtrack_count();
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return count;
    }

    // DH优化
    bool __DH_backtrack(int num) {
        if (num == Size + 1) return true;
        int node = sorted_degree[num];

        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                if (__DH_backtrack(num + 1)) return true;
                colors[node] = -1;
            }
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
        }
        return false;
    }

    // DH优化
<<<<<<< HEAD
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
=======
    bool DH_backtrack(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });
        return __DH_backtrack(num);
    }

    int __DH_backtrack_count(int num) {
        if (num == Size + 1) return 1;
        int node = sorted_degree[num];

        int count = 0;
        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                count += __DH_backtrack_count(num + 1);
                colors[node] = -1;
            }
        }
        return count;
    }

    // DH优化（统计所有解的总数）
    int DH_backtrack_count(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });

        return __DH_backtrack_count(num);
    }

    // MRV+DH优化
    bool MRV_DH_backtrack() {
        int node = selectMRVDH();
        if (node == -1) return true; // 所有节点都已填色
        if (available_color[node][0] == 0) return false;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            updateMRV(node, color);
            colors[node] = color;
            if (MRV_DH_backtrack()) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return false;
    }

    // MRV+DH优化（统计所有解的总数）
    int MRV_DH_backtrack_count() {
        int node = selectMRVDH();
        if (node == -1) return 1; // 找到一个解
        if (available_color[node][0] == 0) return 0;

        int count = 0;
        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            updateMRV(node, color);
            colors[node] = color;
            count += MRV_DH_backtrack_count();
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        return count;
    }

    // MRV+DH+向前探测优化
    bool MRV_DH_FC_backtrack() {
        int node = selectMRVDH();
        if (node == -1) return true; // 所有节点都已填色
        if (available_color[node][0] == 0) return false;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            if (updateMRV(node, color) == false) return false; 
            colors[node] = color;
            if (MRV_DH_backtrack()) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return false;
    }

    // MRV+DH+向前探测优化（统计所有解的总数）
    int MRV_DH_FC_backtrack_count() {
        int node = selectMRVDH();
        if (node == -1) return 1; // 所有节点都已填色
        if (available_color[node][0] == 0) return 0;

        int count = 0;
        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            if (updateMRV(node, color) == false) return 0; 
            colors[node] = color;
            count += MRV_DH_backtrack_count();
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return count;
    }


    // MRV策略：选择颜色最少的未填色节点
    int selectMRV() {
        int selectedNode = -1;
        int minColors = maxColors + 1;

        for (int i = 1; i <= Size; i++) {
            if (colors[i] != -1) continue;
            if (minColors > available_color[i][0]) {
                minColors = available_color[i][0];
                selectedNode = i;
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
            }
        }
        return selectedNode;
    }

<<<<<<< HEAD
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
=======
    bool updateMRV(int node, int color) {
        bool flag = true;
        for (int neighbor : adj[node]) {
            if (colors[neighbor] != -1) continue;
            if (available_color[neighbor][color] == 0) {
                available_color[neighbor][color] = 1;
                available_color[neighbor][0] -= 1;
            }
            if (available_color[neighbor][0] == 0)  
                flag = false;
        }
        return flag;
    }

    void unupdateMRV(int node, int color) {
        for (int neighbor : adj[node]) {
            if (colors[neighbor] != -1) continue;
            if (is_safe(neighbor, color)) {
                available_color[neighbor][color] = 0;
                available_color[neighbor][0] += 1;
            }
        }
    }

    // MRV+DH策略：结合MRV和DH
    int selectMRVDH() {
        int selectedNode = -1;
        int minColors = maxColors + 1;

        for (int i = 1; i <= Size; i++) {
            if (colors[i] != -1) continue;
            if (minColors > available_color[i][0]) {
                minColors = available_color[i][0];
                selectedNode = i;
            }
            else if (minColors == available_color[i][0]) {
                if (degree[selectedNode] < degree[i]) {
                    selectedNode = i;
                }
            }
        }
        return selectedNode;
        

    }

};

void testAlgorithm(const string& filename, int maxColor) {
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件: " << filename << endl;
        return;
    }

    string line;
    int nodes = 0, edges = 0;
<<<<<<< HEAD
    Graph graph(0);
=======
    Graph graph(0, 0);
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306

    while (getline(file, line)) {
        if (line[0] == 'p') {
            sscanf(line.c_str(), "p edge %d %d", &nodes, &edges);
<<<<<<< HEAD
            graph = Graph(nodes);
            graph.domain.resize(nodes + 1);
            for (int i = 1; i <= nodes; i++) {
                graph.domain[i] = i;
            }
=======
            graph = Graph(nodes, maxColor);
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
        } else if (line[0] == 'e') {
            int u, v;
            sscanf(line.c_str(), "e %d %d", &u, &v);
            graph.addEdge(u, v);
        }
    }

    file.close();

    cout << "测试文件: " << filename << ", 节点数: " << nodes << ", 边数: " << edges << endl;

<<<<<<< HEAD
    for (int maxColors : domains) {
        Graph g = graph; // 复制图
        cout << "颜色数: " << maxColors << endl;

        // 普通回溯法
        auto start = chrono::high_resolution_clock::now();
        bool success = g.backtrack(maxColors);
        auto end = chrono::high_resolution_clock::now();

        double timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
=======

    Graph g = graph; // 复制图
    cout << "颜色数: " << maxColor << endl;

    auto start = chrono::high_resolution_clock::now();
    bool success;
    auto end = chrono::high_resolution_clock::now();
    double timeTaken;

    // 普通回溯法
    if (filename.find(string("small_data.col")) != string::npos) {
        start = chrono::high_resolution_clock::now();
        success = g.simple_backtrack(1);
        end = chrono::high_resolution_clock::now();
        timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306
        cout << "普通回溯法: ";
        if (success) {
            cout << "成功，用时 " << timeTaken << " 毫秒" << endl;
        } else {
            cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
        }
<<<<<<< HEAD

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
=======
        for (int i = 1; i <= nodes; i++) {
            cout << i << " -> " << g.colors[i] << endl;
        }

        g = graph;
        start = chrono::high_resolution_clock::now();
        int count = g.simple_backtrack_count(1);
        end = chrono::high_resolution_clock::now();
        timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "普通回溯法(总数): " << count << ", 用时 " << timeTaken << " 毫秒" << endl;
    }
    

    // MRV优化
    g = graph;
    start = chrono::high_resolution_clock::now();
    success = g.MRV_backtrack();
    end = chrono::high_resolution_clock::now();
    timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "MRV优化: ";
    if (success) {
        cout << "成功，用时 " << timeTaken << " 毫秒" << endl;

        // g = graph;
        // start = chrono::high_resolution_clock::now();
        // int count = g.MRV_backtrack_count();
        // end = chrono::high_resolution_clock::now();
        // timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        // cout << "MRV优化(总数): " << count << ", 用时 " << timeTaken << " 毫秒" << endl;
    } else {
        cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
    }


    // // DH优化 
    // g = graph;
    // start = chrono::high_resolution_clock::now();
    // success = g.DH_backtrack(1);
    // end = chrono::high_resolution_clock::now();
    // timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    // cout << "DH优化: ";
    // if (success) {
    //     cout << "成功，用时 " << timeTaken << " 毫秒" << endl;

    //     // g = graph;
    //     // start = chrono::high_resolution_clock::now();
    //     // int count = g.DH_backtrack_count(1);
    //     // end = chrono::high_resolution_clock::now();
    //     // timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    //     // cout << "DH优化(总数): " << count << ", 用时 " << timeTaken << " 毫秒" << endl;
    // } else {
    //     cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
    // }

    // MRV+DH优化
    g = graph;
    start = chrono::high_resolution_clock::now();
    success = g.MRV_DH_backtrack();
    end = chrono::high_resolution_clock::now();

    timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "MRV+DH优化: ";
    if (success) {
        cout << "成功，用时 " << timeTaken << " 毫秒" << endl;

        // g = graph;
        // start = chrono::high_resolution_clock::now();
        // int count = g.MRV_DH_backtrack_count();
        // end = chrono::high_resolution_clock::now();
        // timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        // cout << "MRV+DH优化(总数): " << count << ", 用时 " << timeTaken << " 毫秒" << endl;
    } else {
        cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
    }

    // MRV+DH+向前探测优化
    g = graph;
    start = chrono::high_resolution_clock::now();
    success = g.MRV_DH_FC_backtrack();
    end = chrono::high_resolution_clock::now();

    timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "MRV+DH+向前探测优化: ";
    if (success) {
        cout << "成功，用时 " << timeTaken << " 毫秒" << endl;

        // g = graph;
        // start = chrono::high_resolution_clock::now();
        // int count = g.MRV_DH_FC_backtrack_count();
        // end = chrono::high_resolution_clock::now();
        // timeTaken = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        // cout << "MRV+DH+向前探测优化(总数): " << count << ", 用时 " << timeTaken << " 毫秒" << endl;
    } else {
        cout << "失败，用时 " << timeTaken << " 毫秒" << endl;
    }

    cout << "----------------------------------------" << endl;
    
}

int main() {

    testAlgorithm("D:\\Hyh\\Develop\\Exp3\\Algorithm_exp3_mapcoloring\\small_data.col", 4);
    testAlgorithm("D:\\Hyh\\Develop\\Exp3\\Algorithm_exp3_mapcoloring\\le450_25a.col", 25);
    testAlgorithm("D:\\Hyh\\Develop\\Exp3\\Algorithm_exp3_mapcoloring\\le450_5a.col", 5);
    testAlgorithm("D:\\Hyh\\Develop\\Exp3\\Algorithm_exp3_mapcoloring\\le450_15b.col", 15);
>>>>>>> e3c726cad00306e9f89ed3a4ce37c1376505f306

    return 0;
}