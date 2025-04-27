#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <numeric>
#include <queue>
#include <algorithm>

using namespace std;

// 定义图结构体，用于图着色问题
struct Graph {
    int Size; // 图的节点数
    vector<vector<int>> adj; // 邻接表，表示图的边
    vector<int> colors; // 每个节点的颜色
    int maxColors; // 最大可用颜色数
    vector<vector<int>> available_color; // 记录每个节点可用的颜色
    vector<int> sorted_degree; // 按度排序的节点
    vector<int> degree; // 每个节点的度

    // 构造函数，初始化图结构
    Graph(int n, int c) : Size(n), maxColors(c), adj(n + 1), colors(n + 1, -1), available_color(n + 1, vector<int>(c + 1, 0)), degree(n + 1, 0), sorted_degree(n + 1, 0) {
        for (int i = 1; i <= Size; i++) {
            available_color[i][0] = c;
            sorted_degree[i] = i;
        }
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    bool is_safe(int num, int color) {
        for (int neighbor : adj[num]) {
            if (colors[neighbor] == color) return false;
        }
        return true;
    }

    // 普通回溯法
    bool simple_backtrack(int node) {
        if (node == Size + 1) return true;
        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                if (simple_backtrack(node + 1)) return true;
                colors[node] = -1;
            }
        }
        return false;
    }

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

    void __MRV_backtrack_count(int& ans) {
        if (ans == 1000000) return;
        int node = selectMRV();
        if (node == -1) { // 找到一个解
            ans++;
            return;
        } 
        if (available_color[node][0] == 0) return;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            colors[node] = color;
            updateMRV(node, color);
            __MRV_backtrack_count(ans);
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        return;
    }

    // MRV优化（统计所有解的总数）
    int MRV_backtrack_count() {
        int ans = 0;
        __MRV_backtrack_count(ans);
        return ans;
    }

    void __MRV_CC_backtrack_count(int& ans) {
        if (ans == 100000) return;
        int node = selectMRV();
        if (node == -1) { // 找到一个解
            ans++;
            return;
        } 
        if (available_color[node][0] == 0) return;

        if (ans == -1) {
            ans = 0;
            colors[node] = 1;
            updateMRV(node, 1);
            __MRV_CC_backtrack_count(ans);
            colors[node] = -1;
            unupdateMRV(node, 1);
        }
        else {
            for (int color = 1; color <= maxColors; color++) {
                if (available_color[node][color] == 1) continue;
                colors[node] = color;
                updateMRV(node, color);
                __MRV_CC_backtrack_count(ans);
                colors[node] = -1;
                unupdateMRV(node, color);
            }
        }
        return;
    }

    // MRV+颜色轮询优化（统计所有解的总数）
    int MRV_CC_backtrack_count() {
        int ans = -1;
        __MRV_CC_backtrack_count(ans);
        return ans * maxColors;
    }

    bool __DH_backtrack(int num) {
        if (num == Size + 1) return true;
        int node = sorted_degree[num];

        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                if (__DH_backtrack(num + 1)) return true;
                colors[node] = -1;
            }
        }
        return false;
    }

    // DH优化
    bool DH_backtrack(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });
        return __DH_backtrack(num);
    }

    void __DH_backtrack_count(int num, int& ans) {
        if (ans ==  1000000) return ;
        if (num == Size + 1) {
            ans++;
            return ;
        }
        int node = sorted_degree[num];

        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                __DH_backtrack_count(num + 1, ans);
                colors[node] = -1;
            }
        }
        return ;
    }

    // DH优化（统计所有解的总数）
    int DH_backtrack_count(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });

        int ans = 0;
        __DH_backtrack_count(num, ans);
        return ans;
    }

    void __DH_CC_backtrack_count(int num, int& ans) {
        if (ans ==  100000) return ;
        if (num == Size + 1) {
            ans++;
            return ;
        }
        int node = sorted_degree[num];

        if (num == 1) {
            colors[node] = 1;
            return __DH_CC_backtrack_count(num + 1, ans);
        }

        for (int color = 1; color <= maxColors; color++) {
            if (is_safe(node, color)) {
                colors[node] = color;
                __DH_CC_backtrack_count(num + 1, ans);
                colors[node] = -1;
            }
        }
        return ;
    }

    // DH+颜色轮询优化（统计所有解的总数）
    int DH_CC_backtrack_count(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });

        int ans = 0;
        __DH_CC_backtrack_count(num, ans);
        return ans * maxColors;
    }

    bool __MRV_DH_backtrack() {
        int node = selectMRVDH();
        if (node == -1) return true; // 所有节点都已填色
        if (available_color[node][0] == 0) return false;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            updateMRV(node, color);
            colors[node] = color;
            if (__MRV_DH_backtrack()) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return false;
    }

    // MRV+DH优化
    bool MRV_DH_backtrack() {
        updateMRV(43, 1);
        colors[43] = 1;
        return __MRV_DH_backtrack();
    }

    void __MRV_DH_backtrack_count(int& ans){
        if (ans ==  1000000) return ;
        int node = selectMRVDH();
        if (node == -1) { // 找到一个解
            ans++;
            return ;
        }
        if (available_color[node][0] == 0) return ;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            updateMRV(node, color);
            colors[node] = color;
            __MRV_DH_backtrack_count(ans);
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        return ;
    }

    // MRV+DH优化（统计所有解的总数）
    int MRV_DH_backtrack_count() {
        updateMRV(43, 1);
        colors[43] = 1;
        int ans = 0;
        __MRV_DH_backtrack_count(ans);
        return ans;
    }

    bool __DH_FC_backtrack(int num) {
        if (num == Size + 1) return true;
        int node = sorted_degree[num];
        if (available_color[node][0] == 0) return false;
        
        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            updateMRV(node, color);
            colors[node] = color;
            if (__DH_FC_backtrack(num + 1)) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        return false;
    }

    // DH+向前探测优化
    bool DH_FC_backtrack(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });
        return __DH_FC_backtrack(num);
    }

    void __DH_FC_backtrack_count(int num, int& ans) {
        if (ans ==  1000000) return ;
        if (num == Size + 1) {
            ans++;
            return ;
        }
        int node = sorted_degree[num];
        if (available_color[node][0] == 0) return ;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            updateMRV(node, color);
            colors[node] = color;
            __DH_FC_backtrack_count(num + 1, ans);
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        return ;
    }

    // DH+向前探测优化（统计所有解的总数）
    int DH_FC_backtrack_count(int num) {
        sort(sorted_degree.begin() + 1, sorted_degree.end(), [&](const int i, const int j){
            return degree[i] > degree[j];
        });
        int ans = 0;
        __DH_FC_backtrack_count(num, ans);
        return ans;
    }

    bool __MRV_DH_FC_backtrack() {
        int node = selectMRVDH();
        if (node == -1) return true; // 所有节点都已填色
        if (available_color[node][0] == 0) return false;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            if (updateMRV(node, color) == false) {
                unupdateMRV(node, color);
                return false; 
            }
            colors[node] = color;
            if (__MRV_DH_FC_backtrack()) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return false;
    }


    // MRV+DH+向前探测优化
    bool MRV_DH_FC_backtrack() {
        updateMRV(43, 1);
        colors[43] = 1;
        return __MRV_DH_FC_backtrack();
    }

    void __MRV_DH_FC_backtrack_count(int& ans) {
        if (ans == 100000) return ;
        int node = selectMRVDH();
        if (node == -1) {
            ans++;
            return ;
        }
        if (available_color[node][0] == 0) return ;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            if (updateMRV(node, color) == false) {
                unupdateMRV(node, color);
                return ; 
            } 
            colors[node] = color;
            __MRV_DH_FC_backtrack_count(ans);
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return ;
    }

    // MRV+DH+向前探测优化（统计所有解的总数）
    int MRV_DH_FC_backtrack_count() {
        updateMRV(43, 1);
        colors[43] = 1;
        int ans = 0;
        __MRV_DH_FC_backtrack_count(ans);
        return ans * maxColors;
    }

    // MRV+DH+向前探测优化+颜色轮询
    bool __MRV_DH_FC_CC_backtrack() {
        int node = selectMRVDH();
        if (node == -1) return true; // 所有节点都已填色
        if (available_color[node][0] == 0) return false;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            if (updateMRV(node, color) == false) {
                unupdateMRV(node, color);
                return false; 
            }
            colors[node] = color;
            if (__MRV_DH_FC_CC_backtrack()) return true;
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return false;
    }

    bool MRV_DH_FC_CC_backtrack() {
        updateMRV(43, 1);
        colors[43] = 1;
        return __MRV_DH_FC_CC_backtrack();
    }

    void __MRV_DH_FC_CC_backtrack_count(int& ans) {
        if (ans == 100000) return ;
        int node = selectMRVDH();
        if (node == -1) {
            ans++;
            return ;
        }
        if (available_color[node][0] == 0) return ;

        for (int color = 1; color <= maxColors; color++) {
            if (available_color[node][color] == 1) continue;
            if (updateMRV(node, color) == false) {
                unupdateMRV(node, color);
                return ; 
            } 
            colors[node] = color;
            __MRV_DH_FC_CC_backtrack_count(ans);
            colors[node] = -1;
            unupdateMRV(node, color);
        }
        
        return ;
    }

    // MRV+DH+向前探测优化+颜色轮询（统计所有解的总数）
    int MRV_DH_FC_CC_backtrack_count() {
        updateMRV(43, 1);
        colors[43] = 1;
        int ans = 0;
        __MRV_DH_FC_CC_backtrack_count(ans);
        return ans * maxColors;
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
            }
        }
        return selectedNode;
    }

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
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件: " << filename << endl;
        return;
    }

    string line;
    int nodes = 0, edges = 0;
    Graph graph(0, 0);

    while (getline(file, line)) {
        if (line[0] == 'p') {
            sscanf(line.c_str(), "p edge %d %d", &nodes, &edges);
            graph = Graph(nodes, maxColor);
        } else if (line[0] == 'e') {
            int u, v;
            sscanf(line.c_str(), "e %d %d", &u, &v);
            graph.addEdge(u, v);
        }
    }

    file.close();

    cout << "测试文件: " << filename << ", 节点数: " << nodes << ", 边数: " << edges << endl;


    Graph g = graph; // 复制图
    cout << "颜色数: " << maxColor << endl;

    auto start = chrono::high_resolution_clock::now();
    bool success;
    auto end = chrono::high_resolution_clock::now();
    double timeTaken;

    // // 普通回溯法
    // start = chrono::high_resolution_clock::now();
    // success = g.simple_backtrack(1);
    // end = chrono::high_resolution_clock::now();
    // timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    // cout << "普通回溯法: ";
    // if (success) {
    //     cout << "成功，用时 " << timeTaken << " 微秒" << endl;

    //     g = graph;
    //     start = chrono::high_resolution_clock::now();
    //     int count = g.simple_backtrack_count(1);
    //     end = chrono::high_resolution_clock::now();
    //     timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    //     cout << "普通回溯法(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    // } else {
    //     cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    // }

    // // MRV优化
    // g = graph;
    // start = chrono::high_resolution_clock::now();
    // success = g.MRV_backtrack();
    // end = chrono::high_resolution_clock::now();
    // timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    // cout << "MRV优化: ";
    // if (success) {
    //     cout << "成功，用时 " << timeTaken << " 微秒" << endl;

    //     g = graph;
    //     start = chrono::high_resolution_clock::now();
    //     int count = g.MRV_backtrack_count();
    //     end = chrono::high_resolution_clock::now();
    //     timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    //     cout << "MRV优化(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    // } else {
    //     cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    // }

    // // MRV+颜色轮询优化
    // g = graph;
    // start = chrono::high_resolution_clock::now();
    // int count = g.MRV_CC_backtrack_count();
    // end = chrono::high_resolution_clock::now();
    // timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    // cout << "MRV+颜色轮询优化(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;

    // // DH优化 
    // g = graph;
    // start = chrono::high_resolution_clock::now();
    // success = g.DH_backtrack(1);
    // end = chrono::high_resolution_clock::now();
    // timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    // cout << "DH优化: ";
    // if (success) {
    //     cout << "成功，用时 " << timeTaken << " 微秒" << endl;

    //     g = graph;
    //     start = chrono::high_resolution_clock::now();
    //     int count = g.DH_backtrack_count(1);
    //     end = chrono::high_resolution_clock::now();
    //     timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    //     cout << "DH优化(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    // } else {
    //     cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    // }

    // // DH+颜色轮询优化
    // g = graph;
    // start = chrono::high_resolution_clock::now();
    // int count01 = g.DH_CC_backtrack_count(1);
    // end = chrono::high_resolution_clock::now();
    // timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    // cout << "DH+颜色轮询优化(总数): " << count01 << ", 用时 " << timeTaken << " 微秒" << endl;

    // MRV+DH优化
    g = graph;
    start = chrono::high_resolution_clock::now();
    success = g.MRV_DH_backtrack();
    end = chrono::high_resolution_clock::now();

    timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "MRV+DH优化: ";
    if (success) {
        cout << "成功，用时 " << timeTaken << " 微秒" << endl;

        g = graph;
        start = chrono::high_resolution_clock::now();
        int count = g.MRV_DH_backtrack_count();
        end = chrono::high_resolution_clock::now();
        timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "MRV+DH优化(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    } else {
        cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    }

    // // DH+向前探测优化
    // g = graph;
    // start = chrono::high_resolution_clock::now();
    // success = g.DH_FC_backtrack(1);
    // end = chrono::high_resolution_clock::now();

    // timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    // cout << "DH+向前探测优化: ";
    // if (success) {
    //     cout << "成功，用时 " << timeTaken << " 微秒" << endl;

    //     g = graph;
    //     start = chrono::high_resolution_clock::now();
    //     int count = g.DH_FC_backtrack_count(1);
    //     end = chrono::high_resolution_clock::now();
    //     timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    //     cout << "DH+向前探测优化(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    // } else {
    //     cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    // }

    // MRV+DH+向前探测优化
    g = graph;
    start = chrono::high_resolution_clock::now();
    success = g.MRV_DH_FC_backtrack();
    end = chrono::high_resolution_clock::now();

    timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "MRV+DH+向前探测优化: ";
    if (success) {
        cout << "成功，用时 " << timeTaken << " 微秒" << endl;

        g = graph;
        start = chrono::high_resolution_clock::now();
        int count = g.MRV_DH_FC_backtrack_count();
        end = chrono::high_resolution_clock::now();
        timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "MRV+DH+向前探测优化(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    } else {
        cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    }

    // MRV+DH+向前探测优化+颜色轮询
    g = graph;
    start = chrono::high_resolution_clock::now();
    success = g.MRV_DH_FC_CC_backtrack();
    end = chrono::high_resolution_clock::now();

    timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "MRV+DH+向前探测优化+颜色轮询: ";
    if (success) {
        cout << "成功，用时 " << timeTaken << " 微秒" << endl;

        g = graph;
        start = chrono::high_resolution_clock::now();
        int count = g.MRV_DH_FC_CC_backtrack_count();
        end = chrono::high_resolution_clock::now();
        timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "MRV+DH+向前探测优化+颜色轮询(总数): " << count << ", 用时 " << timeTaken << " 微秒" << endl;
    } else {
        cout << "失败，用时 " << timeTaken << " 微秒" << endl;
    }

    cout << "----------------------------------------" << endl;
    
}

