#pragma once
#include <bits/stdc++.h>
using namespace std;
// Permutation Cycles — kiểm tra/đảo hoán vị, phân rã chu trình và tính bậc bằng LCM.
// Khi dùng: lặp hoán vị, số vòng trở về identity, xử lý từng cycle, đổi giữa permutation và inverse.
// ĐPT: mọi hàm O(n); bộ nhớ O(n).
// Dùng: auto cyc=permutationCycles(p); auto inv=inversePermutation(p); permutationOrderCapped(p, cap);
// Bẫy: p phải là hoán vị 0-based của [0,n); bậc có thể cực lớn nên hàm order nhận cap và trả cap+1 khi vượt.
// CSES: 1162 1698 2216 2217 3398 3422
// Ý nghĩa: kiểm tra p có chứa đúng mỗi giá trị 0..n-1 một lần hay không.
// Tham số: p = dãy ứng viên.
// Trả về: true khi p là hoán vị 0-based hợp lệ.
bool isZeroBasedPermutation(const vector<int>& p) {
    vector<char> seen(p.size(), false);
    for (int x : p) {
        if (x < 0 || x >= (int)p.size() || seen[x]) return false;
        seen[x] = true;
    }
    return true;
}

// Ý nghĩa: tính hoán vị nghịch đảo inv[p[i]] = i.
// Tham số: p = hoán vị 0-based hợp lệ.
// Trả về: hoán vị nghịch đảo; ném invalid_argument nếu p sai.
vector<int> inversePermutation(const vector<int>& p) {
    if (!isZeroBasedPermutation(p)) throw invalid_argument("inversePermutation: p không hợp lệ");
    vector<int> inv(p.size());
    for (int i = 0; i < (int)p.size(); ++i) inv[p[i]] = i;
    return inv;
}

// Ý nghĩa: phân rã p thành các chu trình rời nhau, mỗi đỉnh xuất hiện đúng một lần.
// Tham số: p = hoán vị 0-based hợp lệ.
// Trả về: danh sách chu trình theo đỉnh nhỏ nhất chưa thăm.
vector<vector<int>> permutationCycles(const vector<int>& p) {
    if (!isZeroBasedPermutation(p)) throw invalid_argument("permutationCycles: p không hợp lệ");
    vector<vector<int>> cycles;
    vector<char> seen(p.size(), false);
    for (int s = 0; s < (int)p.size(); ++s) if (!seen[s]) {
        vector<int> cycle;
        for (int u = s; !seen[u]; u = p[u]) {
            seen[u] = true;
            cycle.push_back(u);
        }
        cycles.push_back(std::move(cycle));
    }
    return cycles;
}

// Ý nghĩa: tính bậc của hoán vị = LCM độ dài chu trình, có chặn tràn.
// Tham số: p = hoán vị 0-based; cap = giá trị lớn nhất muốn phân biệt (>=0).
// Trả về: bậc nếu <=cap, ngược lại cap+1.
long long permutationOrderCapped(const vector<int>& p, long long cap = LLONG_MAX - 1) {
    if (cap < 0 || cap == LLONG_MAX) throw invalid_argument("permutationOrderCapped: cap không hợp lệ");
    long long order = 1;
    for (const auto& cycle : permutationCycles(p)) {
        long long len = (long long)cycle.size();
        long long g = std::gcd(order, len);
        __int128 next = (__int128)(order / g) * len;
        if (next > cap) return cap + 1;
        order = (long long)next;
    }
    return order;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ permutation-cycles.hpp -o demo && ./demo
int main() {
    vector<int> p = {1, 2, 0, 4, 3};  // (0 1 2)(3 4)
    auto cycles = permutationCycles(p);
    printf("cycles=%zu, order=%lld, inv[p[0]]=%d\n",
           cycles.size(), permutationOrderCapped(p, 100), inversePermutation(p)[p[0]]);
    return 0;
}
#endif
