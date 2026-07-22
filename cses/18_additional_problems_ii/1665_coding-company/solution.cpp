#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

constexpr long long MODULO = 1'000'000'007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int coderCount = 0;
    int maximumPenalty = 0;
    cin >> coderCount >> maximumPenalty;
    vector<int> skill(coderCount);
    for (int& value : skill) {
        cin >> value;
    }
    sort(skill.begin(), skill.end());

    const int width = maximumPenalty + 1;
    const size_t stateCount = static_cast<size_t>(coderCount + 1) *
                              static_cast<size_t>(width);
    vector<long long> current(stateCount, 0);
    vector<long long> next(stateCount, 0);
    const auto position = [width](int openGroups, int penalty) {
        return static_cast<size_t>(openGroups) * static_cast<size_t>(width) +
               static_cast<size_t>(penalty);
    };
    current[position(0, 0)] = 1;

    for (int index = 0; index < coderCount; ++index) {
        fill(next.begin(), next.end(), 0);
        const int difference = index == 0 ? 0 : skill[index] - skill[index - 1];

        for (int open = 0; open <= index; ++open) {
            const int addedPenalty = open * difference;
            for (int penalty = 0; penalty + addedPenalty <= maximumPenalty;
                 ++penalty) {
                const long long ways = current[position(open, penalty)];
                if (ways == 0) {
                    continue;
                }
                const int newPenalty = penalty + addedPenalty;

                long long& same = next[position(open, newPenalty)];
                same = (same + ways * (open + 1LL)) % MODULO;

                long long& start = next[position(open + 1, newPenalty)];
                start += ways;
                if (start >= MODULO) {
                    start -= MODULO;
                }

                if (open > 0) {
                    long long& close = next[position(open - 1, newPenalty)];
                    close = (close + ways * open) % MODULO;
                }
            }
        }
        current.swap(next);
    }

    long long answer = 0;
    for (int penalty = 0; penalty <= maximumPenalty; ++penalty) {
        answer += current[position(0, penalty)];
        if (answer >= MODULO) {
            answer -= MODULO;
        }
    }
    cout << answer << '\n';
}
