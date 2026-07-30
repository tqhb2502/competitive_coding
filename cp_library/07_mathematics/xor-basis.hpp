#pragma once
#include <bits/stdc++.h>
using namespace std;
// XOR Basis — cơ sở tuyến tính trên GF(2) của một tập số, lưu dạng bậc thang RÚT GỌN (reduced row echelon).
// Khi dùng: XOR lớn nhất của tập con; đếm số giá trị XOR phân biệt (= 2^rank); giá trị XOR nhỏ thứ k; hạng GF(2).
// ĐPT: insert / canForm O(B), maxXor O(B), kthXor O(B) với B = 64 bit (mỗi thao tác chỉ quét vài lượt qua 64 bit);
//      bộ nhớ O(B). Xử lý n số: O(n · B).
// Dùng: XorBasis xb; xb.insert(x); xb.maxXor(); xb.size(); xb.canForm(x); xb.kthXor(1); // k 1-based, kthXor(1) = 0
// Bẫy: mọi giá trị là unsigned long long (KHÔNG dùng số âm); tập giá trị XOR luôn chứa 0 (tập con rỗng) nên
//      kthXor cần 1 <= k <= 2^rank, k = 1 trả về 0; tồn tại tập con KHÁC rỗng có XOR = 0 <=> insert trả về false
//      ít nhất một lần (tức rank < số phần tử đã insert); số giá trị phân biệt là 2^rank, tự lấy mod nếu rank lớn.
// CSES: 3191 3192 3193 3211
struct XorBasis {
    // bas[b] = vector cơ sở có bit cao nhất (pivot) đúng bằng b, hoặc 0 nếu chưa có pivot tại b.
    // Bất biến RREF: nếu bas[b] != 0 thì bit b bằng 0 trong MỌI vector cơ sở khác => các pivot độc lập hoàn toàn.
    unsigned long long bas[64] = {};
    long long rank_ = 0;

    // Ý nghĩa: thêm giá trị x vào cơ sở, đồng thời giữ nguyên dạng bậc thang rút gọn.
    // Tham số: x = giá trị cần thêm (64 bit không dấu).
    // Trả về: true nếu x độc lập tuyến tính với cơ sở hiện tại (rank tăng 1), false nếu x biểu diễn được sẵn.
    bool insert(unsigned long long x) {
        for (int b = 63; b >= 0; b--) {
            if (!((x >> b) & 1ULL)) continue;
            if (bas[b]) { x ^= bas[b]; continue; }
            // x đã sạch mọi pivot > b; tiếp tục khử các pivot < b để x ở dạng rút gọn.
            for (int j = b - 1; j >= 0; j--)
                if (bas[j] && ((x >> j) & 1ULL)) x ^= bas[j];
            // Khử bit b khỏi các vector cơ sở cũ để bit b chỉ còn xuất hiện ở pivot mới.
            for (int j = b + 1; j < 64; j++)
                if (bas[j] && ((bas[j] >> b) & 1ULL)) bas[j] ^= x;
            bas[b] = x;
            rank_++;
            return true;
        }
        return false;   // x == 0 sau khi khử: đã nằm trong không gian sinh
    }
    // Ý nghĩa: kiểm tra x có phải XOR của một tập con nào đó của các giá trị đã insert.
    // Tham số: x = giá trị cần kiểm tra.
    // Trả về: true nếu x thuộc không gian sinh (x = 0 luôn được, ứng với tập con rỗng).
    bool canForm(unsigned long long x) const {
        for (int b = 63; b >= 0; b--)
            if (((x >> b) & 1ULL) && bas[b]) x ^= bas[b];
        return x == 0;
    }
    // Ý nghĩa: lấy hạng (số vector cơ sở) — số giá trị XOR phân biệt bằng 2^size().
    // Trả về: rank của tập giá trị trên GF(2), trong [0, 64].
    long long size() const { return rank_; }
    // Ý nghĩa: XOR lớn nhất lấy được từ một tập con của các giá trị đã insert.
    // Trả về: giá trị XOR lớn nhất (0 nếu chưa insert gì / mọi giá trị đều bằng 0).
    unsigned long long maxXor() const {
        unsigned long long res = 0;
        for (int b = 63; b >= 0; b--)
            if (bas[b] && !((res >> b) & 1ULL)) res ^= bas[b];
        return res;
    }
    // Ý nghĩa: giá trị XOR nhỏ thứ k trong 2^rank giá trị PHÂN BIỆT sinh được (sắp tăng dần).
    // Tham số: k = thứ hạng 1-based, yêu cầu 1 <= k <= 2^rank (k = 1 ứng với giá trị 0).
    //          Kiểu KHÔNG DẤU để rank = 64 vẫn truyền được k = 2^63 (long long sẽ tràn).
    // Trả về: giá trị XOR nhỏ thứ k.
    unsigned long long kthXor(unsigned long long k) const {
        unsigned long long res = 0;
        unsigned long long t = k - 1;   // đổi sang 0-based: bit i của t = có chọn vector cơ sở nhỏ thứ i hay không
        for (int b = 0; b < 64 && t != 0; b++) {
            if (!bas[b]) continue;
            if (t & 1) res ^= bas[b];   // nhờ RREF, pivot tăng dần đúng theo thứ tự độ lớn
            t >>= 1;
        }
        return res;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ xor-basis.hpp -o demo && ./demo
int main() {
    XorBasis xb;
    for (unsigned long long v : {3ULL, 5ULL, 6ULL}) xb.insert(v);   // 6 = 3 ^ 5 nên phụ thuộc
    printf("rank=%lld maxXor=%llu\n", xb.size(), xb.maxXor());      // rank=2 maxXor=6
    printf("canForm(6)=%d canForm(4)=%d\n", (int)xb.canForm(6), (int)xb.canForm(4));   // 1 0
    // 2^2 = 4 giá trị phân biệt sắp tăng: 0 3 5 6
    for (unsigned long long k = 1; k <= 4; k++) printf("%llu ", xb.kthXor(k));   // 0 3 5 6
    printf("\n");
    return 0;
}
#endif
