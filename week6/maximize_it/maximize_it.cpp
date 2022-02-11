// Basic usage of CGAL LP
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

static ET floor_to_double(const CGAL::Quotient<ET>& x) {
  ET a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

static ET ceil_to_double(const CGAL::Quotient<ET>& x) {
  ET a = std::ceil(CGAL::to_double(x));
  while (a - 1 >= x) a -= 1;
  while (a < x) a += 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while (true) {
    int p, a, b;
    cin >> p;
    if (p == 0) break;
    cin >> a >> b;
    
    Program lp;
    const int X = 0; 
    const int Y = 1;
    const int Z = 2;
    
    if (p == 1) {
      
      lp = Program(CGAL::SMALLER, true, 0, false, 0); 
      lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);      //  x +  y <= 4
      lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);  // 4x + 2y <= ab
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);      // -x +  y <= 1
      
      lp.set_c(Y, -b);   
      lp.set_c(X, a);
      
    } else {
      
      lp = Program(CGAL::LARGER, false, 0, true, 0); 
      lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_a(Z, 0, 0); lp.set_b(0, -4);      //  x +  y >= -4
      lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b);  // 4x + 2y >= -ab
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_a(Z, 2, 0); lp.set_b(2, -1);      // -x +  y >= -1
      
      lp.set_c(X, a);
      lp.set_c(Y, b);   
      lp.set_c(Z, 1);   
      
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
      cout << "no" << endl;
    } else if (s.is_unbounded()) { 
      cout << "unbounded" << endl;
    } else {
      if (p == 1) {
        cout << floor_to_double(-s.objective_value()) << endl;
      } else {
        cout << ceil_to_double(s.objective_value()) << endl;
      }
    }
    
  }
  
  return 0;
}