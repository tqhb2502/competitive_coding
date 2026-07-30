#pragma once
#include <bits/stdc++.h>
using namespace std;
// Extended Euclid — gcd mở rộng (tìm x, y: a*x + b*y = gcd), nghịch đảo modulo BẤT KỲ, và định lý số dư CRT.
// Khi dùng: nghịch đảo modulo khi mod KHÔNG nguyên tố; giải hệ đồng dư (CRT); nghiệm phương trình a*x + b*y = c.
// ĐPT: extgcd O(log min(a, b)); crt O(log); bộ nhớ O(1). invMod cần gcd(a, m) = 1.
// Dùng: long long g = extgcd(a, b, x, y); invMod(a, m); auto [r, M] = crt(r1, m1, r2, m2);
// Bẫy: invMod trả -1 nếu gcd(a, m) != 1; crt trả {-1, -1} nếu vô nghiệm; kết quả CRT trong [0, lcm(m1, m2)).
// CSES: 3214 3216
// Ý nghĩa: gcd mở rộng: tìm x, y sao cho a*x + b*y = gcd(a, b).
// Tham số: a, b = hai số; x, y = tham chiếu nhận hệ số Bézout.
// Trả về: gcd(a, b).
long long extgcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = (a < 0) ? -1 : 1; y = 0; return (a < 0) ? -a : a; }   // gcd không âm, giữ a*x = |a|
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1; y = x1 - (a / b) * y1;
    return g;
}
// Ý nghĩa: nghịch đảo modulo của a theo m BẤT KỲ (không cần m nguyên tố).
// Tham số: a = số cần nghịch đảo; m = modulo (> 0).
// Trả về: a^{-1} mod m trong [0, m), hoặc -1 nếu gcd(a, m) != 1.
long long invMod(long long a, long long m) {
    long long x, y;
    long long g = extgcd(((a % m) + m) % m, m, x, y);
    if (g != 1) return -1;
    return ((x % m) + m) % m;
}
// Ý nghĩa: giải hệ đồng dư x ≡ r1 (mod m1), x ≡ r2 (mod m2) bằng định lý số dư Trung Hoa.
// Tham số: r1, m1, r2, m2 = hai đồng dư (m1, m2 > 0).
// Trả về: cặp {nghiệm nhỏ nhất không âm, lcm(m1, m2)}, hoặc {-1, -1} nếu vô nghiệm.
pair<long long, long long> crt(long long r1, long long m1, long long r2, long long m2) {
    long long x, y;
    long long g = extgcd(m1, m2, x, y);
    if ((r2 - r1) % g != 0) return {-1, -1};
    long long lcm = m1 / g * m2;
    long long t = (long long)((__int128)((r2 - r1) / g % (m2 / g)) * (x % (m2 / g)) % (m2 / g));
    long long res = ((r1 + (__int128)m1 * t) % lcm + lcm) % lcm;
    return {res, lcm};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ extended-euclid.hpp -o demo && ./demo
int main() {
    long long x, y;
    long long g = extgcd(12, 18, x, y);
    auto [r, M] = crt(2, 3, 3, 5);
    printf("gcd(12,18)=%lld ; nghịch đảo 3 mod 7 = %lld ; CRT(2 mod 3, 3 mod 5) = %lld mod %lld\n",
           g, invMod(3, 7), r, M);   // 6 ; 5 ; 8 mod 15
    return 0;
}
#endif
