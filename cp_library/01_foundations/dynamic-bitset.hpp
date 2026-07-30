#pragma once
#include <bits/stdc++.h>
using namespace std;
// Dynamic Bitset — bitset độ dài runtime, hỗ trợ OR với bản dịch trái và AND+popcount.
// Khi dùng: subset-sum bằng dp |= dp<<x khi tổng lớn; giao hai hàng bit; đánh dấu tập tới vài triệu bit.
// ĐPT: set/test O(1); shiftOr/countAnd O(n/64); bộ nhớ O(n/64).
// Dùng: DynamicBitset b(S+1); b.set(0); b.shiftOr(x); b.test(S); a.countAnd(other);
// Bẫy: chỉ số 0-based [0,n); shift âm bị từ chối; shiftOr là IN-PLACE nhưng duyệt cao xuống nên đúng 0/1 knapsack.
// CSES: 1706 1745 2137 3159 3360 3417
struct DynamicBitset {
    int n = 0;
    vector<uint64_t> word;

    // Ý nghĩa: tạo bitset n bit, ban đầu toàn 0.
    // Tham số: size = số bit không âm.
    explicit DynamicBitset(int size = 0) {
        if (size < 0) throw invalid_argument("DynamicBitset: size âm");
        n = size;
        word.assign((size + 63) / 64, 0);
    }

    // Ý nghĩa: gán bit pos thành value.
    // Tham số: pos = chỉ số 0-based; value = giá trị mới.
    void set(int pos, bool value = true) {
        if (pos < 0 || pos >= n) throw out_of_range("DynamicBitset::set");
        uint64_t mask = uint64_t(1) << (pos & 63);
        if (value) word[pos >> 6] |= mask;
        else word[pos >> 6] &= ~mask;
    }

    // Ý nghĩa: đọc bit pos.
    // Tham số: pos = chỉ số 0-based.
    // Trả về: true nếu bit đang bật.
    bool test(int pos) const {
        if (pos < 0 || pos >= n) throw out_of_range("DynamicBitset::test");
        return (word[pos >> 6] >> (pos & 63)) & 1ULL;
    }

    // Ý nghĩa: đếm tổng số bit bật.
    // Trả về: popcount của toàn bitset.
    long long count() const {
        long long ans = 0;
        for (uint64_t x : word) ans += __builtin_popcountll(x);
        return ans;
    }

    // Ý nghĩa: thực hiện this |= (this << shift), đúng cho chuyển 0/1 subset-sum.
    // Tham số: shift = số vị trí dịch trái không âm.
    void shiftOr(int shift) {
        if (shift < 0) throw invalid_argument("DynamicBitset::shiftOr: shift âm");
        if (shift == 0 || word.empty()) return;
        int whole = shift >> 6, rem = shift & 63;
        for (int dst = (int)word.size() - 1; dst >= 0; --dst) {
            uint64_t moved = 0;
            int src = dst - whole;
            if (src >= 0) {
                moved |= word[src] << rem;
                if (rem && src - 1 >= 0) moved |= word[src - 1] >> (64 - rem);
            }
            word[dst] |= moved;
        }
        trim();
    }

    // Ý nghĩa: đếm bit cùng bật ở hai bitset.
    // Tham số: other = bitset cùng kích thước.
    // Trả về: popcount(this AND other).
    long long countAnd(const DynamicBitset& other) const {
        if (n != other.n) throw invalid_argument("DynamicBitset::countAnd: khác size");
        long long ans = 0;
        for (size_t i = 0; i < word.size(); ++i)
            ans += __builtin_popcountll(word[i] & other.word[i]);
        return ans;
    }

private:
    // Ý nghĩa: xóa các bit đệm ngoài [0,n) sau phép dịch.
    void trim() {
        if (!word.empty() && (n & 63)) word.back() &= (uint64_t(1) << (n & 63)) - 1;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dynamic-bitset.hpp -o demo && ./demo
int main() {
    DynamicBitset sums(16);
    sums.set(0);
    for (int x : {3, 5, 6}) sums.shiftOr(x);
    DynamicBitset row(16);
    row.set(8); row.set(14);
    printf("tạo được 8? %d; số tổng tạo được=%lld; giao=%lld\n",
           (int)sums.test(8), sums.count(), sums.countAnd(row));  // 1, 8, 2
    return 0;
}
#endif
