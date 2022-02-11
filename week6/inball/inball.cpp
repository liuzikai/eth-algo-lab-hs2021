// Idea: linear programming. Distance from point (x, y) to a line ax + by + c 
//       is ax + by + c / sqrt(a^2 + b^2).
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

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


int main()  {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while (true) {
    int n, d;
    cin >> n;
    if (n == 0) break;
    cin >> d;
    
    Program lp(CGAL::SMALLER, false, 0, false, 0); 
    
    for (int i = 0; i < n; i++) {
      long x = 0;
      
      for (int j = 0; j < d; j++) {
        int a;
        cin >> a;
        lp.set_a(j, i, a);
        x += a * a;
      }
      
      lp.set_a(d, i, (int) std::sqrt(x));  // coefficient of r
      
      int b;
      cin >> b;
      lp.set_b(i, b);
    }
    
    // r >= 0 (-r <= 0)
    lp.set_a(d, n, -1);
    lp.set_b(n, 0);
    
    lp.set_c(d, -1);  // minimize -r (maximize r)
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
      cout << "none" << endl;
    } else if (s.is_unbounded()) {
      cout << "inf" << endl;
    } else {
      cout << std::floor(CGAL::to_double(-s.objective_value())) << endl;
    }
    
  }
  
  
  
  return 0;
}