#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
using namespace std;

void work_on_test_case() {
    int n, m, v;
    cin >> n >> m >> v;

    vector<set<int>> out_edges(n);
    vector<int> dist(n, -1);
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        out_edges[a].emplace(b);
        out_edges[b].emplace(a);
    }

    queue<int> q;
    q.emplace(v);
    dist[v] = 0;

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for (int w : out_edges[u]) {
            if (dist[w] == -1) {
                q.emplace(w);
                dist[w] = dist[u] + 1;
            }
        }
    }

    for (int t : dist) {
        cout << t << " ";
    }
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    
    int t;
    cin >> t;
    for (int g = 0; g < t; g++) {
        work_on_test_case();
    }

    return 0;
}