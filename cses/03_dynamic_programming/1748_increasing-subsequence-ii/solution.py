# Increasing Subsequence II - CSES 1748
# https://cses.fi/problemset/task/1748
# Đếm số dãy con tăng ngặt (mod 1e9+7) bằng Fenwick tree + nén tọa độ.
# dp[i] = 1 + sum(dp[j]) với j < i và a[j] < a[i]; đáp án = sum(dp[i]).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    a = data[1:1 + n]

    MOD = 1000000007

    # Nén tọa độ: gán rank 1..m cho các giá trị phân biệt.
    ints = list(map(int, a))
    sorted_vals = sorted(set(ints))
    rank = {v: i + 1 for i, v in enumerate(sorted_vals)}
    m = len(sorted_vals)

    # Fenwick tree lưu tổng dp theo rank giá trị.
    tree = [0] * (m + 1)

    total = 0
    for v in ints:
        r = rank[v]
        # truy vấn prefix sum trên [1, r-1] -> tổng dp của các phần tử trước có giá trị < v
        i = r - 1
        s = 0
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        dp = (s + 1) % MOD
        total += dp
        # cập nhật: cộng dp vào vị trí rank r
        i = r
        while i <= m:
            tree[i] = (tree[i] + dp) % MOD
            i += i & (-i)

    sys.stdout.write(str(total % MOD))


main()
