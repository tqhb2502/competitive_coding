#pragma once
#include <bits/stdc++.h>
using namespace std;
// Interactive Search — quản lý query budget, ghép callback emit-and-flush/read, và mã hoá đồng thời nhãn 1-based bằng truy vấn bit.
// Khi dùng: bài interactive có giới hạn truy vấn; muốn tách logic thuật toán khỏi cin/cout để mock judge; một query trả thông tin cho nhiều vị trí.
// ĐPT: mỗi performBudgetedInteractiveQuery ngoài callback là O(1); tạo/giải B truy vấn bit là O(items*B), B=floor(log2(maxLabel))+1.
// Dùng: InteractiveQueryBudget budget(10); reply = performBudgetedInteractiveQuery(budget,q,emitAndFlush,readReply);
//        auto queries = oneBasedBinaryLabelQueries(n); auto permutation = decodeBinaryLabelReplies(replies,n,true);
// Bẫy: emitAndFlush PHẢI gửi trọn request và flush trước khi return; helper không tự làm I/O. Budget được trừ TRƯỚC callback (callback lỗi vẫn coi là đã dùng).
//       Các hàng bit theo thứ tự LSB trước; decode cần đúng số hàng và mọi hàng cùng độ dài. Judge trả -1/EOF phải được callback xử lý ngay.
// CSES: 3139 3140 3228

class InteractiveQueryBudget {
    long long limit_;
    long long used_ = 0;

public:
    // Ý nghĩa: tạo bộ đếm query với giới hạn cố định.
    // Tham số: limit = số query tối đa, phải không âm.
    explicit InteractiveQueryBudget(long long limit) : limit_(limit) {
        if (limit < 0) throw invalid_argument("InteractiveQueryBudget: limit âm");
    }

    // Ý nghĩa: lấy giới hạn query ban đầu.
    // Trả về: limit không âm.
    long long queryLimit() const { return limit_; }

    // Ý nghĩa: lấy số query đã được consume.
    // Trả về: used trong [0,limit].
    long long queriesUsed() const { return used_; }

    // Ý nghĩa: lấy số query còn lại.
    // Trả về: limit-used.
    long long queriesRemaining() const { return limit_ - used_; }

    // Ý nghĩa: kiểm tra có thể dùng thêm amount query mà không vượt giới hạn hay không.
    // Tham số: amount = số query muốn dùng, phải không âm để có ý nghĩa.
    // Trả về: true khi 0 <= amount <= queriesRemaining().
    bool canConsume(long long amount = 1) const {
        return amount >= 0 && amount <= queriesRemaining();
    }

    // Ý nghĩa: ghi nhận dùng thêm amount query; kiểm tra budget trước khi thay đổi bộ đếm.
    // Tham số: amount = số query dương hoặc 0.
    void consume(long long amount = 1) {
        if (amount < 0) throw invalid_argument("InteractiveQueryBudget::consume: amount âm");
        if (!canConsume(amount)) throw length_error("InteractiveQueryBudget::consume: vượt query budget");
        used_ += amount;
    }
};

// Ý nghĩa: thực hiện đúng thứ tự budget.consume -> emitAndFlush(request) -> readReply(), không áp đặt kiểu I/O.
// Tham số: budget = bộ đếm; request = dữ liệu query; emitAndFlush = callback gửi+flush; readReply = callback đọc/kiểm tra phản hồi.
// Trả về: nguyên kiểu giá trị do readReply trả về (có thể là reference hoặc void).
template <class Request, class EmitAndFlush, class ReadReply>
decltype(auto) performBudgetedInteractiveQuery(
    InteractiveQueryBudget& budget, const Request& request,
    EmitAndFlush&& emitAndFlush, ReadReply&& readReply) {
    budget.consume();
    std::invoke(emitAndFlush, request);
    return std::invoke(readReply);
}

// Ý nghĩa: số bit tối thiểu để biểu diễn phân biệt mọi nhãn nguyên trong [1,maxLabel].
// Tham số: maxLabel = nhãn lớn nhất không âm.
// Trả về: 0 nếu maxLabel=0, ngược lại floor(log2(maxLabel))+1.
inline int binaryQueryBitsForLabels(int maxLabel) {
    if (maxLabel < 0) throw invalid_argument("binaryQueryBitsForLabels: maxLabel âm");
    int bits = 0;
    while (maxLabel > 0) {
        ++bits;
        maxLabel >>= 1;
    }
    return bits;
}

// Ý nghĩa: dựng các xâu query mà cột label-1 chứa biểu diễn nhị phân của label, theo từng bit LSB trước.
// Tham số: maxLabel = n không âm; các label/cột là 1..n.
// Trả về: B xâu độ dài n, queries[bit][label-1] = '0' + ((label>>bit)&1).
inline vector<string> oneBasedBinaryLabelQueries(int maxLabel) {
    int bits = binaryQueryBitsForLabels(maxLabel);
    vector<string> queries(bits, string(maxLabel, '0'));
    for (int bit = 0; bit < bits; ++bit) {
        for (int label = 1; label <= maxLabel; ++label) {
            queries[bit][label - 1] = char('0' + ((label >> bit) & 1));
        }
    }
    return queries;
}

