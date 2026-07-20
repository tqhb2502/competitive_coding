# Minimizing Coins - CSES 1634
# https://cses.fi/problemset/task/1634
#
# Unbounded knapsack: dp[s] = số đồng xu ít nhất để tạo ra tổng s.
# dp[s] = min(dp[s - c] + 1) với mọi đồng xu c <= s.
# Giải thuần Python (chỉ dùng thư viện chuẩn), tối ưu cho CPython.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])
    coins = data[2:2 + n]

    # INF = giá trị "không tạo được". Đáp án thật luôn <= x (mỗi đồng xu >= 1),
    # nên dùng x + 1 làm INF là an toàn và giữ số nhỏ.
    INF = x + 1

    # Loại trùng và bỏ đồng xu > x (không thể dùng). Sắp xếp tăng dần chỉ để gọn.
    coin_vals = sorted({int(c) for c in coins if int(c) <= x})

    dp = [INF] * (x + 1)
    dp[0] = 0

    # Vòng lặp xuôi (i tăng dần) cho phép dùng lại một đồng xu tùy ý (unbounded).
    for c in coin_vals:
        # Bind local để tăng tốc trong CPython.
        d = dp
        # dp[i] = min(dp[i], dp[i - c] + 1) với i từ c tới x.
        for i in range(c, x + 1):
            v = d[i - c] + 1
            if v < d[i]:
                d[i] = v

    ans = dp[x]
    sys.stdout.write(str(ans if ans <= x else -1) + "\n")


if __name__ == "__main__":
    main()
