#pragma once
#include <bits/stdc++.h>
using namespace std;
// Prefix XOR — XOR đoạn mảng tĩnh trong O(1) bằng tiền tố.
// Khi dùng: nhiều truy vấn XOR trên đoạn, mảng không cập nhật; phép XOR có nghịch đảo là chính nó.
// ĐPT: build O(n), query O(1), bộ nhớ O(n). Index 0-based, đoạn nửa mở [l,r).
// Dùng: PrefixXor px(a); long long value = px.query(l, r);
// Bẫy: đoạn là [l,r), không phải [l,r]; rỗng trả 0; dùng uint64_t nếu cần đúng mẫu bit âm.
// CSES: 1650 1655 3233 3426
struct PrefixXor {
    vector<long long> pref;

    // Ý nghĩa: dựng pref[i] = XOR của i phần tử đầu.
    // Tham số: values = mảng 0-based.
    explicit PrefixXor(const vector<long long>& values) : pref(values.size() + 1, 0) {
        for (int i = 0; i < (int)values.size(); ++i) pref[i + 1] = pref[i] ^ values[i];
    }

    // Ý nghĩa: lấy XOR của đoạn nửa mở [l,r).
    // Tham số: l,r = biên 0-based, yêu cầu 0<=l<=r<=n.
    // Trả về: values[l] XOR ... XOR values[r-1], hoặc 0 nếu đoạn rỗng.
    long long query(int l, int r) const {
        if (l < 0 || l > r || r >= (int)pref.size()) throw out_of_range("PrefixXor::query");
        return pref[r] ^ pref[l];
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ prefix-xor.hpp -o demo && ./demo
int main() {
    PrefixXor px(vector<long long>{3, 2, 4, 5});
    printf("xor[1,4)=%lld; xor rỗng=%lld\n", px.query(1, 4), px.query(2, 2));  // 3, 0
    return 0;
}
#endif
