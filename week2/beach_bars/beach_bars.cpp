// Idea: simple iteration. Test all positions on the X axis. Maintain a deque
//       of parasols within 100m of the location.
#include<iostream>
#include<cstring>
#include<vector>
#include<deque>
#include<algorithm>
using namespace std;

#define MAX_N 1000000
#define MAX_X 1000000

int get_max_distance(int loc, const deque<int> &q) {
  if (q.empty()) return MAX_X;
  return max(abs(q.front() - loc), abs(q.back() - loc));
}

void work_on_test_case() {
  int n;
  cin >> n;
  
  vector<int> x;
  x.reserve(n);
  for (int i = 0; i < n; i++) {
    int t;
    cin >> t;
    x.emplace_back(t);
  }
  
  sort(x.begin(), x.end());
  
  deque<int> q;
  int loc = -MAX_X;
  int next_i = 0;
  
  // First loc
  while (next_i < n && x[next_i] <= loc + 100) {
    q.emplace_back(x[next_i]);
    next_i++;
  }
  
  size_t optimal_count = q.size();
  int optimal_dist = get_max_distance(loc, q);
  vector<int> optimal_loc({loc});
  
  for (loc = -MAX_X + 1; loc <= MAX_X; loc++) {
    
    if (next_i < n && x[next_i] <= loc + 100) {
      q.emplace_back(x[next_i]);
      next_i++;
    }
    
    if (!q.empty() && q.front() < loc - 100) {
      q.pop_front();
    }
    
    if (q.size() > optimal_count) {
      
      optimal_count = q.size();
      optimal_dist = get_max_distance(loc, q);
      optimal_loc.clear();
      optimal_loc.emplace_back(loc);
      
    } else if (q.size() == optimal_count) {
      
      int dist = get_max_distance(loc, q);
      
      if (dist < optimal_dist) {
        
        optimal_dist = dist;
        optimal_loc.clear();
        optimal_loc.emplace_back(loc);
        
      } else if (dist == optimal_dist) {
        
        optimal_loc.emplace_back(loc);
        
      } 
      
    }
  }
  
  cout << optimal_count << " " << optimal_dist << endl;
  for (auto loc : optimal_loc) {
    cout << loc << " ";
  }
  cout << endl;
}

int main() {
  ios::sync_with_stdio(false);
  
  int t;
  cin >> t;
  for (int g = 0; g < t; g++) {
    work_on_test_case();
  }
  
  return 0;
}