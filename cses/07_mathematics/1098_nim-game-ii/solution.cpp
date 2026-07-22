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
        while (n--) {
            long long heap;
            cin >> heap;
            nim_sum ^= heap % 4;
        }
        cout << (nim_sum == 0 ? "second" : "first") << '\n';
    }
}
