// Idea: DP. When one takes the own meeple, minimize the step count. Otherwise,
//       maximize the step count.
#include<iostream>
#include<unordered_map>
#include<vector>
#include<cstring>
using namespace std;

#define MAX_N 100010
#define INVALID 0x3F3F3F3F

int n, m, r, b;
vector<vector<int>> R;  // reverse graph

int w[MAX_N][2];

void work_on_test_case() {
  cin >> n >> m >> r >> b;
  
  R.clear();
  R.resize(n + 1);
  for (int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    R[v].emplace_back(u);
  }
  
  
  memset(w, 0x3F, sizeof(w));
  
  w[n][0] = w[n][1] = 0;
  for (int x = n; x >= 1; x--) {
    for (int k = 0; k < 2; k++) {
      if (k == 0) {
        for (int i : R[x]) {
          if (w[i][0] == INVALID || w[i][0] > w[x][1] + 1) {
            w[i][0] = w[x][1] + 1;
          }
        }
      } else {
        for (int i : R[x]) {
          if (w[i][1] == INVALID || w[i][1] < w[x][0] + 1) {
            w[i][1] = w[x][0] + 1;
          }
        }
      }
    }
  }
  
  cout << ((((w[r][0] - 1) / 2) * 4 + ((w[r][0] - 1) % 2) * 3 < 1 + ((w[b][0] - 1) / 2) * 4 + ((w[b][0] - 1) % 2) * 1) ? 0 : 1) << endl;
  
}

int main() {
  ios::sync_with_stdio(false);
  
  int t; cin >> t;
  for (int g = 0; g < t; g++) {
    work_on_test_case();
  }
  
  return 0;
}