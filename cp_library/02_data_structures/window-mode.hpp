#pragma once
#include <bits/stdc++.h>
using namespace std;
// Window Mode Maintenance — thêm/xóa một giá trị, lấy mode; hòa thì lấy giá trị NHỎ NHẤT.
// Khi dùng: cửa sổ trượt cần phần tử xuất hiện nhiều nhất với tie-break theo giá trị.
// ĐPT: add/remove/mode O(log D), frequency O(log D); bộ nhớ O(D + maxFreq lịch sử).
// Dùng: WindowMode<long long> m; m.add(x); m.remove(x); auto answer=m.mode(); m.maxFrequency();
// Bẫy: remove(x) trả false nếu x không có; mode() là nullopt khi rỗng; T phải so sánh được bằng std::less.
// CSES: 3224

template<class T, class Compare = less<T>>
struct WindowMode {
    map<T, int, Compare> count;
    vector<set<T, Compare>> byFrequency = vector<set<T, Compare>>(1);
    int maximum = 0;
    long long elements = 0;

    // Ý nghĩa: thêm một occurrence của value và chuyển nó sang bucket tần suất kế tiếp.
    // Tham số: value = giá trị cần thêm.
    void add(const T& value) {
        int old = 0;
        auto it = count.find(value);
        if (it != count.end()) old = it->second;
        if (old > 0) byFrequency[old].erase(value);
        int now = old + 1;
        if ((int)byFrequency.size() <= now) byFrequency.resize(now + 1);
        byFrequency[now].insert(value);
        count[value] = now;
        maximum = max(maximum, now);
        ++elements;
    }
    // Ý nghĩa: xóa một occurrence nếu value đang hiện diện và cập nhật bucket/max frequency.
    // Tham số: value = giá trị cần xóa.
    // Trả về: true nếu đã xóa; false nếu count(value)==0.
    bool remove(const T& value) {
        auto it = count.find(value);
        if (it == count.end()) return false;
        int old = it->second;
        byFrequency[old].erase(value);
        if (old == 1) {
            count.erase(it);
        } else {
            it->second = old - 1;
            byFrequency[old - 1].insert(value);
        }
        --elements;
        while (maximum > 0 && byFrequency[maximum].empty()) --maximum;
        return true;
    }
    // Ý nghĩa: lấy mode hiện tại, tie-break bằng thứ tự Compare nhỏ nhất.
    // Trả về: optional chứa mode; nullopt nếu cấu trúc rỗng.
    optional<T> mode() const {
        if (maximum == 0) return nullopt;
        return *byFrequency[maximum].begin();
    }
    // Ý nghĩa: lấy tần suất của mode hiện tại.
    // Trả về: maximum, bằng 0 khi rỗng.
    int maxFrequency() const { return maximum; }
    // Ý nghĩa: lấy tần suất hiện tại của value.
    // Tham số: value = giá trị cần hỏi.
    // Trả về: số occurrence, bằng 0 nếu không có.
    int frequency(const T& value) const {
        auto it = count.find(value);
        return it == count.end() ? 0 : it->second;
    }
    // Ý nghĩa: tổng số occurrence đang lưu, tính cả trùng.
    long long size() const { return elements; }
    // Ý nghĩa: xóa toàn bộ dữ liệu và đưa cấu trúc về trạng thái rỗng.
    void clear() {
        count.clear();
        byFrequency.assign(1, {});
        maximum = 0;
        elements = 0;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ window-mode.hpp -o demo && ./demo
int main() {
    WindowMode<int> mode;
    for (int x : {4, 2, 4, 2, 7}) mode.add(x);
    printf("mode=%d freq=%d", *mode.mode(), mode.maxFrequency());  // 2, tie nhỏ hơn 4
    mode.remove(2);
    printf("; sau remove: %d\n", *mode.mode());                    // 4
    return 0;
}
#endif
