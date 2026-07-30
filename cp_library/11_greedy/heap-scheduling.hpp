#pragma once
#include <bits/stdc++.h>
using namespace std;
// Heap Scheduling — greedy lịch biểu dùng heap/multiset: k người/máy xem được tối đa bao nhiêu đoạn, và thứ tự việc tối ưu.
// Khi dùng: đoạn thời gian chia cho k "người/máy" sao cho mỗi người không xem hai đoạn chồng lấn; hoặc xếp thứ tự việc để tối thiểu tổng thời điểm hoàn thành.
// ĐPT: maxMoviesKPeople O(n log n) (sort n log n + n lần upper_bound/erase/insert trên multiset min(k,n) phần tử); maxTasksProfit O(n log n) (chỉ sort); bộ nhớ O(n).
// Dùng: int c  = maxMoviesKPeople({{1,2},{1,5},{6,7},{3,8}}, 2);   // 4 phim
//        long long p = maxTasksProfit({{6,10},{8,15},{5,12}});       // 2
// Bẫy: MỌI đoạn ở đây là NỬA MỞ [l, r) nên kề nhau (r1 == l2) KHÔNG chồng lấn; với ngày ĐÓNG [a, b] (chạm nhau là xung đột) phải truyền {a, b+1}.
//       tiền điều kiện start < end (đoạn rỗng làm greedy sai); maxTasksProfit tổng cỡ n^2 * maxDuration (~2e16) nên phải long long; k <= 0 trả 0.
//       Cần CHIA ĐOẠN VÀO ÍT PHÒNG NHẤT (CSES 1164) thì dùng minRooms/minRoomsAssign ở interval-scheduling.hpp (đoạn ĐÓNG) — đừng viết lại ở đây.
// CSES: 1630 1632

// Ý nghĩa: số đoạn (phim) tối đa mà k người cùng xem được, mỗi người không được xem hai đoạn chồng lấn — greedy sort theo thời điểm kết thúc, mỗi đoạn giao cho người có thời điểm rảnh LỚN NHẤT mà vẫn <= thời điểm bắt đầu (upper_bound rồi lùi một bước).
// Tham số: intervals = danh sách đoạn {start, end} nửa mở [start, end), giả thiết start < end (nhận bản sao vì hàm tự sort); k = số người (k > n tự cắt về n vì không cần thêm người).
// Trả về: số đoạn tối đa xem được (0 nếu k <= 0 hoặc intervals rỗng).
int maxMoviesKPeople(vector<pair<long long, long long>> intervals, int k) {
    int n = (int)intervals.size();
    if (k <= 0 || n == 0) return 0;
    int people = (int)min<long long>((long long)k, (long long)n);  // nhiều hơn n người là vô ích
    sort(intervals.begin(), intervals.end(),
         [](const pair<long long, long long>& x, const pair<long long, long long>& y) {
             return x.second < y.second;  // thời điểm kết thúc tăng dần
         });
    // freeAt = đa tập thời điểm rảnh của từng người; LLONG_MIN nghĩa là chưa xem gì (rảnh từ vô cùng sớm)
    multiset<long long> freeAt;
    for (int i = 0; i < people; i++) freeAt.insert(LLONG_MIN);
    int taken = 0;
    for (const auto& iv : intervals) {
        auto it = freeAt.upper_bound(iv.first);   // phần tử đầu tiên > start
        if (it == freeAt.begin()) continue;      // không ai rảnh trước start -> bỏ đoạn này
        --it;                                     // thời điểm rảnh lớn nhất <= start
        freeAt.erase(it);                         // xóa ĐÚNG một phần tử (dùng iterator, không dùng giá trị)
        freeAt.insert(iv.second);
        taken++;
    }
    return taken;
}

// Ý nghĩa: xếp thứ tự làm việc (bắt đầu từ thời điểm 0, mỗi lúc một việc) để tối đa tổng (deadline - thời điểm hoàn thành) — greedy Shortest Processing Time first: sort theo duration tăng dần.
// Tham số: tasks = danh sách việc {duration, deadline} (nhận bản sao vì hàm tự sort); duration nên > 0.
// Trả về: tổng phần thưởng lớn nhất, có thể ÂM và rất lớn về độ lớn nên là long long (0 nếu tasks rỗng).
long long maxTasksProfit(vector<pair<long long, long long>> tasks) {
    sort(tasks.begin(), tasks.end());  // pair so sánh duration trước -> duration tăng dần
    long long now = 0, best = 0;
    for (const auto& t : tasks) {
        now += t.first;             // thời điểm hoàn thành việc này
        best += t.second - now;     // deadline - thời điểm hoàn thành
    }
    return best;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ heap-scheduling.hpp -o demo && ./demo
int main() {
    vector<pair<long long, long long>> movies = {{1, 2}, {1, 5}, {6, 7}, {3, 8}};
    printf("k=1 -> %d, k=2 -> %d\n", maxMoviesKPeople(movies, 1), maxMoviesKPeople(movies, 2));
    // k=1 -> 2, k=2 -> 4

    printf("tasks -> %lld\n", maxTasksProfit({{6, 10}, {8, 15}, {5, 12}}));
    // tasks -> 2   (CSES 1630 mẫu)

    return 0;
}
#endif
