///1
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
using std::cin;
using std::cout;
using std::endl;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

static double floor_to_double(const K::FT &x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
  
  std::ios::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while (true) {
    int n;
    cin >> n;
    if (n == 0) break;
    
    long x, y, a, b;
    cin >> x >> y >> a >> b;
    
    std::vector<S> ss;
    ss.reserve(n);
    for (int i = 0; i < n; i++) {
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      ss.emplace_back(P(r, s), P(t, u));
    }
    std::random_shuffle(ss.begin(), ss.end());
    
    bool intersect = false;
    P start(x, y);
    R ray(start, P(a, b));
    S seg;
    
    for (const auto &s : ss) {
      auto o = intersect ? CGAL::intersection(seg, s) : CGAL::intersection(ray, s);
      if (o) {
        if (const P* op = boost::get<P>(&*o)) {
          seg = S(start, *op);
        } else if (const S* os = boost::get<S>(&*o)) {
          if (CGAL::collinear_are_ordered_along_line(start, os->source(), os->target())) {
            seg = S(start, os->source());
          } else {
            seg = S(start, os->target());
          }
        }
        intersect = true;
      }
    }
    
    if (intersect) {
      cout << floor_to_double(seg.target().x()) << " " << floor_to_double(seg.target().y()) << endl;
    } else {
      cout << "no" << endl;
    }
  }
  
  return 0;
}