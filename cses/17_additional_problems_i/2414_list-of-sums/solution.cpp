#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

constexpr long long MAX_VALUE = 1'000'000'000;

bool reconstruct(const vector<long long> &sums, int n, long long first,
                 vector<long long> &answer) {
    if (first <= 0 || first > MAX_VALUE) {
        return false;
    }

    multiset<long long> remaining(sums.begin(), sums.end());
    answer.clear();
    answer.push_back(first);

    while (static_cast<int>(answer.size()) < n) {
        if (remaining.empty()) {
            return false;
        }
        const long long nextValue = *remaining.begin() - first;
        if (nextValue <= 0 || nextValue > MAX_VALUE || nextValue < answer.back()) {
            return false;
        }
        for (long long value : answer) {
            const auto found = remaining.find(value + nextValue);
            if (found == remaining.end()) {
                return false;
            }
            remaining.erase(found);
        }
        answer.push_back(nextValue);
    }
    return remaining.empty();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    const int sumCount = n * (n - 1) / 2;
    vector<long long> sums(sumCount);
    for (long long &sum : sums) {
        cin >> sum;
    }
    sort(sums.begin(), sums.end());

    vector<long long> answer;
    const int candidateCount = min(n, sumCount);
    for (int index = 2; index < candidateCount; ++index) {
        const long long twiceFirst = sums[0] + sums[1] - sums[index];
        if (twiceFirst <= 0 || twiceFirst % 2 != 0) {
            continue;
        }
        if (reconstruct(sums, n, twiceFirst / 2, answer)) {
            for (long long value : answer) {
                cout << value << ' ';
            }
            cout << '\n';
            return 0;
        }
    }
}
