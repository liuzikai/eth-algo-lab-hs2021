#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
using namespace std;

void work_on_test_case() {
    int n;
    cin >> n;

    long long even_count = 1;  // sum of no value is 0, also even
    long long odd_count = 0;
    unsigned mod = 0;
    for (int i = 0; i < n; i++) {
        unsigned x;
        cin >> x;
        mod ^= x;
        if (mod == 0) {
            even_count++;
        } else {
            odd_count++;
        }
    }

    cout << even_count * (even_count - 1) / 2 + odd_count * (odd_count - 1) / 2 << endl;
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