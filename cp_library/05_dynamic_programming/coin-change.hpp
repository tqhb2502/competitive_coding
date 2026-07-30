#pragma once
#include <bits/stdc++.h>
using namespace std;
// Coin Change (đồng xu không giới hạn) — số đồng ít nhất và số CÁCH tạo ra target, O(target * số_loại_xu).
// Khi dùng: mỗi loại xu dùng bao nhiêu lần cũng được; hỏi min số xu, hoặc đếm cách (có/không phân biệt thứ tự).
// ĐPT: O(target * m); bộ nhớ O(target). coinMin trả -1 nếu không tạo được; đếm cách lấy theo MOD.
// Dùng: coinMin(coins, t); coinCountOrdered(coins, t); coinCountUnordered(coins, t);
// Bẫy: THỨ TỰ vòng lặp: ordered = target NGOÀI, xu TRONG; unordered = xu NGOÀI, target TRONG; dùng long long + MOD.
// CSES: 1634 1635 1636
// Ý nghĩa: số đồng xu ít nhất để tạo đúng target (mỗi loại dùng không giới hạn).
// Tham số: coins = mệnh giá các loại xu; target = số tiền cần tạo.
// Trả về: số xu ít nhất, hoặc -1 nếu không tạo được.
long long coinMin(const vector<int>& coins, int target) {
    const long long INF = (long long)4e18;
    vector<long long> dp(target + 1, INF);
    dp[0] = 0;
    for (int c = 1; c <= target; c++)
        for (int coin : coins) if (coin <= c && dp[c - coin] + 1 < dp[c]) dp[c] = dp[c - coin] + 1;
    return dp[target] >= INF ? -1 : dp[target];
}
// Ý nghĩa: số cách tạo target khi CÓ phân biệt thứ tự các đồng xu (dãy có thứ tự).
// Tham số: coins = mệnh giá; target = số tiền; MOD = số dư (mặc định 1e9+7).
// Trả về: số cách theo modulo MOD.
long long coinCountOrdered(const vector<int>& coins, int target, long long MOD = 1000000007) {
    vector<long long> dp(target + 1, 0);
    dp[0] = 1;
    for (int c = 1; c <= target; c++)
        for (int coin : coins) if (coin <= c) dp[c] = (dp[c] + dp[c - coin]) % MOD;
    return dp[target];
}
// Ý nghĩa: số cách tạo target khi KHÔNG phân biệt thứ tự (tập bội của các mệnh giá).
// Tham số: coins = mệnh giá; target = số tiền; MOD = số dư (mặc định 1e9+7).
// Trả về: số cách theo modulo MOD.
long long coinCountUnordered(const vector<int>& coins, int target, long long MOD = 1000000007) {
    vector<long long> dp(target + 1, 0);
    dp[0] = 1;
    for (int coin : coins)
        for (int c = coin; c <= target; c++) dp[c] = (dp[c] + dp[c - coin]) % MOD;
    return dp[target];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ coin-change.hpp -o demo && ./demo
int main() {
    vector<int> coins = {1, 3, 4};
    printf("min xu tạo 6 = %lld ; cách có thứ tự = %lld ; cách không thứ tự = %lld\n",
           coinMin(coins, 6), coinCountOrdered(coins, 6), coinCountUnordered(coins, 6));   // 2 (3+3) ; 9 ; 4
    return 0;
}
#endif
