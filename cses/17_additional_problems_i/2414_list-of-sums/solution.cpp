#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

constexpr long long MAX_VALUE = 1'000'000'000;

// Thử dựng lại dãy A khi đã biết phần tử nhỏ nhất "first" = a1.
// Trả về true nếu multiset các tổng khớp hoàn toàn với một dãy hợp lệ.
bool reconstruct(const vector<long long> &sums, int n, long long first,
                 vector<long long> &answer) {
    if (first <= 0 || first > MAX_VALUE) {
        return false;
    }

    // multiset chứa các tổng chưa dùng.
    multiset<long long> remaining(sums.begin(), sums.end());
    answer.clear();
    answer.push_back(first);

    // Tham lam: mỗi bước lấy phần tử tiếp theo nhỏ nhất của A.
    while (static_cast<int>(answer.size()) < n) {
        if (remaining.empty()) {
            return false;
        }
        // Tổng nhỏ nhất còn lại luôn là a1 + x, nên x = tổng nhỏ nhất - a1.
        const long long nextValue = *remaining.begin() - first;
        // Loại nếu x không hợp lệ hoặc phá vỡ thứ tự tăng dần.
        if (nextValue <= 0 || nextValue > MAX_VALUE || nextValue < answer.back()) {
            return false;
        }
        // Xóa các tổng của x với mọi phần tử đã dựng.
        for (long long value : answer) {
            const auto found = remaining.find(value + nextValue);
            if (found == remaining.end()) {
                return false;
            }
            remaining.erase(found);
        }
        answer.push_back(nextValue);
    }
    // Dãy hợp lệ khi và chỉ khi mọi tổng đều đã được dùng hết.
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
    // S[0] = a1 + a2, S[1] = a1 + a3; ứng viên cho a2 + a3 nằm trong n phần tử đầu.
    const int candidateCount = min(n, sumCount);
    for (int index = 2; index < candidateCount; ++index) {
        // Từ a1 + a2, a1 + a3 và a2 + a3 suy ra 2*a1.
        const long long twiceFirst = sums[0] + sums[1] - sums[index];
        if (twiceFirst <= 0 || twiceFirst % 2 != 0) {
            continue;
        }
        // Thử dựng lại với a1 = twiceFirst / 2; in nghiệm đầu tiên hợp lệ.
        if (reconstruct(sums, n, twiceFirst / 2, answer)) {
            for (long long value : answer) {
                cout << value << ' ';
            }
            cout << '\n';
            return 0;
        }
    }
}
