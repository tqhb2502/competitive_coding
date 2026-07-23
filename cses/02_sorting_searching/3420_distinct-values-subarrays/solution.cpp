#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // last[value] = vị trí xuất hiện gần nhất của giá trị (hash map).
    unordered_map<int, int> lastPosition;
    lastPosition.reserve(2 * n);
    lastPosition.max_load_factor(0.7F);

    int left = 0;             // con trỏ trái nhỏ nhất giữ cho cửa sổ phân biệt
    long long answer = 0;     // dùng long long vì đáp án có thể ~ 2*10^10
    for (int right = 0; right < n; ++right) {
        int value;
        cin >> value;
        // Nếu value đã từng xuất hiện, dời left qua vị trí đó để loại phần tử trùng.
        const auto found = lastPosition.find(value);
        if (found != lastPosition.end()) {
            left = max(left, found->second + 1);
        }
        // Mọi subarray kết thúc tại right, bắt đầu trong [left, right] đều phân biệt.
        answer += right - left + 1;
        lastPosition[value] = right;
    }

    cout << answer << '\n';
    return 0;
}
