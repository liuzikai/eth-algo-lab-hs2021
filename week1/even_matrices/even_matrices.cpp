#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
using namespace std;

bool mod2[210][210];

void work_on_test_case() {
    memset(mod2, 0, sizeof(mod2));

    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        unsigned row_mod2 = 0;
        for (int j = 1; j <= n; j++) {
            bool x;
            cin >> x;
            row_mod2 ^= x;
            mod2[i][j] = mod2[i - 1][j] ^ row_mod2;
        }
    }

    long long result = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            long long even = 1, odd = 0;
            for (int k = 1; k <= n; k++) {
                if ((mod2[i][k] ^ mod2[j][k]) == 0) {
                    even++;
                } else {
                    odd++;
                }
            }
            result += even * (even - 1) / 2 + odd * (odd - 1) / 2;
        }
    }

    cout << result << endl;
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