#pragma once
#include <bits/stdc++.h>
using namespace std;
// Divisor Sieve — đếm số phần tử là bội của mọi d và đảo theo bội để đếm tập con có gcd chính xác.
// Khi dùng: gcd lớn nhất của một nhóm phần tử; đếm subset theo gcd; dữ liệu dương có giá trị tối đa vừa phải.
// ĐPT: dựng và exactGcdSubsetCounts O(M log M), largest O(M); bộ nhớ O(M+n), M=max(max_value, max(values)).
// Dùng: DivisorSieve ds(values); ds.largestDivisorWithAtLeast(2); auto exact=ds.exactGcdSubsetCounts();
// Bẫy: chỉ nhận số dương <= M; exact đếm subset theo chỉ số phần tử (hai giá trị bằng nhau vẫn là hai lựa chọn); mod dương.
// CSES: 1081 3161
struct DivisorSieve {
    int limit;
    vector<long long> frequency;
    vector<long long> multiple_count;
    long long element_count;

    // Ý nghĩa: lập bảng tần suất và số phần tử chia hết cho từng d bằng harmonic sieve.
    // Tham số: values = danh sách số nguyên dương; max_value = miền tối thiểu cần sàng (0 để tự suy ra).
    explicit DivisorSieve(const vector<int>& values, int max_value = 0)
        : limit(max_value), element_count((long long)values.size()) {
        if (max_value < 0) throw invalid_argument("DivisorSieve: negative max_value");
        for (int value : values) {
            if (value <= 0) throw invalid_argument("DivisorSieve: values must be positive");
            limit = max(limit, value);
        }
        frequency.assign(limit + 1, 0);
        for (int value : values) frequency[value]++;
        multiple_count.assign(limit + 1, 0);
        for (int divisor = 1; divisor <= limit; divisor++)
            for (int multiple = divisor; multiple <= limit; multiple += divisor)
                multiple_count[divisor] += frequency[multiple];
    }

    // Ý nghĩa: tìm d lớn nhất chia hết ít nhất required phần tử.
    // Tham số: required = số phần tử tối thiểu, phải dương.
    // Trả về: d lớn nhất thỏa điều kiện, hoặc 0 nếu không có.
    int largestDivisorWithAtLeast(long long required) const {
        if (required <= 0) throw invalid_argument("DivisorSieve::largestDivisorWithAtLeast: required must be positive");
        for (int divisor = limit; divisor >= 1; divisor--)
            if (multiple_count[divisor] >= required) return divisor;
        return 0;
    }

    // Ý nghĩa: đếm số subset không rỗng có gcd đúng bằng từng d bằng đảo tổng trên các bội.
    // Tham số: mod = modulo dương.
    // Trả về: exact[d] cho d=1..limit (exact[0]=0), mỗi giá trị trong [0,mod).
    vector<long long> exactGcdSubsetCounts(long long mod = 1000000007LL) const {
        if (mod <= 0) throw invalid_argument("DivisorSieve::exactGcdSubsetCounts: mod must be positive");
        vector<long long> power_two((size_t)element_count + 1, 0);
        power_two[0] = 1 % mod;
        for (long long i = 1; i <= element_count; i++)
            power_two[(size_t)i] = (long long)((__int128)2 * power_two[(size_t)i - 1] % mod);

        vector<long long> exact(limit + 1, 0);
        for (int divisor = limit; divisor >= 1; divisor--) {
            long long all_nonempty = power_two[(size_t)multiple_count[divisor]];
            exact[divisor] = all_nonempty == 0 ? mod - 1 : all_nonempty - 1;
            for (int multiple = divisor + divisor; multiple <= limit; multiple += divisor) {
                exact[divisor] -= exact[multiple];
                if (exact[divisor] < 0) exact[divisor] += mod;
            }
        }
        return exact;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ divisor-sieve.hpp -o demo && ./demo
int main() {
    DivisorSieve sieve({2, 4, 6});
    auto exact = sieve.exactGcdSubsetCounts();
    printf("gcd cặp lớn nhất=%d; subset gcd=2: %lld; gcd=4: %lld\n",
           sieve.largestDivisorWithAtLeast(2), exact[2], exact[4]);  // 2; 5; 1
    return 0;
}
#endif
