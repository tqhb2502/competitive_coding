# Coin Combinations II - https://cses.fi/problemset/task/1636
# Đếm số cách (không kể thứ tự) tạo ra tổng x từ các đồng xu, kết quả mod 1e9+7.
# DP dạng unbounded knapsack: xử lý từng đồng xu ở vòng ngoài để mỗi tổ hợp
# chỉ được đếm đúng một lần.
import sys

MOD = 1000000007


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])
    coins = data[2:2 + n]

    try:
        # Đường nhanh: dùng numpy để vector hóa. Với mỗi đồng xu c, phép
        # ways[s] += ways[s-c] (s tăng dần) chính là prefix sum theo từng lớp
        # thặng dư modulo c. Reshape về (m, c) rồi cumsum theo axis=0 sẽ tính
        # đồng thời tất cả các lớp -> O(x) mỗi đồng xu bằng C.
        import numpy as np

        ways = np.zeros(x + 1, dtype=np.int64)
        ways[0] = 1
        for cb in coins:
            c = int(cb)
            if c > x:
                continue
            m = (x + 1 + c - 1) // c          # số hàng sau khi reshape
            total = m * c
            pad = total - (x + 1)
            if pad:
                arr = np.concatenate(
                    [ways, np.zeros(pad, dtype=np.int64)])
            else:
                arr = ways
            ways = (arr.reshape(m, c).cumsum(axis=0) % MOD).reshape(-1)[:x + 1]
        sys.stdout.write(str(int(ways[x] % MOD)) + "\n")
        return
    except ImportError:
        pass

    # Đường dự phòng (không có numpy): DP thuần Python O(n*x).
    ways = [0] * (x + 1)
    ways[0] = 1
    for cb in coins:
        c = int(cb)
        if c > x:
            continue
        for s in range(c, x + 1):
            v = ways[s] + ways[s - c]
            if v >= MOD:
                v -= MOD
            ways[s] = v
    sys.stdout.write(str(ways[x] % MOD) + "\n")


main()
