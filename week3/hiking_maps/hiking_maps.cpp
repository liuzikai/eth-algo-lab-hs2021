// Idea: sliding window + inbound checks using orientation test.
// Note: must use EPIC (therefore cannot check inbound with CGAL intersection).
//       See the comments at the end of this file as well as the one in
//       hiking_maps_epec.cpp for a run time comparision by only changing
//       the kernel. EPIC is ~2-3x faster than EPEC.
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
using std::cin;
using std::cout;
using std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

static bool check_inbound(const P &u, const P *ps) {
  for (int j = 0; j < 6; j += 2) {
    if (CGAL::orientation(ps[j], ps[j + 1], u) == CGAL::RIGHT_TURN) {
      return false;
    }
  }
  return true;
}

static void testcase() {
  int n, m;
  cin >> m >> n;
  
  std::vector<P> points;
  points.reserve(m);
  for (int i = 0; i < m; i++) {
    int x, y;
    cin >> x >> y;
    points.emplace_back(x, y);
  }
  
  
  std::vector<int> last(m - 1, -1);
  int l;
  int ans = n;
  {
    
    for (int u = 0; u < n; u++) {
      
      P ps[6];
      for (int j = 0; j < 6; j++) {
        int x, y;
        cin >> x >> y;
        ps[j] = P(x, y);
      }
      
      for (int j = 0; j < 6; j += 2) {
        if (CGAL::orientation(ps[j], ps[j + 1], ps[(j + 2) % 6]) != CGAL::LEFT_TURN) {
          std::swap(ps[j], ps[j + 1]);
        }
      }
      
      bool u_inbounded = check_inbound(points[0], ps);
      for (int s = 1; s < m; s++) {
        bool v_inbounded = check_inbound(points[s], ps);
        if (u_inbounded && v_inbounded) {
          last[s - 1] = u;
        }
        u_inbounded = v_inbounded;
      }
      
      int min_last = n;
      for (const auto &i : last) {
        if (i < min_last) {
          min_last = i;
          if (min_last < 0) break;
        }
      }
      
      l = min_last;
      if (l >= 0 && ans > u - l + 1) {
        ans = u - l + 1;
      } 
    }
  }
  
  cout << ans << endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}

/*

Compiling: successful

Testing solution >>>>
   Test set 1 (20 pts / 0.400 s) : Correct answer      (0.003s)
   Test set 2 (20 pts / 0.400 s) : Correct answer      (0.006s)
   Test set 3 (20 pts / 0.400 s) : Correct answer      (0.05s)
   Test set 4 (20 pts / 0.400 s) : Correct answer      (0.14s)
   Test set 5 (20 pts / 0.400 s) : Correct answer      (0.089s)

Total score: 100

 */ 