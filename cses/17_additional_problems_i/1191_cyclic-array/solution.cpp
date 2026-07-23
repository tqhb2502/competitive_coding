#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long limit;
    cin >> n >> limit;

    // Nhân đôi mảng để mỗi điểm cắt vòng tại i thành đoạn tuyến tính [i, i+n).
    vector<long long> values(2 * n);
    for (int i = 0; i < n; ++i) {
        cin >> values[i];
        values[i + n] = values[i];
    }

    const int doubledSize = 2 * n;

    // Two pointers: nextPosition[left] là vị trí kết thúc đoạn tham lam dài nhất
    // bắt đầu từ left mà tổng vẫn <= limit (các số dương nên tham lam là tối ưu).
    vector<int> nextPosition(doubledSize + 1, doubledSize);
    long long sum = 0;
    int right = 0;
    for (int left = 0; left < doubledSize; ++left) {
        while (right < doubledSize && sum + values[right] <= limit) {
            sum += values[right];
            ++right;
        }
        nextPosition[left] = right;
        sum -= values[left];  // bỏ phần tử left để cửa sổ trượt sang phải
    }

    // Số mức binary lifting đủ để nhảy tối đa n đoạn.
    int levels = 1;
    while ((1LL << levels) <= n) {
        ++levels;
    }

    // jump[level][p] = vị trí đạt được sau khi lấy 2^level đoạn tham lam từ p.
    vector<vector<int>> jump(levels, vector<int>(doubledSize + 1, doubledSize));
    jump[0] = nextPosition;
    for (int level = 1; level < levels; ++level) {
        for (int position = 0; position <= doubledSize; ++position) {
            jump[level][position] = jump[level - 1][jump[level - 1][position]];
        }
    }

    int answer = numeric_limits<int>::max();
    // Thử mọi điểm cắt vòng start, đếm số đoạn tối thiểu phủ [start, start+n).
    for (int start = 0; start < n; ++start) {
        const int target = start + n;
        int position = start;
        int segments = 0;
        // Nhảy các lũy thừa lớn nhất mà chưa vượt qua target rồi cộng đoạn cuối.
        for (int level = levels - 1; level >= 0; --level) {
            if (jump[level][position] < target) {
                position = jump[level][position];
                segments += 1 << level;
            }
        }
        answer = min(answer, segments + 1);
    }

    cout << answer << '\n';
}
