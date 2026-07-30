#pragma once
#include <bits/stdc++.h>
using namespace std;
// Labeled Combinatorics — phân phối số thành phần của functional graph và SCC của tournament gắn nhãn.
// Khi dùng: đếm cấu trúc theo đúng k thành phần; công thức Lagrange hoặc ghép dãy SCC có nhãn.
// ĐPT: functional graph O(n²)/O(n); tournament O(n³)/O(n²), phù hợp n<=500.
// Dùng: functionalGraphComponentDistribution(n,MOD); tournamentSccDistribution(n,MOD);
// Bẫy: mod phải nguyên tố và mod>n; vector có n+1 phần tử, index k là số thành phần; n=0 trả answer[0]=1.
// CSES: 2415 3232

// Functional graph dùng công thức Lagrange; tournament dùng DP trên dãy SCC.

namespace labeled_combinatorics_detail {

inline long long add(long long first, long long second, long long mod) {
    __int128 sum = static_cast<__int128>(first) + second;
    if (sum >= mod) sum -= mod;
    return static_cast<long long>(sum);
}

inline long long subtract(long long first, long long second, long long mod) {
    __int128 difference = static_cast<__int128>(first) - second;
    if (difference < 0) difference += mod;
    return static_cast<long long>(difference);
}

inline long long multiply(long long first, long long second, long long mod) {
    return static_cast<long long>(
        static_cast<__int128>(first) * second % mod);
}

inline long long power(long long base, long long exponent, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    if (base < 0) base += mod;
    while (exponent > 0) {
        if (exponent & 1LL) result = multiply(result, base, mod);
        base = multiply(base, base, mod);
        exponent >>= 1LL;
    }
    return result;
}

struct Tables {
    int limit;
    long long mod;
    vector<long long> factorial;
    vector<long long> inverseFactorial;
    vector<long long> inverse;

    Tables(int limit, long long mod)
        : limit(limit),
          mod(mod),
          factorial(limit + 1),
          inverseFactorial(limit + 1),
          inverse(limit + 1) {
        if (limit < 0)
            throw invalid_argument("labeled combinatorics: n âm");
        if (mod < 2 || mod <= limit)
            throw invalid_argument(
                "labeled combinatorics: cần modulus nguyên tố > n");
        factorial[0] = 1 % mod;
        for (int value = 1; value <= limit; ++value)
            factorial[value] =
                multiply(factorial[value - 1], value, mod);
        inverseFactorial[limit] =
            power(factorial[limit], mod - 2, mod);
        for (int value = limit; value >= 1; --value)
            inverseFactorial[value - 1] =
                multiply(inverseFactorial[value], value, mod);
        for (int value = 1; value <= limit; ++value)
            inverse[value] =
                multiply(factorial[value - 1],
                         inverseFactorial[value], mod);
    }

    long long combination(int total, int chosen) const {
        if (chosen < 0 || chosen > total || total < 0 || total > limit)
            return 0;
        return multiply(
            multiply(factorial[total], inverseFactorial[chosen], mod),
            inverseFactorial[total - chosen], mod);
    }
};

inline vector<long long> tournamentStrongCounts(
        int n, const Tables& tables) {
    const long long mod = tables.mod;
    vector<long long> all(n + 1), strong(n + 1);
    all[0] = 1 % mod;
    long long powerOfTwo = 1 % mod;  // 2^(size-1)
    for (int size = 1; size <= n; ++size) {
        all[size] = multiply(all[size - 1], powerOfTwo, mod);
        powerOfTwo = add(powerOfTwo, powerOfTwo, mod);

        long long nonStrong = 0;
        for (int firstSize = 1; firstSize < size; ++firstSize) {
            long long term =
                multiply(tables.combination(size, firstSize),
                         strong[firstSize], mod);
            term = multiply(term, all[size - firstSize], mod);
            nonStrong = add(nonStrong, term, mod);
        }
        strong[size] = subtract(all[size], nonStrong, mod);
    }
    return strong;
}

}  // namespace labeled_combinatorics_detail

