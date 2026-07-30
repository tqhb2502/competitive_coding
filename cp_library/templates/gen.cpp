// Sinh test NGẪU NHIÊN cho stress test (so lời giải chính với brute).
// Biên dịch/chạy: g++ -std=c++17 -O2 gen.cpp -o gen && ./gen 12345 > in.txt
// Dùng cùng: bash cp_library/templates/stress.sh main.cpp brute.cpp gen.cpp 500
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    unsigned long long seed = argc > 1 ? strtoull(argv[1], nullptr, 10) : 12345ULL;
    mt19937_64 rng(seed);
    auto rnd = [&](long long lo, long long hi) {
        return lo + (long long)(rng() % (unsigned long long)(hi - lo + 1));
    };
    // TODO: sửa cho đúng ĐỊNH DẠNG INPUT của đề. Giữ n nhỏ để brute chạy kịp và lỗi dễ đọc.
    int n = (int)rnd(1, 8);
    printf("%d\n", n);
    for (int i = 0; i < n; i++) printf("%lld%c", rnd(1, 10), i + 1 == n ? '\n' : ' ');
    return 0;
}