int main() {

    // testAlgorithm("small_data.col", 4);
    // testAlgorithm("le450_25a.col", 25);
    // testAlgorithm("le450_5a.col", 5);
    testAlgorithm("le450_15b.col", 15);
    // testAlgorithm("300_250.col",  6);
    // testAlgorithm("300_500.col",  6);
    // testAlgorithm("300_750.col",  6);
    // testAlgorithm("300_1000.col", 6);
    // testAlgorithm("300_1250.col", 6);
    // testAlgorithm("300_1500.col", 6);
    // testAlgorithm("300_1750.col", 6);
    // testAlgorithm("300_2000.col", 6);
    // testAlgorithm("300_2250.col", 6);
    // testAlgorithm("300_2500.col", 6);

    //testAlgorithm("250_1250.col", 6);
    //testAlgorithm("500_2500.col",   6);
    //testAlgorithm("750_3750.col",   6);
    //testAlgorithm("1000_5000.col",  6);
    //testAlgorithm("1250_6250.col",  6);
    //testAlgorithm("1500_7500.col",  6);
    //testAlgorithm("1750_8750.col",  6);
    //testAlgorithm("2000_10000.col", 6);
    //testAlgorithm("2250_11250.col", 6);
    //testAlgorithm("2500_12500.col", 6);

    return 0;
}