// Ý nghĩa: đếm functional graph trên n đỉnh gắn nhãn theo số thành phần yếu.
// Công thức dùng EGF T=x*exp(T), F(x,y)=(1-T)^(-y) và đảo Lagrange.
// Trả về: answer[k] = số functional graph có đúng k thành phần, modulo mod.
inline vector<long long> functionalGraphComponentDistribution(
        int n, long long mod) {
    if (n < 0)
        throw invalid_argument(
            "functionalGraphComponentDistribution: n âm");
    labeled_combinatorics_detail::Tables tables(n, mod);
    vector<long long> answer(n + 1);
    if (n == 0) {
        answer[0] = 1 % mod;
        return answer;
    }

    vector<long long> powerOfN(n + 1), binomialPolynomial(n),
        sumPolynomial(n);
    powerOfN[0] = 1 % mod;
    for (int exponent = 1; exponent <= n; ++exponent)
        powerOfN[exponent] =
            labeled_combinatorics_detail::multiply(
                powerOfN[exponent - 1], n, mod);

    // Q_r(y)=C(y+r,r); Q_{r+1}=Q_r*(1+y/(r+1)).
    binomialPolynomial[0] = 1 % mod;
    for (int r = 0; r < n; ++r) {
        const int exponent = n - 1 - r;
        const long long weight =
            labeled_combinatorics_detail::multiply(
                powerOfN[exponent],
                tables.inverseFactorial[exponent], mod);
        for (int degree = 0; degree <= r; ++degree) {
            const long long term =
                labeled_combinatorics_detail::multiply(
                    weight, binomialPolynomial[degree], mod);
            sumPolynomial[degree] =
                labeled_combinatorics_detail::add(
                    sumPolynomial[degree], term, mod);
        }
        if (r + 1 < n) {
            const long long inverseDenominator = tables.inverse[r + 1];
            for (int degree = r + 1; degree >= 1; --degree) {
                const long long term =
                    labeled_combinatorics_detail::multiply(
                        binomialPolynomial[degree - 1],
                        inverseDenominator, mod);
                binomialPolynomial[degree] =
                    labeled_combinatorics_detail::add(
                        binomialPolynomial[degree], term, mod);
            }
        }
    }

    // n![x^n y^k]F = (n-1)! * [y^(k-1)]sum_r w_r Q_r(y).
    const long long scale = tables.factorial[n - 1];
    for (int components = 1; components <= n; ++components)
        answer[components] =
            labeled_combinatorics_detail::multiply(
                scale, sumPolynomial[components - 1], mod);
    return answer;
}

// Ý nghĩa: đếm tournament liên thông mạnh trên m đỉnh, cho mọi 0<=m<=n.
// Trả về: strong[m]; strong[0]=0. Dùng
// all[m]=sum_s C(m,s)*strong[s]*all[m-s], với SCC đầu tiên là duy nhất.
inline vector<long long> stronglyConnectedTournamentCounts(
        int n, long long mod) {
    if (n < 0)
        throw invalid_argument(
            "stronglyConnectedTournamentCounts: n âm");
    labeled_combinatorics_detail::Tables tables(n, mod);
    return labeled_combinatorics_detail::tournamentStrongCounts(n, tables);
}

// Ý nghĩa: đếm tournament n đỉnh gắn nhãn theo số SCC.
// Các SCC của tournament có một thứ tự toàn phần duy nhất; cạnh giữa hai SCC
// bị ép hướng, nên DP ghép một dãy các tournament liên thông mạnh.
// Trả về: answer[k] = số tournament có đúng k SCC, modulo mod.
inline vector<long long> tournamentSccDistribution(int n, long long mod) {
    if (n < 0)
        throw invalid_argument("tournamentSccDistribution: n âm");
    labeled_combinatorics_detail::Tables tables(n, mod);
    const vector<long long> strong =
        labeled_combinatorics_detail::tournamentStrongCounts(n, tables);

    vector<vector<long long>> transition(
        n + 1, vector<long long>(n + 1));
    for (int total = 1; total <= n; ++total)
        for (int componentSize = 1; componentSize <= total;
             ++componentSize)
            transition[total][componentSize] =
                labeled_combinatorics_detail::multiply(
                    tables.combination(total, componentSize),
                    strong[componentSize], mod);

    vector<vector<long long>> distribution(
        n + 1, vector<long long>(n + 1));
    distribution[0][0] = 1 % mod;
    for (int components = 1; components <= n; ++components) {
        for (int total = components; total <= n; ++total) {
            const int maximumLastSize = total - components + 1;
            long long ways = 0;
            for (int lastSize = 1; lastSize <= maximumLastSize; ++lastSize) {
                const long long term =
                    labeled_combinatorics_detail::multiply(
                        transition[total][lastSize],
                        distribution[total - lastSize][components - 1],
                        mod);
                ways = labeled_combinatorics_detail::add(ways, term, mod);
            }
            distribution[total][components] = ways;
        }
    }
    return distribution[n];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ labeled-combinatorics.hpp -o demo && ./demo
int main() {
    constexpr long long MOD = 1000000007;
    auto functional = functionalGraphComponentDistribution(2, MOD);
    auto tournament = tournamentSccDistribution(3, MOD);
    printf("Functional graph n=2 (k=1..2): %lld %lld\n",
           functional[1], functional[2]); // 3 1
    printf("Tournament n=3 (k=1..3): %lld %lld %lld\n",
           tournament[1], tournament[2], tournament[3]); // 2 0 6
    return 0;
}
#endif
