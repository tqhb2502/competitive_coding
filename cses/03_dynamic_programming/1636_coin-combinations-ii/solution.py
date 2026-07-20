# Coin Combinations II - https://cses.fi/problemset/task/1636
# Đếm số cách (KHÔNG kể thứ tự) tạo ra tổng x từ các đồng xu, kết quả mod 1e9+7.
# DP dạng unbounded knapsack đếm tổ hợp: đồng xu ở vòng NGOÀI, tổng ở vòng TRONG
# tăng dần, nên mỗi multiset đồng xu chỉ được đếm đúng một lần.
import sys

MOD = 1000000007


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])

    ways = [0] * (x + 1)
    ways[0] = 1

    # Bind hot names to locals for CPython speed.
    w = ways
    mod = MOD

    for i in range(2, 2 + n):
        c = int(data[i])
        if c > x:
            continue
        # ways[s] += ways[s - c] với s tăng dần (dùng c không giới hạn số lần).
        for s in range(c, x + 1):
            v = w[s] + w[s - c]
            if v >= mod:
                v -= mod
            w[s] = v

    sys.stdout.write(str(w[x] % mod) + "\n")


main()
