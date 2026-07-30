#pragma once
#include <bits/stdc++.h>
using namespace std;
// Permutation Code — đổi hoán vị 0-based ↔ Lehmer code (nhỏ hơn bên phải) và ↔ prefix-inversion code (lớn hơn bên trái).
// Khi dùng: khôi phục hoán vị từ số nghịch thế tăng thêm theo từng vị trí; xếp hạng/giải mã hoán vị; chọn phần tử còn trống theo thứ tự.
// ĐPT: mọi phép encode/decode O(n log n); bộ nhớ O(n), dùng Fenwick tần suất + order statistic.
// Dùng: auto l = permutationToLehmerCode(p); lehmerCodeToPermutation(l);
//        auto c = permutationToPrefixInversionCode(p); prefixInversionCodeToPermutation(c);
// Bẫy: p phải chứa đúng 0..n-1; Lehmer l[i] thuộc [0,n-1-i], còn prefix code c[i] thuộc [0,i] — hai quy ước KHÔNG hoán đổi trực tiếp.
//       Tổng các chữ số của cả hai code đều bằng số nghịch thế; decode trả hoán vị GIÁ TRỊ 0-based, không phải thứ tự chèn.
// CSES: 3140

namespace permutation_code_detail {
struct PermutationCodeFenwick {
    int n;
    vector<int> tree;

    // Ý nghĩa: tạo Fenwick tần suất n vị trí, ban đầu đều bằng 0.
    // Tham số: n = số vị trí, phải không âm.
    explicit PermutationCodeFenwick(int n) : n(n), tree(n + 1, 0) {
        if (n < 0) throw invalid_argument("PermutationCodeFenwick: n âm");
    }

    // Ý nghĩa: cộng delta vào tần suất tại index 0-based.
    // Tham số: index = vị trí trong [0,n); delta = lượng cộng (các API decode duy trì tần suất 0/1).
    void add(int index, int delta) {
        for (int i = index + 1; i <= n; i += i & -i) tree[i] += delta;
    }

    // Ý nghĩa: đếm tổng tần suất trên đoạn nửa mở [0,right).
    // Tham số: right = biên phải 0-based trong [0,n].
    // Trả về: tổng tree logic tại các index < right.
    int prefixCount(int right) const {
        int answer = 0;
        for (int i = right; i > 0; i -= i & -i) answer += tree[i];
        return answer;
    }

    // Ý nghĩa: tìm vị trí của phần tử thứ order trong multiset tần suất bằng binary lifting Fenwick.
    // Tham số: order = thứ tự 0-based, phải thuộc [0, tổng_tần_suất).
    // Trả về: index 0-based nhỏ nhất sao cho tổng tiền tố tới và gồm index lớn hơn order.
    int indexByOrder(int order) const {
        int total = prefixCount(n);
        if (order < 0 || order >= total)
            throw invalid_argument("PermutationCodeFenwick::indexByOrder: order ngoài miền");
        int index = 0;
        int step = 1;
        while ((long long)step * 2 <= n) step *= 2;
        for (; step > 0; step >>= 1) {
            int next = index + step;
            if (next <= n && tree[next] <= order) {
                index = next;
                order -= tree[next];
            }
        }
        return index;  // index đang là số vị trí đứng trước đáp án, cũng chính là index 0-based.
    }
};

// Ý nghĩa: kiểm tra và từ chối nếu p không phải hoán vị 0-based của [0,n).
// Tham số: p = dãy cần kiểm tra; caller = tên API để tạo thông báo lỗi rõ nghĩa.
inline void requirePermutation(const vector<int>& p, const char* caller) {
    vector<char> seen(p.size(), false);
    for (int value : p) {
        if (value < 0 || value >= (int)p.size() || seen[value])
            throw invalid_argument(string(caller) + ": p không phải hoán vị 0-based");
        seen[value] = true;
    }
}
}  // namespace permutation_code_detail

