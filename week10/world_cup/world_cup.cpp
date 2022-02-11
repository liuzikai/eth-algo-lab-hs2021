// Idea: linear programming + set disjoin.
// Variable: flow between each warehouse-stadium pair (m * n)
// Inequalities: supply limits, alcohol limits, demands (n + 3m)
// Maximize: revenue
// The road between every pair of w and s traverses each contour line at most 
// once => a contour lies between w and s but enclose neither does not count!
// The path bypasses it.
// If w and s are enclosed by a contour, the path does not cross the contour.
// Assume w is enclosed by a set of contours W, s by S, contours being crossed 
// are: W disjoin S. Use squared distance test to find W and S.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

#define MAX_N 200
#define MAX_M 20
#define F(i, j) ((i) * m + (j))

int a[MAX_N];
int r[MAX_N][MAX_M];  // store r * 100

void testcase() {
  int n, m, c;
  cin >> n >> m >> c;
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp(CGAL::SMALLER, true, 0, false, 0); 
  int eq_cnt = 0;
  
  vector<K::Point_2> n_points;
  n_points.reserve(n);
  for (int i = 0; i < n; i++) {
    int x, y, s;
    cin >> x >> y >> s >> a[i];
    
    n_points.emplace_back(x, y);
    
    // Supply limit
    for (int j = 0; j < m; j++) {
      lp.set_a(F(i, j), eq_cnt, 1);
    }
    lp.set_b(eq_cnt, s);
    eq_cnt++;
  }
  
  vector<K::Point_2> m_points;
  m_points.reserve(m);
  for (int j = 0; j < m; j++) {
    int x, y, d, u;
    cin >> x >> y >> d >> u;
    
    m_points.emplace_back(x, y);
    
    // Pure alcohol limit
    for (int i = 0; i < n; i++) {
      lp.set_a(F(i, j), eq_cnt, a[i]);
    }
    lp.set_b(eq_cnt, 100 * u);
    eq_cnt++;
    
    // Demand limit (<= and >=)
    for (int i = 0; i < n; i++) {
      lp.set_a(F(i, j), eq_cnt, 1);
      lp.set_a(F(i, j), eq_cnt + 1, -1);
    }
    lp.set_b(eq_cnt, d);
    lp.set_b(eq_cnt + 1, -d);
    eq_cnt += 2;
  }
  
  // Maxmize revenue (minimize negation)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> r[i][j];
      r[i][j] *= 100;
    }
  }
  
  vector<vector<int>> n_layers(n, vector<int>());
  vector<vector<int>> m_layers(m, vector<int>());
  for (int k = 0; k < c; k++) {
    int x, y, r;
    cin >> x >> y >> r;
    
    K::FT r2 = K::FT(r) * K::FT(r);
    K::Point_2 center(x, y);
    
    for (int i = 0; i < n; i++) {
      if (CGAL::squared_distance(center, n_points[i]) < r2) {
        n_layers[i].emplace_back(k);
      }
    }
    
    for (int j = 0; j < m; j++) {
      if (CGAL::squared_distance(center, m_points[j]) < r2) {
        m_layers[j].emplace_back(k);
      }
    }
  }
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      
      // n_layers[i] and m_layers[j] are ordered, find diff using parallel runs
      int diff = 0;
      auto it1 = n_layers[i].cbegin();
      auto it2 = m_layers[j].cbegin();
      while (it1 != n_layers[i].cend() && it2 != m_layers[j].cend()) {
        if (*it1 < *it2) {
          ++diff;
          ++it1;
        } else if (*it1 > *it2) {
          ++diff;
          ++it2;
        } else {
          ++it1;
          ++it2;
        }
      }
      while (it1 != n_layers[i].cend()) { ++diff; ++it1; }
      while (it2 != m_layers[j].cend()) { ++diff; ++it2; }
      // cout << i << ", " << j << " diff = " << diff << endl;
      lp.set_c(F(i, j), -(r[i][j] - diff));  // 100 * r
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_infeasible()) {
    cout << "RIOT!" << endl;
  } else {
    auto x = -s.objective_value() / 100;
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    cout << a << endl;
  }
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}