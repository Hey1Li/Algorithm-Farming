/*
This is a solution to the Tom Jerry Problem:
https://www.careercup.com/question?id=5749266532270080

This is a good demo, since it synthesizes DFS, DP, shortest path algorithms and
bit masking, being really challenging.

In the following program, a cell with value 1 denotes block, 2 denotes cheese.

Examples inputs:
3 0 0
0 1 2
0 1 0
2 0 0

5 0 1
0 0 1 2 1
0 0 1 0 1
2 0 2 0 0
0 2 0 0 0
2 1 0 2 1

10 0 0
0 0 1 2 1 0 0 0 0 2
0 0 1 0 1 0 1 1 1 1
1 0 1 0 1 0 0 0 0 0
0 0 1 0 1 0 0 0 0 2
0 1 0 2 1 0 0 0 0 0
0 1 0 0 0 0 0 0 0 0
0 0 0 0 0 2 2 2 0 0
0 0 0 0 0 0 0 2 0 0
1 1 1 1 0 0 0 2 0 0
2 0 0 0 0 0 0 0 0 0
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <queue>
#include <bitset>
using namespace std;

vector<int> cheese;
vector<vector<int>> dist;
vector<vector<int>> dp;

void locate(vector<vector<int>>& mat) {
    int n = mat.size();
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (mat[i][j] == 2)
        cheese.push_back(i * n + j);
}

vector<int> dijkstra(vector<vector<int>>& mat, int sr, int sc) {
    int n = mat.size(), start = sr * n + sc;
    vector<int> label(n * n, INT_MAX);
    label[start] = 0;
    auto cmp = [&](int x, int y){ return label[x] > label[y]; };
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
    pq.push(start);
    while (!pq.empty()) {
        int t = pq.top();
        pq.pop();
        int r = t / n, c = t % n;
        vector<vector<int>> nb{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < 4; i++) {
            int ri = r + nb[i][0], ci = c + nb[i][1], next = ri * n + ci;
            bool bounded = ri >= 0 && ri < n && ci >= 0 && ci < n;
            if (bounded && mat[ri][ci] != 1 && label[next] > label[t] + 1) {
                label[next] = label[t] + 1;
                pq.push(next);
            }
        }
    }
    vector<int> ans;
    for (int &i : cheese) ans.push_back(label[i]);
    return ans;
}

void apsp(vector<vector<int>>& mat, int jr, int jc) {
    int n = mat.size();
    dist.push_back(dijkstra(mat, 0, 0));
    for (int &e : cheese) dist.push_back(dijkstra(mat, e / n, e % n));
    dist.push_back(dijkstra(mat, jr, jc));
}

int tsp(int last, int tour) {
    if (dp[last][tour]) {
        return dp[last][tour];
    }
    int prev = tour & ~(1 << last);
    if (!prev) {
        return dp[last][tour] = dist.front()[last];
    }
    int m = cheese.size(), ans = INT_MAX;

    int t = -1, p = -1;
    for (int i = 0; i < m; i++) {
        if (!(prev & (1 << i))) continue;
        ans = min(ans, tsp(i, prev) + dist[last + 1][i]);
    }
    return dp[last][tour] = ans;
}

int min_steps(vector<vector<int>>& mat, int jr, int jc) {
    locate(mat);
    apsp(mat, jr, jc);

    cout << endl;
    for (auto &v : dist) {
        for (auto &e : v) {
            cout << e << ' ';
        }
        cout << endl;
    }
    cout << endl;

    int m = cheese.size();
    dp.resize(m, vector<int>(1 << m));
    int ans = INT_MAX;
    for (int i = 0; i < m; i++)
        ans = min(ans, tsp(i, (1 << m) - 1) + dist.back()[i]);
    return ans;
}

int main() {
    int n, jr, jc;
    cin >> n >> jr >> jc;
    vector<vector<int>> mat(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    cout << endl;
    cout << min_steps(mat, jr, jc) << endl;
    return 0;
}