// Ý nghĩa: mã hoá p thành Lehmer code, code[i] = số phần tử bên PHẢI nhỏ hơn p[i].
// Tham số: p = hoán vị 0-based của [0,n).
// Trả về: vector code với 0 <= code[i] <= n-1-i; tổng code là số nghịch thế.
inline vector<int> permutationToLehmerCode(const vector<int>& p) {
    permutation_code_detail::requirePermutation(p, "permutationToLehmerCode");
    int n = (int)p.size();
    permutation_code_detail::PermutationCodeFenwick frequency(n);
    vector<int> code(n);
    for (int i = n - 1; i >= 0; --i) {
        code[i] = frequency.prefixCount(p[i]);
        frequency.add(p[i], 1);
    }
    return code;
}

// Ý nghĩa: giải Lehmer code bằng cách lấy phần tử chưa dùng thứ code[i] từ trái sang phải.
// Tham số: code = dãy độ dài n với 0 <= code[i] <= n-1-i.
// Trả về: hoán vị 0-based duy nhất có Lehmer code đã cho; ném invalid_argument nếu code sai.
inline vector<int> lehmerCodeToPermutation(const vector<int>& code) {
    int n = (int)code.size();
    permutation_code_detail::PermutationCodeFenwick available(n);
    for (int value = 0; value < n; ++value) available.add(value, 1);
    vector<int> permutation(n);
    for (int i = 0; i < n; ++i) {
        if (code[i] < 0 || code[i] > n - 1 - i)
            throw invalid_argument("lehmerCodeToPermutation: chữ số ngoài [0,n-1-i]");
        permutation[i] = available.indexByOrder(code[i]);
        available.add(permutation[i], -1);
    }
    return permutation;
}

// Ý nghĩa: mã hoá p thành prefix-inversion code, code[i] = số phần tử bên TRÁI lớn hơn p[i].
// Tham số: p = hoán vị 0-based của [0,n).
// Trả về: vector code với 0 <= code[i] <= i; tổng code là số nghịch thế.
inline vector<int> permutationToPrefixInversionCode(const vector<int>& p) {
    permutation_code_detail::requirePermutation(p, "permutationToPrefixInversionCode");
    int n = (int)p.size();
    permutation_code_detail::PermutationCodeFenwick frequency(n);
    vector<int> code(n);
    for (int i = 0; i < n; ++i) {
        int smallerOnLeft = frequency.prefixCount(p[i]);
        code[i] = i - smallerOnLeft;
        frequency.add(p[i], 1);
    }
    return code;
}

// Ý nghĩa: giải prefix-inversion code; khi đi i giảm dần, p[i] là phần tử chưa dùng hạng i-code[i] (0-based).
// Tham số: code = dãy độ dài n với 0 <= code[i] <= i.
// Trả về: hoán vị 0-based duy nhất có prefix-inversion code đã cho; ném invalid_argument nếu code sai.
inline vector<int> prefixInversionCodeToPermutation(const vector<int>& code) {
    int n = (int)code.size();
    permutation_code_detail::PermutationCodeFenwick available(n);
    for (int value = 0; value < n; ++value) available.add(value, 1);
    vector<int> permutation(n);
    for (int i = n - 1; i >= 0; --i) {
        if (code[i] < 0 || code[i] > i)
            throw invalid_argument("prefixInversionCodeToPermutation: chữ số ngoài [0,i]");
        int order = i - code[i];
        permutation[i] = available.indexByOrder(order);
        available.add(permutation[i], -1);
    }
    return permutation;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ permutation-code.hpp -o demo && ./demo
int main() {
    vector<int> p = {3, 0, 2, 1};
    auto lehmer = permutationToLehmerCode(p);          // [3,0,1,0]
    auto prefix = permutationToPrefixInversionCode(p); // [0,1,1,2]
    printf("lehmer:");
    for (int x : lehmer) printf(" %d", x);
    printf("; prefix:");
    for (int x : prefix) printf(" %d", x);
    printf("; round-trip=%d/%d\n",
           lehmerCodeToPermutation(lehmer) == p,
           prefixInversionCodeToPermutation(prefix) == p);
    // lehmer: 3 0 1 0; prefix: 0 1 1 2; round-trip=1/1
    return 0;
}
#endif
