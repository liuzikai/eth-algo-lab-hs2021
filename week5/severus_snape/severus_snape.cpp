// Idea: DP + greedy. The answer <= 200. Fix a total number of potions cnt and 
//       a number of B cntB (cntA = cnt - cntB), the chosen B's needs to have
//       sum{w} >= W + a * cntA. It suffics to choose the first cntB B's
//       with largest w. Then the problem becomes how to check whether there
//       exists a combination of cntA A's such that sum{p} >= P + b * cntB and
//       sum{h} >= H. If p = h for every A, choosing cntA largest A's works.
//       Otherwise, greedy doesn't work. DP comes into play (explained below).
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

int n, m;
long a, b, P, H, W;
vector<pair<long, long>> A;
vector<long> B;

#define MAX_P (1 << 31)
vector<long> reachable_p;  // max{sum{p}} using index A's with sum{h} >= H

static void solveA() {
  // dp[j][h]: maximal reachable p using j A's with total happiness == h
  vector<vector<long>> dp(n + 1, vector<long>(H + 1, -1));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    const auto &a = A[i];
    for (int j = i + 1; j >= 1; j--) {
      for (long h = H + a.second; h >= a.second; h--) {
        long &cur = dp[j][std::min(H, h)];
        const long &prev = dp[j - 1][h - a.second];
        if (prev < 0) continue;
        if (cur < prev + a.first) cur = prev + a.first;
      }
    }
  }
  
  reachable_p = vector<long>(n + 1, -1);
  for (int j = n; j >= 0; j--) {
    reachable_p[j] = dp[j][H];
  }
}

static bool solve(int cnt) {
  long w = 0;
  for (int cntB = 0; cntB <= std::min(cnt, m); cntB++) {
    int cntA = cnt - cntB;
    if (cntA <= n && w >= W + a * cntA && reachable_p[cntA] >= P + b * cntB) {
      return true;
    }
    if (cntB < m) w += B[cntB];
  }
  return false;
}

void testcase() {
  cin >> n >> m >> a >> b >> P >> H >> W;
  
  A.clear();
  A.reserve(n);
  for (int i = 0; i < n; i++) {
    long p, h;
    cin >> p >> h;
    A.emplace_back(p, h);
  }
  solveA();
  
  B.clear();
  B.reserve(m);
  for (int i = 0; i < m; i++) {
    long w;
    cin >> w;
    B.emplace_back(w);
  }
  
  std::sort(B.begin(), B.end(), std::greater<long>());
  
  for (int cnt = 0; cnt <= n + m; cnt++) {
    if (solve(cnt)) {
      cout << cnt << endl;
      return;
    }
  }
  
  cout << -1 << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}