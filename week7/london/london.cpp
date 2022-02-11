// Idea: max flow. Aggregate (front, back) combinations (26 * 26). Every piece 
//       has one flow in, two possible outs.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

#define LETTER(i) (i)
#define PIECE(f, b) (26 + (f) * 26 + (b))

void testcase() {
  int h, w;
  cin >> h >> w;
  
  string note;
  cin >> note;
  vector<int> letter_count(26, 0);
  for (char c : note) {
    letter_count[c - 'A']++;
  }
  
  vector<string> front;
  for (int i = 0; i < h; i++) {
    string line;
    cin >> line;
    front.emplace_back(std::move(line));
  }
  
  vector<int> piece_count(26 * 26, 0);
  for (int i = 0; i < h; i++) {
    string line;
    cin >> line;
    for (int j = 0; j < w; j++) {
      piece_count[(front[i][w - j - 1] - 'A') * 26 + (line[j] - 'A')]++;
    }
  }
  
  graph G(26 + 26 * 26);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for (int f = 0; f < 26; f++) {
    for (int b = 0; b < 26; b++) {
      adder.add_edge(v_source, PIECE(f, b), piece_count[f * 26 + b]);
      adder.add_edge(PIECE(f, b), LETTER(f), piece_count[f * 26 + b]);
      adder.add_edge(PIECE(f, b), LETTER(b), piece_count[f * 26 + b]);
    }
  }
  
  int sum = 0;
  for (int c = 0; c < 26; c++) {
    adder.add_edge(LETTER(c), v_sink, letter_count[c]);
    sum += letter_count[c];
  }
  
  cout << (boost::push_relabel_max_flow(G, v_source, v_sink) == sum ? "Yes" : "No") << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}