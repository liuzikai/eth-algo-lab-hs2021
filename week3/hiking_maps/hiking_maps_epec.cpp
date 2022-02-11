///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
using std::cin;
using std::cout;
using std::endl;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

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
      
      L edges[3];
      for (int j = 0; j < 3; j++) {
        int x0, y0, x1, y1;
        cin >> x0 >> y0 >> x1 >> y1;
        edges[j] = L(P(x0, y0), P(x1, y1));
      }
      
      P ps[3];
      for (int j = 0; j < 3; j++) {
        auto o = CGAL::intersection(edges[j], edges[(j + 1) % 3]);
        if (const P* op = boost::get<P>(&*o)) {
          ps[j] = *op;
        } else throw std::runtime_error("strange intersection");
      }
      
      T t(ps[0], ps[1], ps[2]);
      
      bool u_unbounded = t.has_on_unbounded_side(points[0]);
      for (int s = 1; s < m; s++) {
        bool v_unbounded = t.has_on_unbounded_side(points[s]);
        if (!u_unbounded && !v_unbounded) {
          last[s - 1] = u;
        }
        u_unbounded = v_unbounded;
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

Testing solution >>>>
   Test set 1 (20 pts / 0.400 s) : Correct answer      (0.014s)
   Test set 2 (20 pts / 0.400 s) : Correct answer      (0.023s)
   Test set 3 (20 pts / 0.400 s) : Time limit exceeded
   Test set 4 (20 pts / 0.400 s) : Correct answer      (0.279s)
   Test set 5 (20 pts / 0.400 s) : Correct answer      (0.188s)

Total score: 80

*/