// Ý nghĩa: ghép các hàng reply bit LSB trước thành nhãn tại từng cột; tuỳ chọn kiểm tra kết quả là hoán vị 1..maxLabel.
// Tham số: replies = đúng B xâu bit cùng độ dài; maxLabel = cận nhãn; requirePermutation = có bắt buộc mỗi nhãn xuất hiện đúng một lần.
// Trả về: vector nhãn 1-based; ném invalid_argument khi kích thước/bit/nhãn hoặc điều kiện hoán vị sai.
inline vector<int> decodeBinaryLabelReplies(
    const vector<string>& replies, int maxLabel, bool requirePermutation = false) {
    int bits = binaryQueryBitsForLabels(maxLabel);
    if ((int)replies.size() != bits)
        throw invalid_argument("decodeBinaryLabelReplies: sai số hàng bit");
    if (bits == 0) return {};
    size_t itemCount = replies[0].size();
    for (const string& row : replies) {
        if (row.size() != itemCount)
            throw invalid_argument("decodeBinaryLabelReplies: các hàng khác độ dài");
    }

    vector<int> labels(itemCount, 0);
    for (int bit = 0; bit < bits; ++bit) {
        for (size_t item = 0; item < itemCount; ++item) {
            char digit = replies[bit][item];
            if (digit != '0' && digit != '1')
                throw invalid_argument("decodeBinaryLabelReplies: reply không phải xâu bit");
            if (digit == '1') labels[item] |= 1 << bit;
        }
    }
    for (int label : labels) {
        if (label < 1 || label > maxLabel)
            throw invalid_argument("decodeBinaryLabelReplies: nhãn giải mã ngoài [1,maxLabel]");
    }
    if (requirePermutation) {
        if (itemCount != (size_t)maxLabel)
            throw invalid_argument("decodeBinaryLabelReplies: hoán vị phải có đúng maxLabel phần tử");
        vector<char> seen(maxLabel + 1, false);
        for (int label : labels) {
            if (seen[label])
                throw invalid_argument("decodeBinaryLabelReplies: nhãn lặp trong hoán vị");
            seen[label] = true;
        }
    }
    return labels;
}

// Ý nghĩa: suy ra số nghịch thế x bên trong một đoạn TRƯỚC khi reverse từ delta tổng: after=before+C(length,2)-2x.
// Tham số: totalBefore = tổng inversion trước reverse; totalAfter = tổng sau reverse; length = độ dài đoạn không âm.
// Trả về: x trong [0,C(length,2)]; ném invalid_argument nếu số đo không thể hợp lệ và overflow_error nếu x không vừa long long.
inline long long internalInversionsBeforeReverse(
    long long totalBefore, long long totalAfter, long long length) {
    if (totalBefore < 0 || totalAfter < 0 || length < 0)
        throw invalid_argument("internalInversionsBeforeReverse: tham số âm");
    __int128 pairs = (__int128)length * (length - 1) / 2;
    __int128 numerator = (__int128)totalBefore + pairs - totalAfter;
    if (numerator < 0 || (numerator & 1))
        throw invalid_argument("internalInversionsBeforeReverse: delta không hợp lệ");
    __int128 inside = numerator / 2;
    if (inside > pairs)
        throw invalid_argument("internalInversionsBeforeReverse: x vượt số cặp trong đoạn");
    if (inside > numeric_limits<long long>::max())
        throw overflow_error("internalInversionsBeforeReverse: kết quả vượt long long");
    return (long long)inside;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ interactive-search.hpp -o demo && ./demo
int main() {
    vector<int> hiddenPermutation = {3, 1, 4, 2};  // reply[i] = query[a[i]-1]
    auto queries = oneBasedBinaryLabelQueries(4);
    vector<string> replies;
    string pending;
    InteractiveQueryBudget budget(3);
    for (const string& query : queries) {
        replies.push_back(performBudgetedInteractiveQuery(
            budget, query,
            [&](const string& emitted) { pending = emitted; },  // mock "emit + flush"
            [&]() {
                string reply;
                for (int label : hiddenPermutation) reply += pending[label - 1];
                return reply;
            }));
    }
    auto decoded = decodeBinaryLabelReplies(replies, 4, true);
    printf("decoded:");
    for (int x : decoded) printf(" %d", x);
    printf("; used=%lld; inside=%lld\n",
           budget.queriesUsed(), internalInversionsBeforeReverse(5, 4, 3));
    // decoded: 3 1 4 2; used=3; inside=2
    return 0;
}
#endif
