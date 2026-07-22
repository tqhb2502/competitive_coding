#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> values(static_cast<size_t>(n));
    int maximum = 0;
    for (int& value : values) {
        cin >> value;
        maximum = max(maximum, value);
    }

    vector<int> frequency(static_cast<size_t>(maximum) + 1, 0);
    for (int value : values) {
        ++frequency[static_cast<size_t>(value)];
    }

    for (int divisor = maximum; divisor >= 1; --divisor) {
        int divisible_count = 0;
        for (int multiple = divisor; multiple <= maximum; multiple += divisor) {
            divisible_count += frequency[static_cast<size_t>(multiple)];
            if (divisible_count >= 2) {
                cout << divisor << '\n';
                return 0;
            }
        }
    }
}
