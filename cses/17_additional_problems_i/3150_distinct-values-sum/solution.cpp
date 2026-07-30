#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> values(static_cast<size_t>(n));
    for (long long& value : values) {
        cin >> value;
    }

    // Nén tọa độ các giá trị: sắp xếp bản sao rồi loại trùng lặp.
    vector<long long> coordinates = values;
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());

    // lastPosition[index] lưu vị trí xuất hiện gần nhất của giá trị đã nén.
    vector<int> lastPosition(coordinates.size(), 0);

    long long answer = 0;
    for (int position = 1; position <= n; ++position) {
        long long value = values[static_cast<size_t>(position - 1)];
        // Tìm chỉ số đã nén của giá trị hiện tại bằng tra cứu nhị phân.
        size_t index = static_cast<size_t>(lower_bound(coordinates.begin(), coordinates.end(), value)
                                           - coordinates.begin());
        int previous = lastPosition[index];
        // Đóng góp của vị trí này khi là lần xuất hiện đầu tiên trong mảng con:
        // (position - previous) cách chọn đầu trái, (n - position + 1) cách chọn đầu phải.
        answer += static_cast<long long>(position - previous) * (n - position + 1LL);
        lastPosition[index] = position;
    }

    cout << answer << '\n';
}
