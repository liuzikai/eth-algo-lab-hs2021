// Idea: linear programming. See comments below for expressing constraints.
//       Orthogonal line can be expressed by swapping coefficients of x and y
//       to avoid division.
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef double IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

class ConstraintAdder{
public:
  explicit ConstraintAdder(Program &lp) : lp(lp) {}
  
  void add(IT a, IT b, IT c, IT l, IT rhs) {
    lp.set_a(0, cnt, a); 
    lp.set_a(1, cnt, b); 
    lp.set_a(2, cnt, c); 
    lp.set_a(3, cnt, l); 
    lp.set_b(cnt, rhs);
    cnt++;
  }
  
private:
  Program &lp;
  int cnt = 0;
};

void testcase() {
  
  int N, M;
  double S;
  cin >> N >> M >> S;
  
  Program lp (CGAL::SMALLER, false, 0, false, 0);  // Ax <= b
  ConstraintAdder c(lp);
  
  double a_coeff = 0, x_sum = 0;
  vector<long long> Xs, Ys;
  Xs.reserve(N + M);
  Ys.reserve(N + M);
  
  // Nobel
  for (int i = 0; i < N; i++) {
    long long x, y; 
    cin >> x >> y;
    Xs.emplace_back(x); Ys.emplace_back(y);
    
    // -y * a - b <= -x
    c.add(-y, -1, 0, 0, -x);
    
    a_coeff += y;
    x_sum += x;
  }
  
  // Common
  for (int i = 0; i < M; i++) {
    long long x, y; 
    cin >> x >> y;
    Xs.emplace_back(x); Ys.emplace_back(y);
    
    // y * a + b <= x
    c.add(y, 1, 0, 0, x);
    
    a_coeff -= y;
    x_sum -= x;
  }
  
  if (CGAL::solve_linear_program(lp, ET()).is_infeasible()) {
    cout << "Yuck!" << endl;
    return;
  }
  
  if (S != -1) {
    // (sum(y_n) - sum(y_c)) * a + (N - M) * b <= S + (sum(x_n) - sum(x_c))
    c.add(a_coeff, N - M, 0, 0, S + x_sum);
  
    if (CGAL::solve_linear_program(lp, ET()).is_infeasible()) {
      cout << "Bankrupt!" << endl;
      return;
    }
  }
  
  // Constraints of maximal
  for (int i = 0; i < N + M; i++) {
    long long x = Xs[i], y = Ys[i];
    c.add(x, 0, -1, -1, -y);  //  x * a - c - l <= -y
    c.add(-x, 0, 1, -1, y);   // -x * a + c - l <= y
  }
  
  // Minimize l
  lp.set_c(3, 1);
  Solution s = CGAL::solve_linear_program(lp, ET());
  cout << (long long) std::ceil(CGAL::to_double(s.objective_value())) << endl;
  
}

int main() {
  std::ios::sync_with_stdio(false);
  
  int T; cin >> T;
  for (int i = 0; i < T; i++) testcase();
  
  return 0;
}