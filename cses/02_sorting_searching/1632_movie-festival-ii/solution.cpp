#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    // Lưu mỗi phim dưới dạng (end, start) để tiện sắp xếp theo thời gian kết thúc.
    vector<pair<int, int>> movies;
    movies.reserve(n);
    for (int i = 0; i < n; ++i) {
        int start, end;
        cin >> start >> end;
        movies.emplace_back(end, start);
    }

    // Sắp xếp các phim theo thời gian kết thúc b tăng dần.
    sort(movies.begin(), movies.end());

    // multiset chứa "thời điểm rảnh" của k thành viên, ban đầu tất cả bằng 0.
    multiset<int> available_times;
    for (int member = 0; member < k; ++member) {
        available_times.insert(0);
    }

    int answer = 0;
    for (const auto& [end, start] : movies) {
        // Tìm thành viên có thời điểm rảnh LỚN NHẤT mà vẫn <= start:
        // upper_bound cho phần tử đầu tiên > start, lùi một bước là giá trị cần.
        auto member = available_times.upper_bound(start);
        if (member == available_times.begin()) {
            continue;  // Không thành viên nào rảnh kịp trước khi phim bắt đầu.
        }

        --member;
        // Giao phim cho thành viên đó: cập nhật thời điểm rảnh thành end.
        available_times.erase(member);
        available_times.insert(end);
        ++answer;
    }

    cout << answer << '\n';
    return 0;
}
