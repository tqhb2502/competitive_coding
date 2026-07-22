#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    long long moves = 0;
    long long previous = 0;
    for (int i = 0; i < n; ++i) {
        long long value;
        cin >> value;
        if (value < previous) {
            moves += previous - value;
        } else {
            previous = value;
        }
    }

    cout << moves << '\n';
    return 0;
}
