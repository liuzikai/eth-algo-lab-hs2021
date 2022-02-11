// Idea: split and list. Let i be the number of gulps, j be the number of moves,
//       the problem is to find the minimal i such that s[i] * j + max_dist[j] 
//       >= D, where max_dist[j] is the maximal distance with j moves that 
//       satisfies total time < T. The key is to compute max_dist[].
//       D and T are too large for DP, while n is small. 2^n is still too large.
//       Split the set of moves by half into S1 and S2. List {dist, time} pairs
//       of S1 and S2 into L1 and L2. For {d1, t1} in L1, binary search for
//       {d2, t2} such that t2 < T - t1.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

typedef pair<long, long> Move;  // distance, time
typedef vector<Move> MoveV;

static void list(MoveV &L, MoveV::const_iterator s, const MoveV::const_iterator &t, int cnt, Move cur) {
  if (cnt == 0) {
    L.emplace_back(cur);
    return;
  }
  if (s == t) return;
  list(L, s + 1, t, cnt, cur);
  list(L, s + 1, t, cnt - 1, {cur.first + s->first, cur.second + s->second});
}

// Binary search in increasing-T vector for the max distance achievable <= T
static long binary_search(const MoveV &D, long T) {
  size_t l = 0, u = D.size() - 1;
  while (l < u) {
    size_t m = (l + u + 1) / 2;
    if (D[m].second <= T) {
      l = m;
    } else {
      u = m - 1;
    }
  }
  return (D[l].second <= T ? D[l].first : -1);
}

static void testcase() {
  int n, m;
  long D, T;
  cin >> n >> m >> D >> T;
  
  MoveV moves;
  moves.reserve(n);
  for (int i = 0; i < n; i++) {
    long d, t;
    cin >> d >> t;
    moves.emplace_back(d, t);
  }
  
  vector<long> s;
  s.reserve(m);
  s.emplace_back(0);
  for (int i = 1; i <= m; i++) {
    long x;
    cin >> x;
    s.emplace_back(x);
  }
  
  int S1_size = n / 2, S2_size = n - S1_size;
  auto S1_begin = moves.begin(), S1_end = S1_begin + S1_size;
  auto S2_begin = S1_end, S2_end = moves.end();
  
  vector<MoveV> all_D2(S2_size + 1, MoveV());  // move count -> [{max dist, time}]
  for (int k2 = 0; k2 <= S2_size; k2++) {
    MoveV L2;  // {total dist, total time}
    L2.reserve(1 << k2);
    list(L2, S2_begin, S2_end, k2, {0, 0});
    std::sort(L2.begin(), L2.end(), 
      [](const Move &m1, const Move &m2) {
        return m1.second < m2.second;
      });
    
    MoveV &D2 = all_D2[k2];  
    D2.reserve(L2.size());
    long max_dist = 0;
    for (const auto l : L2) {
      if (l.first > max_dist) max_dist = l.first;
      if (!D2.empty() && D2.back().second == l.second && max_dist > D2.back().first) {
        D2.back().first = max_dist;  // keep the max for the same total time
      } else {
        D2.emplace_back(max_dist, l.second);
      }
    }
  }
  
  vector<long> max_dist(n + 1, -1);  // move count -> max_dist satisfying total time < T
  
  for (int k1 = 0; k1 <= S1_size; k1++) {
    MoveV L1;  // {total dist, total time}
    L1.reserve(1 << k1);
    list(L1, S1_begin, S1_end, k1, {0, 0});
    
    for (const auto l : L1) {
      
      for (int k2 = 0; k2 <= S2_size; k2++) {
        long L2_dist = binary_search(all_D2[k2], T - l.second - 1);
        if (L2_dist != -1) {
          max_dist[k1 + k2] = std::max(max_dist[k1 + k2], l.first + L2_dist);
        }
      }
      
    }
  }
  
  for (int i = 0; i <= m; i++) {
    for (int j = 0; j <= n; j++) {
      if (max_dist[j] != -1 && s[i] * j + max_dist[j] >= D) {
        cout << i << endl;
        return;
      }
    }
  }
  
  cout << "Panoramix captured" << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}