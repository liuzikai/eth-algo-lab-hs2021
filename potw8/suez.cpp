// Idea: linear programming
#include <iostream>
#include <vector>
#include <climits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
using namespace std;

using IT = int;
using ET = CGAL::Gmpz;

using Program = CGAL::Quadratic_program<IT>;
using Solution = CGAL::Quadratic_program_solution<ET>;

using XY = pair<IT, IT>;

int N, M, H, W;

class ConstraintAdder {
public:

  explicit ConstraintAdder(Program &lp) : lp(lp) {}
  
  void add(IT a_index, IT a_coeff, IT b) {
    //for (int i = 0; i < N; i++) lp.set_a(i, cnt, 0);
    lp.set_a(a_index, cnt, a_coeff);
    lp.set_b(cnt, b);
    cnt++;
  }
  
  void add(IT a1_index, IT a2_index, IT a_coeff, IT b) {
    //for (int i = 0; i < N; i++) lp.set_a(i, cnt, 0);
    lp.set_a(a1_index, cnt, a_coeff);
    lp.set_a(a2_index, cnt, a_coeff);
    lp.set_b(cnt, b);
    cnt++;
  }
  
private:

  Program &lp;
  int cnt = 0;
};

void testcase() {
  
  cin >> N >> M >> H >> W;
  
  vector<XY> new_nails, old_nails;
  new_nails.reserve(N);
  old_nails.reserve(M);
  
  for (int i = 0; i < N; i++) {
    IT x, y; cin >> x >> y;
    new_nails.emplace_back(x, y);
  }
  
  for (int i = 0; i < M; i++) {
    IT x, y; cin >> x >> y;
    old_nails.emplace_back(x, y);
  }
  
  Program lp(CGAL::SMALLER, true, 1, false);
  ConstraintAdder ca(lp);
  
  for (int i = 0; i < N; i++) {
    const XY& new_nail = new_nails[i];
    
    IT x_c_upper = INT_MAX, y_c_upper = INT_MAX;
    for (int j = 0; j < M; j++) {
      const XY& old_nail = old_nails[j];
      IT abs_dx = std::abs(new_nail.first - old_nail.first);
      IT abs_dy = std::abs(new_nail.second - old_nail.second);
      if ((long long) abs_dx * H >= (long long) abs_dy * W) {
        x_c_upper = min(x_c_upper, 2 * abs_dx - W);
      } else {
        y_c_upper = min(y_c_upper, 2 * abs_dy - H);
      }
    }
    ca.add(i, W, x_c_upper);
    ca.add(i, H, y_c_upper);
    
    for (int j = i + 1; j < N; j++) {
      const XY& other_nail = new_nails[j];
      IT abs_dx = std::abs(new_nail.first - other_nail.first);
      IT abs_dy = std::abs(new_nail.second - other_nail.second);
      if ((long long) abs_dx * H >= (long long) abs_dy * W) {
        ca.add(i, j, W, 2 * abs_dx);
      } else {
        ca.add(i, j, H, 2 * abs_dy);
      }
    }
  }
  
  for (int k = 0; k < N; k++) {
    lp.set_c(k, -1);  
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert (s.solves_linear_program(lp));
  
  auto result = (long long) std::ceil(CGAL::to_double(s.objective_value()) * -2 * (W + H));
  cout << result << endl;
  
}

int main() {
  ios::sync_with_stdio(false);
  
  int T; cin >> T;
  for (int g = 0; g < T; g++) testcase();
  
  return 0;
}