// Idea: dynamic programming with properly encoded states. There are at most
//       4 types of fighters, which can be encoded in 2 bits. Encode the
//       types of the last two fighters from every entrance. |p - q| cannot
//       grows to infinity. A large enough |p - q| cancels out the maximum
//       possible points. There I limits |p - q| from -24 to 24.
#include <iostream>
#include <vector>
#include <set>
#include <cstring>
using std::cin;
using std::cout;
using std::endl;

#define MAX_N 5000
#define MAX_M 3
#define MAX_P_MINUS_Q 24
#define OFF(d) (d + MAX_P_MINUS_Q + 1)

int e[2][(MAX_P_MINUS_Q + 2) * 2][1 << 4][1 << 4];
// First index: rolling array

const size_t MASK[MAX_M + 1] = {0, 0x3, 0xF, 0x3F};
int DELTA[MAX_M + 1][1 << 6];

static int get_distinct_types(size_t s, int m) {
  std::set<size_t> tys;
  for (int i = 0; i < m; i++) {
    tys.insert(s & 0x3);
    s >>= 2;
  }
  return tys.size();
}

static void testcase() {
  int n, k, m;
  cin >> n >> k >> m;
  
  memset(e, 0xC0, sizeof(e));
  e[0][OFF(0)][0][0] = 0;
  
  for (int r = 1; r <= n; r++) {
    size_t x;  cin >> x;
    
    auto &cur_e = e[r & 1];
    auto &last_e = e[!(r & 1)];
    memset(cur_e, 0xC0, sizeof(cur_e));
    
    for (int diff = -MAX_P_MINUS_Q; diff <= MAX_P_MINUS_Q; diff++) {
      if ((r + diff) % 2 != 0) continue;
      
      const int p = (r + diff) / 2, q = r - p;
      if (p < 0 || q < 0) continue;
    
      if (p > 0) {
        for (size_t prev_ps = 0; prev_ps <= MASK[std::min(m, p) - 1]; prev_ps++) {
          for (size_t qs = 0; qs <= MASK[std::min(m - 1, q)]; qs++) {
            
            size_t ps = ((prev_ps << 2) | x);
            
            int &cur = cur_e[OFF(diff)][ps & MASK[std::min(m - 1, p)]][qs];
            int val = last_e[OFF(diff - 1)][prev_ps][qs] +
              DELTA[std::min(m, p)][ps] - (1 << std::abs(diff));
            
            if (val >= 0 && val > cur) cur = val;
            
            //if (val >= 0) 
            //cout << x << " P = " << p << " e[" << r << "][" << diff << "][" << ps << "][" << qs << "] = " << cur << endl;
          }
        }
      }
            
      if (q > 0) {
        for (size_t ps = 0; ps <= MASK[std::min(m - 1, p)]; ps++) {
          for (size_t prev_qs = 0; prev_qs <= MASK[std::min(m, q) - 1]; prev_qs++) {
            
            size_t qs = ((prev_qs << 2) | x);
            
            auto &cur = cur_e[OFF(diff)][ps][qs & MASK[std::min(m - 1, q)]];
            int val = last_e[OFF(diff + 1)][ps][prev_qs] +
                DELTA[std::min(m, q)][qs] - (1 << std::abs(diff));
            
            if (val >= 0 && val > cur) cur = val;
            
            //if (val >= 0) 
            //cout << x << " Q = " << q << " e[" << r << "][" << diff << "][" << ps << "][" << qs << "] = " << cur << endl;
          }
        }
      }
    }
    
  }
  
  int res = -15000000;
  for (int diff = -MAX_P_MINUS_Q; diff <= MAX_P_MINUS_Q; diff++) {
    for (size_t ps = 0; ps <= MASK[m - 1]; ps++) {
      for (size_t qs = 0; qs <= MASK[m - 1]; qs++) {
        res = std::max(res, e[n & 1][OFF(diff)][ps][qs]);
      }
    }
  }
  
  cout << res << endl;
}

int main() {
  std::ios::sync_with_stdio(false);
  
  memset(DELTA, 0xC0, sizeof(DELTA));
  for (int m = 0; m <= MAX_M; m++) {
    for (size_t s = 0; s <= MASK[MAX_M]; s++) {
      DELTA[m][s] = get_distinct_types(s, m) * 1000;
      // cout << "DELTA[" << m << "][" << s << "] = " << DELTA[m][s] << endl;
    }
  }
  
  int T;
  cin >> T;
  for (int i = 0; i < T; i++) testcase();
  
  return 0;
}