// Idea: DP, infinite backpack. The optimal solution does not necessarily takes
//       k volume, but must be < k + max{volume}. Therefore the problem becomes
//       infinite backpack (every item has ulimited amount) of k + max{volume}.
//
//       dp[i][v]: optimial solution (min cost max types) using drinks 0..i
//       and filling up volume v;
//       dp[i][v] = best{ dp[i - 1][v] (not using drink i),
//                        dp[i - 1][v - v_i] + c_i cost & 1 types (use one drink i)
//                        dp[i][v - v_i] + c_i cost (use one MORE drink i) };
//       The key here is to make use of dp[i][v - v_i], instead of trying 
//       different counts of drink i.
//
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

#define MAX_COST 100000000

struct Solution {
  int cost = MAX_COST;
  int types = 0;
};

static void update(Solution &s, int new_cost, int new_types) {
  if (new_cost < s.cost || (new_cost == s.cost && new_types > s.types)) {
    s.cost = new_cost;
    s.types = new_types;
  }
}

void testcase() {
  int n, k;
  cin >> n >> k;
  
  vector<pair<int, int>> drinks;  // cost, volume
  drinks.reserve(n);
  int max_v = 0;
  for (int i = 0; i < n; i++) {
    int c, v;
    cin >> c >> v;
    drinks.emplace_back(c, v);
    if (v > max_v) max_v = v;
  }
  
  typedef vector<Solution> SolutionV;
  vector<SolutionV> dp(2, SolutionV(k + max_v));  // cost, types (default MAX)
  dp[1][0] = {0, 0};
  for (int i = 0; i < n; i++) {
    auto &dp_cur = dp[i & 1];
    auto &dp_prev = dp[!(i & 1)];
    
    int cost = drinks[i].first;
    int vol = drinks[i].second;
    
    for (int v = 0; v < k + max_v; v++) {
      auto &cur = dp_cur[v];
      cur = dp_prev[v];
      if (v >= vol) {
        update(cur, dp_prev[v - vol].cost + cost, dp_prev[v - vol].types + 1);
        update(cur, dp_cur[v - vol].cost + cost, dp_cur[v - vol].types);
      }
    }
  }
  
  auto &dp_cur = dp[(n - 1) & 1];
  Solution result = dp_cur[k];
  for (int v = k + 1; v < k + max_v; v++) {
    update(result, dp_cur[v].cost, dp_cur[v].types);
  }
  
  cout << result.cost << ' ' << result.types << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}