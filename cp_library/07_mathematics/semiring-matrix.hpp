#pragma once
#include <bits/stdc++.h>
using namespace std;
// Semiring Matrix — ma trận tổng quát theo (add,multiply); kèm min-plus cho chi phí nhỏ nhất dùng đúng k cạnh.
// Khi dùng: matrix exponentiation mà phép cộng/nhân không phải +/*; min-plus shortest walk; boolean reachability.
// ĐPT: nhân O(r*k*c), pow O(n^3 log e), vector*pow O(n^3 log e + n^2 log e); bộ nhớ O(n^2).
// Dùng: MinPlusSemiring mp(INF); SemiringMatrix<MinPlusSemiring> m(n, mp); auto r=m.pow(k);
// Bẫy: pow chỉ cho ma trận vuông, e >= 0; hai toán hạng phải dùng cùng semiring; INF phải lớn hơn mọi đáp án thật.
// CSES: 1724
struct MinPlusSemiring {
    using Value = long long;
    long long inf;

    // Ý nghĩa: tạo semiring min-plus với phần tử cộng đơn vị là inf.
    // Tham số: infinity = giá trị lớn hơn mọi chi phí hữu hạn cần biểu diễn.
    explicit MinPlusSemiring(long long infinity = (1LL << 62)) : inf(infinity) {
        if (inf <= 0) throw invalid_argument("MinPlusSemiring: infinity must be positive");
    }

    // Ý nghĩa: lấy phần tử đơn vị của phép cộng min.
    // Trả về: inf.
    long long zero() const { return inf; }

    // Ý nghĩa: lấy phần tử đơn vị của phép nhân cộng số học.
    // Trả về: 0.
    long long one() const { return 0; }

    // Ý nghĩa: phép cộng của semiring.
    // Tham số: a, b = hai chi phí.
    // Trả về: min(a,b).
    long long add(long long a, long long b) const { return min(a, b); }

    // Ý nghĩa: phép nhân min-plus, cộng hai chi phí và chặn an toàn tại [-inf, inf].
    // Tham số: a, b = hai chi phí; inf biểu diễn không tồn tại.
    // Trả về: inf nếu một toán hạng là inf, ngược lại là tổng đã chặn để không tràn long long.
    long long multiply(long long a, long long b) const {
        if (a == inf || b == inf) return inf;
        __int128 sum = (__int128)a + b;
        if (sum >= inf) return inf;
        if (sum <= -(__int128)inf) return -inf;
        return (long long)sum;
    }
};

template<class Semiring>
struct SemiringMatrix {
    using Value = typename Semiring::Value;
    int rows, cols;
    Semiring semiring;
    vector<vector<Value>> a;

    // Ý nghĩa: tạo ma trận rows x cols chứa toàn phần tử zero của semiring.
    // Tham số: rows, cols = số hàng/cột; semiring = chính sách zero/one/add/multiply.
    SemiringMatrix(int rows, int cols, const Semiring& semiring = Semiring())
        : rows(rows), cols(cols), semiring(semiring),
          a(rows < 0 ? 0 : (size_t)rows,
            vector<Value>(cols < 0 ? 0 : (size_t)cols, semiring.zero())) {
        if (rows < 0 || cols < 0) throw invalid_argument("SemiringMatrix: negative dimension");
    }

    // Ý nghĩa: tạo ma trận vuông n x n chứa zero.
    // Tham số: n = kích thước; semiring = chính sách phép toán.
    explicit SemiringMatrix(int n, const Semiring& semiring = Semiring())
        : SemiringMatrix(n, n, semiring) {}

    // Ý nghĩa: tạo ma trận đơn vị vuông theo semiring.
    // Tham số: n = kích thước; semiring = chính sách phép toán.
    // Trả về: ma trận có one trên đường chéo và zero ngoài đường chéo.
    static SemiringMatrix identity(int n, const Semiring& semiring = Semiring()) {
        SemiringMatrix result(n, semiring);
        for (int i = 0; i < n; i++) result.a[i][i] = semiring.one();
        return result;
    }

    // Ý nghĩa: nhân ma trận theo các phép toán của semiring.
    // Tham số: other = ma trận bên phải, có rows bằng cols của ma trận hiện tại.
    // Trả về: tích kích thước rows x other.cols.
    SemiringMatrix operator*(const SemiringMatrix& other) const {
        if (cols != other.rows) throw invalid_argument("SemiringMatrix: incompatible dimensions");
        SemiringMatrix result(rows, other.cols, semiring);
        Value zero = semiring.zero();
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (a[i][k] == zero) continue;
                for (int j = 0; j < other.cols; j++) {
                    if (other.a[k][j] == zero) continue;
                    result.a[i][j] = semiring.add(
                        result.a[i][j], semiring.multiply(a[i][k], other.a[k][j]));
                }
            }
        }
        return result;
    }

    // Ý nghĩa: lũy thừa ma trận vuông bằng bình phương liên tiếp.
    // Tham số: exponent = số mũ không âm.
    // Trả về: ma trận this^exponent theo semiring.
    SemiringMatrix pow(long long exponent) const {
        if (rows != cols || exponent < 0) throw invalid_argument("SemiringMatrix::pow: need square matrix and e >= 0");
        SemiringMatrix result = identity(rows, semiring), base = *this;
        while (exponent > 0) {
            if (exponent & 1) result = result * base;
            exponent >>= 1;
            if (exponent) base = base * base;
        }
        return result;
    }

    // Ý nghĩa: tính row_vector * this^exponent, tránh nhân result matrix ở các bit 1.
    // Tham số: row_vector = vector hàng cỡ rows; exponent = số mũ không âm.
    // Trả về: vector hàng sau lũy thừa.
    vector<Value> applyPowerToRow(vector<Value> row_vector, long long exponent) const {
        if (rows != cols || (int)row_vector.size() != rows || exponent < 0)
            throw invalid_argument("SemiringMatrix::applyPowerToRow: invalid dimension or exponent");
        SemiringMatrix base = *this;
        Value zero = semiring.zero();
        while (exponent > 0) {
            if (exponent & 1) {
                vector<Value> next(cols, zero);
                for (int i = 0; i < rows; i++) {
                    if (row_vector[i] == zero) continue;
                    for (int j = 0; j < cols; j++) {
                        if (base.a[i][j] == zero) continue;
                        next[j] = semiring.add(
                            next[j], semiring.multiply(row_vector[i], base.a[i][j]));
                    }
                }
                row_vector.swap(next);
            }
            exponent >>= 1;
            if (exponent) base = base * base;
        }
        return row_vector;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ semiring-matrix.hpp -o demo && ./demo
int main() {
    const long long INF = 1LL << 62;
    MinPlusSemiring min_plus(INF);
    SemiringMatrix<MinPlusSemiring> graph(3, min_plus);
    graph.a[0][1] = 4;
    graph.a[1][2] = 7;
    graph.a[0][2] = 20;
    auto exactly_two = graph.pow(2);
    printf("chi phí 0->2 dùng đúng 2 cạnh = %lld\n", exactly_two.a[0][2]);  // 11
    return 0;
}
#endif
