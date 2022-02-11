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

    long long reached_pos;  // dominoes at position >= reached_pos falls

    // The first domino must fall, reaching the position 1 + h0 - 1
    int fall_count = 1;
    cin >> reached_pos;

    for (long long current_pos = 2; current_pos <= n; current_pos++) {

        long long h;
        cin >> h;

        if (reached_pos >= current_pos) {
            reached_pos = std::max(reached_pos, current_pos + h - 1);
            fall_count++;
        }
    }

    cout << fall_count << endl;
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