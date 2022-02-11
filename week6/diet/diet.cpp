// Idea: linear programming.
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
    
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0) break;
    
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    // Minimum: i * 2 (negated), maximum: i * 2 + 1
    
    for (int i = 0; i < n; i++) {
      int min, max;
      cin >> min >> max;
      lp.set_b(i * 2, -min);
      lp.set_b(i * 2 + 1, max);
    }
    
    for (int i = 0; i < m; i++) {
      int p;
      cin >> p;
      lp.set_c(i, p);
      for (int j = 0; j < n; j++) {
        int c;
        cin >> c;
        lp.set_a(i, j * 2,  -c);
        lp.set_a(i, j * 2 + 1,  c);
      }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
      cout << "No such diet." << endl;
    } else {
      cout << std::floor(CGAL::to_double(s.objective_value())) << endl;
    }
  }
  
  return 0;
}