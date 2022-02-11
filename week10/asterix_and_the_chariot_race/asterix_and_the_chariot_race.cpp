// Idea: DP. There are only n - 1 edges while all nodes (cities) are reachable,
//       so every edge must have a unique end point (equivalently, every node
//       has a unique parent), and the graph is a DAG.
// Formulation: dp(u, {Y, M, N}) - minimal cost to save all reachable cities 
//              from u (including u) from disgrace, where 
//              Y: u is fixed
//              M: u is not fixed and none of u's children is fixed. Therefore 
//                 u's parent must be fixed. 
//              N: u is not fixed but some of u's children are fixed, so 
//                 u's parent need not necessarily be fixed.
//              dp(u, Y) = c[u] + sum(min{dp(v, Y), dp(v, M), dp(v, N)}) for every u->v
//              dp(u, M) = sum(dp(v, N)) for every u->v
//              dp(u, Y) = sum(min{dp(v, Y), dp(v, N)}) for every u->v and one of v must be chosen as Y
// Evaluated on the reversed DAG order.

#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

const long MAX_N = 100000L;
const long MAX_RESULT = (MAX_N * (1 << 15));

int c[MAX_N];
int order[MAX_N];

long dp[MAX_N][3];
const int Y = 0;
const int M = 1;
const int N = 2;

void testcase() {
  int n;
  cin >> n;
  
  vector<vector<int>> to(n, vector<int>());
  
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    cin >> u >> v;
    to[u].emplace_back(v);
  }
  
  for (int i = 0; i < n; i++) {
    cin >> c[i];
  }
  
  std::vector<int> order;
  order.reserve(n);
  
  std::vector<int> timestamp(n, -1);
  std::queue<int> Q; 
  timestamp[0] = 0;
  Q.push(0);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    
    order.emplace_back(u);
    
    for (int v : to[u]) {
      if (timestamp[v] != -1) continue;
      timestamp[v] = timestamp[u] + 1;
      Q.push(v);
    }
  }
  
  for (int i = n - 1; i >= 0; i--) {
    const int u = order[i];
    if (to[u].empty()) {
      dp[u][Y] = c[u];
      dp[u][M] = 0;
      dp[u][N] = MAX_RESULT;
    } else {
      long sum_min_Y_M_N = 0, sum_N = 0, min_Y_replace = MAX_RESULT, sum_min_Y_N = 0;
      for (int v : to[u]) {
        long min_Y_N = std::min(dp[v][Y], dp[v][N]);
        sum_min_Y_M_N += std::min(min_Y_N, dp[v][M]);
        sum_N += dp[v][N];
        min_Y_replace = std::min(min_Y_replace, dp[v][Y] - min_Y_N);
        sum_min_Y_N += min_Y_N;
      }
      dp[u][Y] = sum_min_Y_M_N + c[u];
      dp[u][M] = sum_N;
      dp[u][N] = sum_min_Y_N + min_Y_replace;
    }
    // cout << u << ": " << dp[u][Y] << " " << dp[u][M] << " " << dp[u][N] << endl;
  }
  
  cout << std::min(dp[0][Y], dp[0][N]) << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}