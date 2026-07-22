#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        cin >> n;
        long long nim_sum = 0;
        for (int stair = 1; stair <= n; ++stair) {
            long long balls;
            cin >> balls;
            if (stair % 2 == 0) {
                nim_sum ^= balls;
            }
        }
        cout << (nim_sum == 0 ? "second" : "first") << '\n';
    }
}
