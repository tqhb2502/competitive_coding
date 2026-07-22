#include <iostream>
#include <unordered_map>

using namespace std;

constexpr long long MODULO = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    unordered_map<int, int> frequency;
    frequency.reserve(2 * n);
    frequency.max_load_factor(0.7F);
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        ++frequency[value];
    }

    long long answer = 1;
    for (const auto& [value, count] : frequency) {
        (void)value;
        answer = answer * (count + 1) % MODULO;
    }

    cout << (answer + MODULO - 1) % MODULO << '\n';
    return 0;
}
