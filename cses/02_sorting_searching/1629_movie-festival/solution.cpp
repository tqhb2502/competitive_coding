#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Lưu mỗi phim dưới dạng (end, start) để sắp xếp tự nhiên theo thời điểm kết thúc.
    vector<pair<int, int>> movies;
    movies.reserve(n);
    for (int i = 0; i < n; ++i) {
        int start, end;
        cin >> start >> end;
        movies.emplace_back(end, start);
    }

    // Sắp xếp tăng dần theo thời điểm kết thúc (greedy).
    sort(movies.begin(), movies.end());

    int answer = 0;
    int current_end = -1;  // Thời điểm kết thúc của phim vừa chọn gần nhất.
    for (const auto& [end, start] : movies) {
        // Chọn phim nếu nó bắt đầu không sớm hơn lúc phim trước kết thúc.
        if (start >= current_end) {
            ++answer;
            current_end = end;
        }
    }

    cout << answer << '\n';
    return 0;
}
