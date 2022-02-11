#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <set>
using namespace std;

void work_on_test_case() {
    int q;
    cin >> q;

    map<string, vector<int>> m;

    for (int i = 0; i < q; i++) {
        int a;
        string b;
        cin >> a >> b;
        auto it = m.find(b);
        if (a == 0) {
            if (it != m.end()) {
                m.erase(it);
            }
        } else {
            if (it == m.end()) {
                it = m.emplace(b, vector<int>()).first;
            }
            it->second.emplace_back(a);
        }
    }

    string s;
    cin >> s;
    auto it = m.find(s);
    if (it == m.end()) {
        cout << "Empty" << endl;
    } else {
        auto &v = it->second;
        std::sort(v.begin(), v.end());
        for (const auto &a : v) {
            cout << a << " ";
        }
        cout << endl;
    }
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