#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;

int64_t mod_power(int64_t base, int64_t exponent) {
    int64_t result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

struct TestCase {
    int n;
    int a;
    int b;
};
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;

    vector<TestCase> tests(test_count);
    int maximum_n = 0;
    int maximum_moved = 0;
    vector<vector<int>> queries_by_moved(5001);

    for (int index = 0; index < test_count; ++index) {
        auto& [n, a, b] = tests[index];
        cin >> n >> a >> b;
        maximum_n = max(maximum_n, n);

        const int moved = a + b;
        if (moved <= n) {
            maximum_moved = max(maximum_moved, moved);
            queries_by_moved[moved].push_back(index);
        }
    }

    vector<int64_t> factorial(maximum_n + 1, 1);
    vector<int64_t> inverse_factorial(maximum_n + 1, 1);
    for (int value = 1; value <= maximum_n; ++value) {
        factorial[value] = factorial[value - 1] * value % MOD;
    }
    inverse_factorial[maximum_n] = mod_power(factorial[maximum_n], MOD - 2);
    for (int value = maximum_n; value >= 1; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value % MOD;
    }

    vector<int64_t> derangement_value(test_count, 0);
    for (const int index : queries_by_moved[0]) {
        derangement_value[index] = (tests[index].b == 0);
    }

    vector<int64_t> two_rows_ago(maximum_moved + 1, 0);
    vector<int64_t> previous(maximum_moved + 1, 0);
    vector<int64_t> current(maximum_moved + 1, 0);
    previous[0] = 1;

    for (int size = 1; size <= maximum_moved; ++size) {
        current[0] = 0;
        for (int wins = 1; wins < size; ++wins) {
            const int64_t value =
                static_cast<int64_t>(wins) * previous[wins] +
                static_cast<int64_t>(size - wins) * previous[wins - 1] +
                static_cast<int64_t>(size - 1) * two_rows_ago[wins - 1];
            current[wins] = value % MOD;
        }
        current[size] = 0;

        for (const int index : queries_by_moved[size]) {
            derangement_value[index] = current[tests[index].b];
        }

        two_rows_ago.swap(previous);
        previous.swap(current);
    }

    for (int index = 0; index < test_count; ++index) {
        const auto [n, a, b] = tests[index];
        const int moved = a + b;
        if (moved > n) {
            cout << 0 << '\n';
            continue;
        }

        int64_t choose = factorial[n] * inverse_factorial[moved] % MOD;
        choose = choose * inverse_factorial[n - moved] % MOD;
        int64_t answer = factorial[n] * choose % MOD;
        answer = answer * derangement_value[index] % MOD;
        cout << answer << '\n';
    }
}
