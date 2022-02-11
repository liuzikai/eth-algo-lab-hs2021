// Idea: given a line, another line with larger(smaller) y0 and larger(smaller)
//       slope does not intersect. If intersect, the one with slope closer to
//       0 reaches the intersection faster
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAX_N (500000 + 10)
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Segment_2 Segment_2;
typedef K::Point_2 Point_2;

int n;
bool failed[MAX_N];

struct biker {
  long long y0, y1;
  Segment_2 seg;
  Segment_2 nseg;
  
  bool a_eq_0() const { return y1 == y0; }
  bool a_ge_0() const { return y1 >= y0; }
  bool a_le_0() const { return y1 <= y0; }
};

vector<biker> bikers;
vector<int> idx;

struct idx_biker_y0_less {
  bool operator()(int a, int b) const {
    return bikers[a].y0 < bikers[b].y0;
  }
};

biker &get(int i) {
  return bikers[idx[i]];
}

// Segments whose slopes are closest to 0
Segment_2 *seg_ex[2][2][MAX_N];
// First index: forward (0 to i, inclusive), backward (i to n - 1, inclusive)
enum {
  BEFORE = 0,
  AFTER = 1
};
// Second index: a >= 0, a <= 0
enum {
  A_GE_0 = 0,
  A_LE_0 = 1
};
// Third index: index into idx

void fill_seg_ex(int d, int i_init, int i_end, int i_inc) {

  auto &b = get(i_init);
  if (b.a_ge_0()) seg_ex[d][A_GE_0][i_init] = &(b.seg);
  if (b.a_le_0()) seg_ex[d][A_LE_0][i_init] = &(b.seg);

  for (int i = i_init + i_inc; i != i_end; i += i_inc) {

    seg_ex[d][A_GE_0][i] = seg_ex[d][A_GE_0][i - i_inc];
    seg_ex[d][A_LE_0][i] = seg_ex[d][A_LE_0][i - i_inc];

    auto &b = get(i);
    if (b.a_ge_0()) {
      if (seg_ex[d][A_GE_0][i] == nullptr || CGAL::compare_slope(b.seg, *seg_ex[d][A_GE_0][i]) == CGAL::SMALLER) {
        seg_ex[d][A_GE_0][i] = &(b.seg);
      }
    }
    if (b.a_le_0()) {
      if (seg_ex[d][A_LE_0][i] == nullptr || CGAL::compare_slope(b.seg, *seg_ex[d][A_LE_0][i]) == CGAL::LARGER) {
        seg_ex[d][A_LE_0][i] = &(b.seg);
      }
    }
  }
}


void work_on_test_case() {
  
  memset(failed, 0, sizeof(failed));
  memset(seg_ex, 0, sizeof(seg_ex));
  bikers.clear();
  idx.clear();
  
  cin >> n;
  bikers.reserve(n);
  idx.reserve(n);
  for (int i = 0; i < n; i++) {
    long long y0, x1, y1;
    cin >> y0 >> x1 >> y1;
    bikers.emplace_back(
      biker{y0, y1, Segment_2{Point_2{0, y0}, Point_2{x1, y1}}, 
                    Segment_2{Point_2{0, y0}, Point_2{-x1, y1}}});
    idx.emplace_back(i);
  }
  
  sort(idx.begin(), idx.end(), idx_biker_y0_less());
  
  fill_seg_ex(BEFORE, 0, n, 1);
  fill_seg_ex(AFTER, n - 1, 0, -1);
  
  for (int i = 0; i < n; i++) {
    
    const biker &b = get(i);
    
    if (b.a_eq_0()) { 

      continue;  
      
    } else if (b.a_ge_0()) {  // actually, a > 0 since the equal case is captured above

      if (i + 1 < n) {
        if ((seg_ex[AFTER][A_GE_0][i + 1] != nullptr && CGAL::compare_slope(*seg_ex[AFTER][A_GE_0][i + 1], b.seg) == CGAL::SMALLER) ||
            (seg_ex[AFTER][A_LE_0][i + 1] != nullptr && CGAL::compare_slope(*seg_ex[AFTER][A_LE_0][i + 1], b.nseg) == CGAL::LARGER)) {

          failed[idx[i]] = true;
        }
      }

    } else {  // a < 0

      if (i - 1 >= 0) {
        if ((seg_ex[BEFORE][A_LE_0][i - 1] != nullptr && CGAL::compare_slope(*seg_ex[BEFORE][A_LE_0][i - 1], b.seg) == CGAL::LARGER) ||
            (seg_ex[BEFORE][A_GE_0][i - 1] != nullptr && CGAL::compare_slope(*seg_ex[BEFORE][A_GE_0][i - 1], b.nseg) != CGAL::LARGER)) {

          failed[idx[i]] = true;
        }
      }
    }
  }
  
  for (int i = 0; i < n; i++) {
    if (!failed[i]) cout << i << " ";
  }
  cout << endl;
  
}

int main() {
  ios::sync_with_stdio(false);
  
  int t; cin >> t;
  for (int g = 0; g < t; g++) {
    work_on_test_case();
  }
  
  return 0;
}