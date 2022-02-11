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

    set<int> s;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        if (a == 0) {
            s.insert(b);
        } else {
            s.erase(b);
        }
    }

    if (s.empty()) {
        cout << "Empty" << endl;
    } else {
        for (const auto &b : s) {
            cout << b << " ";
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