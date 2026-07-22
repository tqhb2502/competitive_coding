#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

vector<long long> subset_sums(const vector<long long>& values) {
    vector<long long> sums{0};
    sums.reserve(1ULL << values.size());
    for (long long value : values) {
        const size_t current_size = sums.size();
        for (size_t i = 0; i < current_size; ++i) {
            sums.push_back(sums[i] + value);
        }
    }
    return sums;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> weights(n);
    long long total = 0;
    for (long long& weight : weights) {
        cin >> weight;
        total += weight;
    }

    const int half = n / 2;
    const vector<long long> left_values(weights.begin(), weights.begin() + half);
    const vector<long long> right_values(weights.begin() + half, weights.end());
    const vector<long long> left_sums = subset_sums(left_values);
    vector<long long> right_sums = subset_sums(right_values);
    sort(right_sums.begin(), right_sums.end());

    long long best = numeric_limits<long long>::max();
    for (long long left_sum : left_sums) {
        const long long key = total - 2 * left_sum;
        const auto position = lower_bound(
            right_sums.begin(), right_sums.end(), key,
            [](long long right_sum, long long target) {
                return 2 * right_sum < target;
            });

        if (position != right_sums.end()) {
            best = min(best, llabs(key - 2 * *position));
        }
        if (position != right_sums.begin()) {
            const auto previous = position - 1;
            best = min(best, llabs(key - 2 * *previous));
        }
    }

    cout << best << '\n';
    return 0;
}
