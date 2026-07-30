// Lời giải NGÂY THƠ — chậm nhưng chắc đúng, dùng làm mốc so trong stress test.
// Biên dịch: g++ -std=c++17 -O2 brute.cpp -o brute
// Dùng cùng: bash cp_library/templates/stress.sh main.cpp brute.cpp gen.cpp 500
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;              // hết input -> thoát êm
    vector<long long> a(n);
    for (auto& x : a) cin >> x;
    // TODO: giải bằng cách ĐƠN GIẢN NHẤT (duyệt toàn bộ / O(n^3) cũng được) — không cần nhanh.
    long long sum = 0;
    for (long long x : a) sum += x;
    cout << sum << '\n';
    return 0;
}
