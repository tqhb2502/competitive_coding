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
    // Sắp xếp kỹ năng tăng dần để quét theo thứ tự
    sort(skill.begin(), skill.end());

    // dp[open][penalty]: số cách với open đội đang mở và tổng phạt tích lũy
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
        // Khoảng cách kỹ năng so với người liền trước
        const int difference = index == 0 ? 0 : skill[index] - skill[index - 1];

        for (int open = 0; open <= index; ++open) {
            // Mọi đội đang mở kéo dài biên phải qua khoảng difference
            const int addedPenalty = open * difference;
            for (int penalty = 0; penalty + addedPenalty <= maximumPenalty;
                 ++penalty) {
                const long long ways = current[position(open, penalty)];
                if (ways == 0) {
                    continue;
                }
                const int newPenalty = penalty + addedPenalty;

                // Gộp hai lựa chọn: tạo đội một người, hoặc thêm vào đội mở và
                // vẫn để đội đó mở; hệ số open + 1
                long long& same = next[position(open, newPenalty)];
                same = (same + ways * (open + 1LL)) % MODULO;

                // Bắt đầu một đội mở mới: open tăng 1
                long long& start = next[position(open + 1, newPenalty)];
                start += ways;
                if (start >= MODULO) {
                    start -= MODULO;
                }

                // Thêm vào rồi đóng một trong open đội: open giảm 1
                if (open > 0) {
                    long long& close = next[position(open - 1, newPenalty)];
                    close = (close + ways * open) % MODULO;
                }
            }
        }
        current.swap(next);
    }

    // Đáp án: tổng các trạng thái không còn đội mở với mọi cost <= x
    long long answer = 0;
    for (int penalty = 0; penalty <= maximumPenalty; ++penalty) {
        answer += current[position(0, penalty)];
        if (answer >= MODULO) {
            answer -= MODULO;
        }
    }
    cout << answer << '\n';
}
