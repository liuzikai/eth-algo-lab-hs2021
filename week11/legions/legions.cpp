// Idea: linear programming. Maximize t subject to dist / v >= t. To constraint
//       the point within the region bounded by legions, test 
//       a * xs + b * ys + c. If it is > 0, constaint ax + by + c >= 0 to keep
//       the point on the same side of the line. Vice versa.
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
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int VAR_X = 0, VAR_Y = 1, VAR_T = 2;

class equation_adder {
  Program &lp;
  int count = 0;

public:
  explicit equation_adder(Program &lp) : lp(lp) {}

  void add(IT x, IT y, IT t, IT b) {
    lp.set_a(VAR_X, count, x); 
    lp.set_a(VAR_Y, count, y); 
    lp.set_a(VAR_T, count, t); 
    lp.set_b(count, b);
    count++;
  }
};

#define square(x) ((x) * (x))

void testcase() {
  IT xs, ys, n;
  cin >> xs >> ys >> n;
  
  // create an LP with Ax >= b, no lower bounds and no upper bounds
  Program lp(CGAL::LARGER, false, 0, false, 0); 
  equation_adder adder(lp);
  
  for (int i = 0; i < n; i++) {
    IT a, b, c, v;
    cin >> a >> b >> c >> v;
    
    IT k = a * xs + b * ys + c;
    if (k < 0) {
      a = -a;
      b = -b;
      c = -c;
    }
    
    adder.add(a, b, 0, -c);  // ax + by + c >= 0 (ax + by >= -c)
    
    IT d = (IT) std::sqrt(square(a) + square(b));
    adder.add(a, b, -d * v, -c);  // |ax + by + c| / (sqrt(a^2 + b^2) * v) >= t
  }
  
  lp.set_l(VAR_T, true, 0);  // t >= 0
  lp.set_c(VAR_T, -1);       // maxmize t (minimize -t)
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  auto x = -s.objective_value();
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  cout << a << endl;
}

int main()  {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}