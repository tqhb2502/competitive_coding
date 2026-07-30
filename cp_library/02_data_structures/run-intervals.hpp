#pragma once
#include <bits/stdc++.h>
using namespace std;
// Run Intervals — duy trì các đoạn ký tự bằng nhau cực đại dưới phép gán/đảo một vị trí.
// Khi dùng: chuỗi động với point update; cần độ dài run lớn nhất hoặc run chứa một vị trí.
// ĐPT: dựng O(n log n), assign/flipBit O(log n), maxRun O(1), runRange O(log n); bộ nhớ O(n).
// Dùng: RunIntervals runs("00110"); runs.flipBit(2); int best = runs.maxRun();
// Bẫy: vị trí 0-based; runRange trả đoạn NỬA MỞ [l,r); flipBit chỉ nhận ký tự '0'/'1'; chuỗi rỗng có maxRun=0.
// CSES: 1188
struct RunIntervals {
private:
    string data;
    set<int> boundaries;       // 0, n và i nếu data[i-1] != data[i].
    multiset<int> runLengths;  // độ dài giữa hai boundary liên tiếp.

public:
    // Ý nghĩa: tạo cấu trúc rỗng.
    RunIntervals() { reset(""); }

    // Ý nghĩa: tạo cấu trúc từ toàn bộ chuỗi ban đầu.
    // Tham số: initial = chuỗi ký tự cần chia thành các run cực đại.
    explicit RunIntervals(string initial) { reset(std::move(initial)); }

    // Ý nghĩa: thay toàn bộ nội dung và dựng lại các run.
    // Tham số: initial = chuỗi ký tự mới.
    void reset(string initial) {
        if (initial.size() > (size_t)INT_MAX)
            throw length_error("RunIntervals::reset: chuỗi quá dài cho chỉ số int");
        data = std::move(initial);
        boundaries.clear();
        runLengths.clear();
        int n = (int)data.size();
        boundaries.insert(0);
        for (int i = 1; i < n; ++i)
            if (data[i - 1] != data[i]) boundaries.insert(i);
        boundaries.insert(n);
        if (n == 0) return;
        for (auto it = next(boundaries.begin()); it != boundaries.end(); ++it)
            runLengths.insert(*it - *prev(it));
    }

    // Ý nghĩa: lấy số ký tự hiện có.
    // Trả về: độ dài chuỗi.
    int size() const { return (int)data.size(); }

    // Ý nghĩa: kiểm tra cấu trúc có rỗng hay không.
    // Trả về: true khi chuỗi có độ dài 0.
    bool empty() const { return data.empty(); }

    // Ý nghĩa: đọc toàn bộ chuỗi hiện tại mà không sao chép.
    // Trả về: tham chiếu const tới chuỗi nội bộ; nội dung phản ánh các lần assign/reset sau đó.
    const string& values() const { return data; }

    // Ý nghĩa: đọc ký tự tại một vị trí.
    // Tham số: position = chỉ số 0-based.
    // Trả về: ký tự data[position]; ném out_of_range nếu vị trí không hợp lệ.
    char at(int position) const {
        checkPosition(position);
        return data[position];
    }

    // Ý nghĩa: gán một ký tự và cập nhật hai biên có thể thay đổi.
    // Tham số: position = chỉ số 0-based; value = ký tự mới.
    // Trả về: độ dài run lớn nhất sau phép gán (không đổi cấu trúc nếu ký tự đã bằng value).
    int assign(int position, char value) {
        checkPosition(position);
        if (data[position] == value) return maxRun();
        data[position] = value;
        if (position > 0)
            synchronizeBoundary(position, data[position - 1] != data[position]);
        if (position + 1 < size())
            synchronizeBoundary(position + 1, data[position] != data[position + 1]);
        return maxRun();
    }

    // Ý nghĩa: đảo bit '0' thành '1' hoặc ngược lại rồi cập nhật các run.
    // Tham số: position = chỉ số 0-based của ký tự bit.
    // Trả về: độ dài run lớn nhất sau phép đảo; ném invalid_argument nếu ký tự không phải bit.
    int flipBit(int position) {
        checkPosition(position);
        if (data[position] != '0' && data[position] != '1')
            throw invalid_argument("RunIntervals::flipBit: ký tự không phải '0'/'1'");
        return assign(position, data[position] == '0' ? '1' : '0');
    }

    // Ý nghĩa: lấy độ dài lớn nhất trong các run hiện tại.
    // Trả về: max độ dài run, hoặc 0 nếu chuỗi rỗng.
    int maxRun() const {
        return runLengths.empty() ? 0 : *runLengths.rbegin();
    }

    // Ý nghĩa: tìm run cực đại chứa một vị trí.
    // Tham số: position = chỉ số 0-based.
    // Trả về: cặp {l,r} mô tả đoạn NỬA MỞ [l,r); ném out_of_range nếu vị trí không hợp lệ.
    pair<int, int> runRange(int position) const {
        checkPosition(position);
        auto right = boundaries.upper_bound(position);
        return {*prev(right), *right};
    }

    // Ý nghĩa: lấy độ dài run cực đại chứa một vị trí.
    // Tham số: position = chỉ số 0-based.
    // Trả về: r-l với [l,r) = runRange(position).
    int runLengthAt(int position) const {
        auto range = runRange(position);
        return range.second - range.first;
    }

private:
    // Ý nghĩa: kiểm tra một vị trí thuộc chuỗi, ném out_of_range nếu sai.
    // Tham số: position = chỉ số 0-based cần kiểm tra.
    void checkPosition(int position) const {
        if (position < 0 || position >= size())
            throw out_of_range("RunIntervals: vị trí ngoài chuỗi");
    }

    // Ý nghĩa: thêm/xóa một boundary nội bộ và đồng bộ multiset độ dài.
    // Tham số: point = biên giữa point-1 và point (0 < point < n);
    //          shouldExist = trạng thái boundary đúng sau cập nhật.
    void synchronizeBoundary(int point, bool shouldExist) {
        auto found = boundaries.find(point);
        if ((found != boundaries.end()) == shouldExist) return;
        if (shouldExist) {
            auto right = boundaries.upper_bound(point);
            int r = *right, l = *prev(right);
            eraseOneLength(r - l);
            runLengths.insert(point - l);
            runLengths.insert(r - point);
            boundaries.insert(point);
        } else {
            int l = *prev(found), r = *next(found);
            eraseOneLength(point - l);
            eraseOneLength(r - point);
            runLengths.insert(r - l);
            boundaries.erase(found);
        }
    }

    // Ý nghĩa: xóa đúng một bản của một độ dài đang tồn tại.
    // Tham số: length = độ dài cần xóa khỏi multiset.
    void eraseOneLength(int length) {
        auto it = runLengths.find(length);
        if (it == runLengths.end())
            throw logic_error("RunIntervals: invariant độ dài bị hỏng");
        runLengths.erase(it);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ run-intervals.hpp -o demo && ./demo
int main() {
    RunIntervals runs("00110");
    printf("ban dau=%d; sau flip(2)=%d\n", runs.maxRun(), runs.flipBit(2));  // 2, 3 ("00010")
    auto [l, r] = runs.runRange(1);
    printf("run chua 1 = [%d,%d), dai=%d\n", l, r, runs.runLengthAt(1));   // [0,3), 3
    return 0;
}
#endif
