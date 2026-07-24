#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Đọc thời điểm đến và rời đi của từng khách hàng
    vector<int> arrivals(n), departures(n);
    for (int i = 0; i < n; ++i) {
        cin >> arrivals[i] >> departures[i];
    }

    // Sắp xếp riêng mảng thời điểm đến và mảng thời điểm rời đi
    sort(arrivals.begin(), arrivals.end());
    sort(departures.begin(), departures.end());

    int arrival_index = 0;
    int departure_index = 0;
    int current = 0;  // số khách đang có mặt
    int answer = 0;   // đỉnh (giá trị lớn nhất) của current

    // Quét (sweep) bằng hai con trỏ để hợp nhất hai chuỗi sự kiện theo thời gian
    while (arrival_index < n) {
        if (arrivals[arrival_index] < departures[departure_index]) {
            // Sự kiện đến: thêm một khách và cập nhật đỉnh
            ++current;
            answer = max(answer, current);
            ++arrival_index;
        } else {
            // Sự kiện rời đi: bớt một khách. Đề bài bảo đảm mọi thời điểm đều
            // phân biệt nên thời điểm đến không bao giờ trùng thời điểm rời đi.
            --current;
            ++departure_index;
        }
    }

    cout << answer << '\n';
    return 0;
}
