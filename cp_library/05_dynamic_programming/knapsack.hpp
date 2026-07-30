#pragma once
#include <bits/stdc++.h>
using namespace std;
// Knapsack — 0/1, subset-sum và bounded bằng monotonic queue theo lớp đồng dư.
// Khi dùng: tối ưu dưới sức chứa; mỗi vật dùng một lần hoặc mỗi loại có giới hạn count[i].
// ĐPT: 0/1 và bounded đều O(nW); bộ nhớ O(W). Bounded không phụ thuộc độ lớn count.
// Dùng: knapsack01(w,val,W); subsetSum(a,S); boundedKnapsack(w,val,count,W);
// Bẫy: weight phải >0, capacity >=0; giá trị/tích count*value và đáp án phải vừa long long.
// CSES: 1093 1158 1159 1665 1706 1745 2176 3159 3302
// Ý nghĩa: giá trị lớn nhất chọn tập con các vật (mỗi vật 1 lần) sao cho tổng trọng lượng <= W.
// Tham số: w = trọng lượng từng vật; val = giá trị từng vật; W = sức chứa.
// Trả về: tổng giá trị lớn nhất đạt được.
long long knapsack01(const vector<int>& w, const vector<long long>& val, int W) {
    vector<long long> dp(W + 1, 0);
    for (int i = 0; i < (int)w.size(); i++)
        for (int c = W; c >= w[i]; c--) dp[c] = max(dp[c], dp[c - w[i]] + val[i]);
    return dp[W];
}
// Ý nghĩa: kiểm tra có chọn được tập con của a với tổng đúng bằng S hay không (subset-sum 0/1).
// Tham số: a = mảng số không âm; S = tổng cần đạt.
// Trả về: true nếu tồn tại tập con có tổng = S.
bool subsetSum(const vector<int>& a, int S) {
    vector<char> dp(S + 1, 0);
    dp[0] = 1;
    for (int x : a)
        for (int c = S; c >= x; c--) if (dp[c - x]) dp[c] = 1;
    return dp[S];
}

// Ý nghĩa: bounded knapsack O(nW) bằng max cửa sổ trên từng lớp dung lượng modulo weight.
// Tham số: weight/value/limit của mỗi loại; capacity = tổng trọng lượng tối đa.
// Trả về: tổng value lớn nhất với mỗi loại i dùng từ 0 tới limit[i] lần.
long long boundedKnapsack(const vector<int>& weight, const vector<long long>& value,
                          const vector<int>& limit, int capacity) {
    assert(capacity >= 0 && weight.size() == value.size() && value.size() == limit.size());
    vector<long long> dp(capacity + 1, 0), next(capacity + 1);
    for (int item = 0; item < (int)weight.size(); ++item) {
        int w = weight[item], copies = limit[item];
        long long gain = value[item];
        assert(w > 0 && copies >= 0);
        next = dp;
        for (int residue = 0; residue < w && residue <= capacity; ++residue) {
            deque<pair<int, __int128>> candidates;  // (q, dp[r+q*w] - q*gain)
            int q = 0;
            for (int current = residue; current <= capacity; current += w, ++q) {
                while (!candidates.empty() && candidates.front().first < q - copies)
                    candidates.pop_front();
                __int128 score = (__int128)dp[current] - (__int128)q * gain;
                while (!candidates.empty() && candidates.back().second <= score)
                    candidates.pop_back();
                candidates.push_back({q, score});
                __int128 best = (__int128)q * gain + candidates.front().second;
                assert(best >= LLONG_MIN && best <= LLONG_MAX);
                next[current] = (long long)best;
            }
        }
        dp.swap(next);
    }
    return dp[capacity];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ knapsack.hpp -o demo && ./demo
int main() {
    vector<int> w = {2, 3, 4, 5};
    vector<long long> val = {3, 4, 5, 6};
    printf("knapsack01(W=5) = %lld ; subsetSum({2,3,4,5}, 7) = %d\n",
           knapsack01(w, val, 5), (int)subsetSum(w, 7));   // 3+4=7 tại W=5 -> 7 ; 2+5=7 -> 1
    printf("bounded = %lld\n",
           boundedKnapsack({2, 3}, {3, 5}, {3, 1}, 7));    // 2+2+3 -> 11
    return 0;
}
#endif
