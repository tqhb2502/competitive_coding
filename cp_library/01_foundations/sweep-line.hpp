#pragma once
#include <bits/stdc++.h>
using namespace std;
// Sweep Line — sắp sự kiện theo (tọa độ, phase) và kiểm tra quan hệ chứa nhau của mọi đoạn đóng.
// Khi dùng: biến đối tượng hình học/thời gian thành OPEN/QUERY/CLOSE; cần quy định rõ thứ tự sự kiện cùng tọa độ; nested ranges.
// ĐPT: orderedSweepEvents O(e log e), intervalContainmentFlags O(n log n); bộ nhớ O(e) hoặc O(n), gồm cả bản sao output/thứ tự.
// Dùng: auto ordered = orderedSweepEvents(events, getX, getPhase);  // phase nhỏ được xử lý trước
//        auto f = intervalContainmentFlags({{1,6},{2,4},{4,8}});     // đoạn ĐÓNG
// Bẫy: tie là phần cốt lõi: stabbing đoạn ĐÓNG thường OPEN < QUERY < CLOSE; đoạn NỬA MỞ thường CLOSE < OPEN < QUERY tại cùng x.
//       intervalContainmentFlags coi hai bản sao [l,r] là hai đoạn khác: mỗi bản vừa chứa vừa bị chứa bởi bản còn lại; yêu cầu l <= r.
// CSES: 1203 1619 1740 1741 1742 2168 3163

// Ý nghĩa: trả bản sao events được stable-sort theo coordinateOf(event), rồi phaseOf(event) tăng dần; exact tie giữ thứ tự đầu vào.
// Tham số: events = danh sách sự kiện; coordinateOf = projection tọa độ; phaseOf = projection độ ưu tiên (nhỏ xử lý trước).
// Trả về: danh sách sự kiện đã có thứ tự quét; kiểu tọa độ và phase chỉ cần hỗ trợ operator<.
template <class Event, class CoordinateOf, class PhaseOf>
vector<Event> orderedSweepEvents(vector<Event> events, CoordinateOf coordinateOf, PhaseOf phaseOf) {
    stable_sort(events.begin(), events.end(), [&](const Event& a, const Event& b) {
        const auto& xa = std::invoke(coordinateOf, a);
        const auto& xb = std::invoke(coordinateOf, b);
        if (xa < xb) return true;
        if (xb < xa) return false;
        const auto& pa = std::invoke(phaseOf, a);
        const auto& pb = std::invoke(phaseOf, b);
        return pa < pb;
    });
    return events;
}

struct IntervalContainmentFlags {
    vector<char> contains;    // contains[i] = 1 khi đoạn i chứa ít nhất một đoạn khác.
    vector<char> containedBy; // containedBy[i] = 1 khi đoạn i bị ít nhất một đoạn khác chứa.
};

// Ý nghĩa: với từng đoạn đóng [l,r], đánh dấu nó có chứa / bị chứa bởi ít nhất một đoạn khác hay không.
// Tham số: intervals = danh sách {l,r} theo thứ tự cần trả lời, mỗi đoạn phải thoả l <= r; các đoạn trùng nhau được phép.
// Trả về: hai vector char 0/1 cùng kích thước input trong IntervalContainmentFlags.
inline IntervalContainmentFlags intervalContainmentFlags(
    const vector<pair<long long, long long>>& intervals) {
    int n = (int)intervals.size();
    for (const auto& interval : intervals) {
        if (interval.first > interval.second)
            throw invalid_argument("intervalContainmentFlags: gặp đoạn có l > r");
    }

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    stable_sort(order.begin(), order.end(), [&](int i, int j) {
        if (intervals[i].first != intervals[j].first)
            return intervals[i].first < intervals[j].first;
        return intervals[i].second > intervals[j].second;
    });

    IntervalContainmentFlags answer{vector<char>(n, false), vector<char>(n, false)};

    // Quét trái -> phải: mọi group trước có l nhỏ hơn, hoặc cùng l nhưng r lớn hơn.
    bool hasMaximum = false;
    long long maximumRight = 0;
    for (int begin = 0; begin < n; ) {
        int end = begin + 1;
        while (end < n && intervals[order[end]] == intervals[order[begin]]) ++end;
        bool hasDuplicate = end - begin >= 2;
        long long right = intervals[order[begin]].second;
        bool isContained = hasDuplicate || (hasMaximum && maximumRight >= right);
        for (int k = begin; k < end; ++k) answer.containedBy[order[k]] = isContained;
        if (!hasMaximum || right > maximumRight) maximumRight = right;
        hasMaximum = true;
        begin = end;
    }

    // Quét phải -> trái: mọi group sau có l lớn hơn, hoặc cùng l nhưng r nhỏ hơn.
    bool hasMinimum = false;
    long long minimumRight = 0;
    for (int end = n; end > 0; ) {
        int begin = end - 1;
        while (begin > 0 && intervals[order[begin - 1]] == intervals[order[end - 1]]) --begin;
        bool hasDuplicate = end - begin >= 2;
        long long right = intervals[order[begin]].second;
        bool doesContain = hasDuplicate || (hasMinimum && minimumRight <= right);
        for (int k = begin; k < end; ++k) answer.contains[order[k]] = doesContain;
        if (!hasMinimum || right < minimumRight) minimumRight = right;
        hasMinimum = true;
        end = begin;
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ sweep-line.hpp -o demo && ./demo
int main() {
    vector<tuple<int, int, string>> events = {
        {3, 2, "close"}, {3, 0, "open"}, {3, 1, "query"}, {1, 0, "first"}};
    auto ordered = orderedSweepEvents(
        events, [](const auto& e) { return get<0>(e); }, [](const auto& e) { return get<1>(e); });
    printf("events:");
    for (const auto& e : ordered) printf(" %s", get<2>(e).c_str());
    // events: first open query close

    auto flags = intervalContainmentFlags({{1, 6}, {2, 4}, {4, 8}, {1, 6}});
    printf("; contains:");
    for (char x : flags.contains) printf(" %d", (int)x);
    printf("; contained:");
    for (char x : flags.containedBy) printf(" %d", (int)x);
    printf("\n");
    // contains: 1 0 0 1; contained: 1 1 0 1
    return 0;
}
#endif
