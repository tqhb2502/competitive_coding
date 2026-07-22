#include <iostream>
#include <string>

using namespace std;

char digitAt(long long k) {
    long long digits = 1;
    long long count = 9;
    long long start = 1;

    while (k > digits * count) {
        k -= digits * count;
        ++digits;
        count *= 10;
        start *= 10;
    }

    const long long number = start + (k - 1) / digits;
    const int digitIndex = static_cast<int>((k - 1) % digits);
    return to_string(number)[digitIndex];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        long long k;
        cin >> k;
        cout << digitAt(k) << '\n';
    }

    return 0;
}
