#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <set>
using namespace std;

vector<set<int>> out_edges;
vector<int> visit, finish;
int timestamp;

void dfs(int v) {
    visit[v] = timestamp++;
    for (int u : out_edges[v]) {
        if (visit[u] == -1) {
            dfs(u);
        }
    }
    finish[v] = timestamp++;
}

void work_on_test_case() {
    int n, m, v;
    cin >> n >> m >> v;

    out_edges = vector<set<int>>(n);
    visit = vector<int>(n, -1);
    finish = vector<int>(n, -1);
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        out_edges[a].emplace(b);
        out_edges[b].emplace(a);
    }

    timestamp = 0;
    dfs(v);
    
    for (int t : visit) {
        cout << t << " ";
    }
    cout << endl;

    for (int t : finish) {
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