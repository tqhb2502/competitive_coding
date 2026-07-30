#pragma once
#include <bits/stdc++.h>
using namespace std;
// Convex Hull Trick (đơn điệu) — MIN của các đường a*x+b khi slope thêm KHÔNG TĂNG & query x TĂNG dần.
// Khi dùng: DP dp[i]=min_j(a_j*x_i + b_j) với a_j giảm dần theo j, x_i không giảm. Nhẹ hơn Li Chao.
// ĐPT: add O(1) khấu hao, query O(1) khấu hao (con trỏ chỉ tiến); bộ nhớ O(n).
// Dùng: CHT h; h.add(a, b); // slope a phải KHÔNG TĂNG; h.query(x); // x phải KHÔNG giảm
// Bẫy: CHỈ đúng với hai thứ tự trên; slope trùng tự giữ intercept nhỏ hơn; kết quả a*x+b phải vừa long long.
// CSES: 2084 3427 3428
struct CHT {
    using Wide = __int128;
    using UWide = unsigned __int128;
    vector<long long> A, B;  // các đường theo thứ tự thêm (slope giảm dần)
    int ptr = 0;
    optional<long long> lastQueryX;
    // So sánh lhsA*lhsB <= rhsA*rhsB chính xác. Mỗi thừa số là hiệu của hai
    // long long nên trị tuyệt đối tối đa 2^64-1: tích vừa khít unsigned __int128,
    // nhưng có thể vượt miền signed __int128.
    static UWide magnitude(Wide value) {
        return value >= 0 ? (UWide)value : (UWide)(-(value + 1)) + 1;
    }
    static int productSign(Wide first, Wide second) {
        if (first == 0 || second == 0) return 0;
        return (first < 0) == (second < 0) ? 1 : -1;
    }
    static bool productLessEqual(Wide lhsA, Wide lhsB, Wide rhsA, Wide rhsB) {
        int lhsSign = productSign(lhsA, lhsB);
        int rhsSign = productSign(rhsA, rhsB);
        if (lhsSign != rhsSign) return lhsSign < rhsSign;
        if (lhsSign == 0) return true;
        UWide lhs = magnitude(lhsA) * magnitude(lhsB);
        UWide rhs = magnitude(rhsA) * magnitude(rhsB);
        return lhsSign > 0 ? lhs <= rhs : lhs >= rhs;
    }
    // Ý nghĩa: kiểm tra đường j có bị đường mới (na, nb) làm dư thừa so với đường i hay không.
    // Tham số: i = chỉ số đường ngoài; j = chỉ số đường giữa; na, nb = slope và hệ số tự do đường mới.
    // Trả về: true nếu đường j thừa (giao điểm mới không nằm bên phải giao (i, j)), cần loại bỏ.
    bool bad(int i, int j, long long na, long long nb) const {
        return productLessEqual((Wide)nb - B[i], (Wide)A[i] - A[j],
                                (Wide)B[j] - B[i], (Wide)A[i] - na);
    }
    // Ý nghĩa: thêm đường a*x+b vào bao, loại các đường cuối trở nên dư thừa để giữ tính lồi.
    // Tham số: a = slope (phải không tăng qua các lần add); b = hệ số tự do.
    void add(long long a, long long b) {
        if (!A.empty() && a > A.back())
            throw invalid_argument("CHT::add: slope phải không tăng");
        if (!A.empty() && a == A.back()) {
            if (b >= B.back()) return;  // cùng slope: đường mới không bao giờ tốt hơn
            A.pop_back();
            B.pop_back();
            if (ptr >= (int)A.size()) ptr = max(0, (int)A.size() - 1);
        }
        while (A.size() >= 2 && bad((int)A.size() - 2, (int)A.size() - 1, a, b)) {
            A.pop_back(); B.pop_back();
            if (ptr >= (int)A.size()) ptr = (int)A.size() - 1;
        }
        A.push_back(a); B.push_back(b);
    }
    // Ý nghĩa: tính chính xác bằng __int128 để việc SO SÁNH không tràn, kể cả khi tích tạm vượt long long.
    // Tham số: i = chỉ số đường; x = điểm cần tính.
    // Trả về: A[i]*x + B[i] ở kiểu __int128.
    Wide eval128(int i, long long x) const { return (Wide)A[i] * x + B[i]; }
    // Ý nghĩa: tính giá trị đường thứ i tại x.
    // Tham số: i = chỉ số đường; x = điểm cần tính.
    // Trả về: A[i]*x + B[i]; yêu cầu kết quả vừa trong long long.
    long long f(int i, long long x) const {
        Wide value = eval128(i, x);
        if (value < LLONG_MIN || value > LLONG_MAX)
            throw overflow_error("CHT: giá trị đường vượt long long");
        return (long long)value;
    }
    // Ý nghĩa: trả về MIN của mọi đường tại x, dùng con trỏ ptr chỉ tiến (x phải không giảm).
    // Tham số: x = điểm truy vấn (không giảm giữa các lần gọi).
    // Trả về: giá trị nhỏ nhất min_i(A[i]*x + B[i]); LLONG_MAX nếu chưa có đường.
    long long query(long long x) {
        if (A.empty()) return LLONG_MAX;
        if (lastQueryX && x < *lastQueryX)
            throw invalid_argument("CHT::query: x phải không giảm");
        lastQueryX = x;
        if (ptr >= (int)A.size()) ptr = (int)A.size() - 1;
        while (ptr + 1 < (int)A.size() && eval128(ptr + 1, x) <= eval128(ptr, x)) ptr++;
        return f(ptr, x);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ convex-hull-trick.hpp -o demo && ./demo
int main() {
    CHT h;
    h.add(3, 0);     // slope giảm dần: 3, 1, -2
    h.add(3, -1);    // slope trùng: tự giữ intercept -1 tốt hơn
    h.add(1, 4);
    h.add(-2, 10);
    printf("min tại x=1: %lld  x=5: %lld\n", h.query(1), h.query(5));   // 2  0  (x tăng dần)
    return 0;
}
#endif
