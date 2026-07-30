#pragma once
#include <bits/stdc++.h>
using namespace std;
// Mobius & Euler Phi — sàng tuyến tính O(n) tính hàm Mobius mu[] và hàm Euler phi[] cho mọi số tới n.
// Khi dùng: đếm theo nghịch đảo Mobius (đếm cặp nguyên tố cùng nhau, số không chia hết...), tổng phi, hàm nhân tính.
// ĐPT: dựng O(n); bộ nhớ O(n). mu[1]=phi[1]=1; mu[n] thuộc {-1,0,1}; phi[n] = số số trong [1,n] nguyên tố cùng nhau với n.
// Dùng: Mobius M(n); M.mu[x]; M.phi[x]; // cùng danh sách nguyên tố M.primes
// Bẫy: chỉ cho x <= n đã sàng; mu[x]=0 nếu x có thừa số bình phương; mu là hàm NHÂN TÍNH.
// CSES: 2417
struct Mobius {
    vector<int> mu, phi, spf, primes;
    // Ý nghĩa: sàng tuyến tính tính mu[i], phi[i], ước nguyên tố nhỏ nhất spf[i] và danh sách nguyên tố tới n.
    // Tham số: n = giới hạn trên của miền sàng.
    explicit Mobius(int n) : mu(n + 1, 0), phi(n + 1, 0), spf(n + 1, 0) {
        if (n >= 1) { mu[1] = 1; phi[1] = 1; }
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) { spf[i] = i; primes.push_back(i); mu[i] = -1; phi[i] = i - 1; }
            for (int p : primes) {
                if ((long long)i * p > n) break;
                spf[i * p] = p;
                if (i % p == 0) { mu[i * p] = 0; phi[i * p] = phi[i] * p; break; }
                mu[i * p] = -mu[i]; phi[i * p] = phi[i] * (p - 1);
            }
        }
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ mobius.hpp -o demo && ./demo
int main() {
    Mobius M(30);
    printf("mu[6]=%d mu[12]=%d mu[30]=%d ; phi[10]=%d phi[12]=%d\n",
           M.mu[6], M.mu[12], M.mu[30], M.phi[10], M.phi[12]);   // 1 0 -1 ; 4 4
    return 0;
}
#endif
