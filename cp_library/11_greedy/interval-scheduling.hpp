#pragma once
#include <bits/stdc++.h>
using namespace std;
// Interval Scheduling — greedy trên đoạn: chọn tối đa đoạn không giao (theo điểm KẾT THÚC) và số phòng tối thiểu = độ chồng lấp lớn nhất.
// Khi dùng: chọn nhiều nhất phim/hoạt động không trùng giờ; chia mọi đoạn vào ít phòng/máy nhất; đo số đoạn chồng nhau nhiều nhất.
// ĐPT: maxNonOverlappingPick/maxNonOverlapping O(n log n) (sort theo r); minRooms O(n log n) (sort 2n sự kiện); minRoomsAssign O(n log n) (sort theo l + heap); bộ nhớ O(n).
// Dùng: vector<pair<long long, long long>> iv = {{3, 5}, {4, 9}, {5, 8}};
//        int k = maxNonOverlapping(iv); vector<int> id = maxNonOverlappingPick(iv);  // đoạn NỬA MỞ [l, r)
//        int rooms = minRooms(iv); vector<int> room = minRoomsAssign(iv);            // đoạn ĐÓNG [l, r]
// Bẫy: hai nhóm hàm dùng HAI quy ước khác nhau — (a) nửa mở [l, r) nên r == l' KHÔNG tính là giao (CSES 1629),
//       (b) đóng [l, r] nên trùng đúng một mốc LÀ giao (CSES 1164); muốn (a) hiểu đoạn đóng thì truyền {l, r + 1}; đòi l <= r; ở (a) đoạn rỗng l == r không giao với ai nên luôn được chọn; phòng đánh số 0-based (CSES in room[i] + 1).
// CSES: 1164 1629

// Ý nghĩa: chọn tập lớn nhất các đoạn NỬA MỞ [l, r) đôi một không giao (giao nghĩa là max(l) < min(r)),
//          greedy theo điểm kết thúc r tăng dần — luôn giữ đoạn kết thúc sớm nhất còn hợp lệ.
// Tham số: iv = danh sách đoạn (l, r) với l <= r, đánh chỉ số 0-based theo thứ tự trong iv.
// Trả về: chỉ số gốc của các đoạn được chọn, xếp theo (r, l) tăng dần (đúng thứ tự thời gian greedy chọn).
vector<int> maxNonOverlappingPick(const vector<pair<long long, long long>>& iv) {
    int n = (int)iv.size();
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y) {
        if (iv[x].second != iv[y].second) return iv[x].second < iv[y].second;
        return iv[x].first < iv[y].first;
    });
    vector<int> pick;
    long long cur = LLONG_MIN;  // cur = điểm kết thúc của đoạn được chọn gần nhất
    for (int i : ord) {
        long long l = iv[i].first, r = iv[i].second;
        if (l == r) {           // đoạn rỗng [t, t): không giao với đoạn nào, chọn "miễn phí"
            pick.push_back(i);
            continue;
        }
        if (l >= cur) {         // bắt đầu không sớm hơn lúc đoạn trước kết thúc => chạm biên vẫn được
            pick.push_back(i);
            cur = r;
        }
    }
    return pick;
}

// Ý nghĩa: đếm số đoạn NỬA MỞ [l, r) nhiều nhất có thể chọn để đôi một không giao (CSES 1629 Movie Festival).
// Tham số: iv = danh sách đoạn (l, r) với l <= r.
// Trả về: lực lượng lớn nhất của một tập đoạn đôi một không giao.
int maxNonOverlapping(const vector<pair<long long, long long>>& iv) {
    return (int)maxNonOverlappingPick(iv).size();
}

// Ý nghĩa: số phòng ít nhất để chứa mọi đoạn ĐÓNG [l, r] (hai đoạn chồng nhau phải khác phòng) —
//          bằng độ chồng lấp lớn nhất, tính bằng quét sự kiện (+1 tại l, -1 tại r, cùng mốc thì +1 trước).
// Tham số: iv = danh sách đoạn (l, r) với l <= r; trùng đúng một mốc (r == l') ĐƯỢC coi là chồng nhau.
// Trả về: số đoạn chồng nhau nhiều nhất tại một thời điểm = số phòng tối thiểu (0 nếu iv rỗng).
int minRooms(const vector<pair<long long, long long>>& iv) {
    vector<pair<long long, int>> ev;
    ev.reserve(iv.size() * 2);
    for (const auto& p : iv) {
        ev.emplace_back(p.first, 1);    // mở đoạn
        ev.emplace_back(p.second, -1);  // đóng đoạn
    }
    sort(ev.begin(), ev.end(), [](const pair<long long, int>& x, const pair<long long, int>& y) {
        if (x.first != y.first) return x.first < y.first;
        return x.second > y.second;      // cùng mốc: mở (+1) trước đóng (-1) => trùng biên là CHỒNG
    });
    int cur = 0, best = 0;
    for (const auto& e : ev) {
        cur += e.second;
        best = max(best, cur);
    }
    return best;
}

// Ý nghĩa: gán phòng cho từng đoạn ĐÓNG [l, r] sao cho không phòng nào chứa hai đoạn chồng nhau, dùng
//          đúng minRooms(iv) phòng — xét đoạn theo l tăng, min-heap (thời điểm kết thúc, phòng) để tái dùng
//          phòng trống sớm nhất (CSES 1164 Room Allocation).
// Tham số: iv = danh sách đoạn (l, r) với l <= r, chỉ số 0-based.
// Trả về: room với room[i] = số phòng 0-based của đoạn i (theo thứ tự gốc); số phòng dùng = max(room) + 1
//         = minRooms(iv); vector rỗng nếu iv rỗng.
vector<int> minRoomsAssign(const vector<pair<long long, long long>>& iv) {
    int n = (int)iv.size();
    vector<int> ord(n), room(n, -1);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y) {
        if (iv[x].first != iv[y].first) return iv[x].first < iv[y].first;
        return iv[x].second < iv[y].second;
    });
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    int k = 0;  // số phòng đã mở
    for (int i : ord) {
        long long l = iv[i].first, r = iv[i].second;
        if (!pq.empty() && pq.top().first < l) {  // phòng trống HẲN trước l (đóng < l, không chạm biên)
            int id = pq.top().second;
            pq.pop();
            room[i] = id;
        } else {
            room[i] = k++;                       // mọi phòng còn bị chiếm => mở phòng mới
        }
        pq.emplace(r, room[i]);
    }
    return room;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ interval-scheduling.hpp -o demo && ./demo
int main() {
    // (a) CSES 1629: 3 phim (3,5) (4,9) (5,8), nửa mở nên xem được (3,5) rồi (5,8).
    vector<pair<long long, long long>> movies = {{3, 5}, {4, 9}, {5, 8}};
    printf("maxNonOverlapping = %d\n", maxNonOverlapping(movies));  // 2
    vector<int> pick = maxNonOverlappingPick(movies);
    printf("chi so chon =");
    for (int i : pick) printf(" %d", i);
    printf("\n");  // 0 2  (phim [3,5) rồi [5,8))

    // (b) CSES 1164: 3 khách [1,2] [2,4] [4,4], đoạn đóng nên trùng mốc là chồng nhau.
    vector<pair<long long, long long>> guests = {{1, 2}, {2, 4}, {4, 4}};
    printf("minRooms = %d\n", minRooms(guests));  // 2
    vector<int> room = minRoomsAssign(guests);
    printf("phong (1-based) =");
    for (int x : room) printf(" %d", x + 1);
    printf("\n");  // 1 2 1
    return 0;
}
#endif
