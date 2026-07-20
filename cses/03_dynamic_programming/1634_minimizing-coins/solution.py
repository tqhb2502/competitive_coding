# Minimizing Coins - CSES 1634
# https://cses.fi/problemset/task/1634
#
# Unbounded knapsack: dp[s] = số đồng xu ít nhất để tạo ra tổng s.
# dp[s] = min(dp[s - c] + 1) với mọi đồng xu c.
# Đường nhanh dùng numpy; nếu không có numpy thì rơi về bản thuần Python.

import sys

INF = 1 << 30  # giá trị "không thể tạo được"; đáp án thật luôn <= x

# Ngưỡng chọn thuật toán cho mỗi đồng xu (rows = số hàng khi reshape = ceil((x+1)/c)):
#   - rows <= ngưỡng (đồng xu lớn): dùng "trượt-min nhân đôi" (binary lifting),
#     chỉ tốn ceil(log2(rows)) lượt trên mảng liền mạch nên rất nhanh.
#   - rows >  ngưỡng (đồng xu nhỏ): dùng prefix-min theo lớp đồng dư (accumulate),
#     hiệu quả khi mảng "cao" (nhiều hàng, ít cột).
DOUBLING_MAX_ROWS = 10


def solve_numpy(n, x, coins, np):
    # dp có (x + 1) phần tử, dp[0] = 0, các ô còn lại = INF.
    # Dùng int32: giá trị lớn nhất ~ INF + x < 2^31 nên không tràn số.
    size = x + 1
    dp = np.full(size, INF, dtype=np.int32)
    dp[0] = 0
    # Cột chỉ số hàng, tính trước một lần rồi cắt lát cho từng đồng xu.
    base = np.arange(size, dtype=np.int32).reshape(size, 1)
    for c in coins:
        if c > x:
            continue
        rows = (size + c - 1) // c
        if rows <= DOUBLING_MAX_ROWS:
            # Đồng xu lớn: trượt-min nhân đôi bước.
            # Mỗi lượt "dp[i] = min(dp[i], dp[i - step] + add)" mở rộng số lần
            # dùng đồng xu c lên gấp đôi (add = step / c). Sau ceil(log2(rows))
            # lượt là bao phủ đủ mọi k. "dp[:-step] + add" tạo mảng tạm riêng
            # nên phép min in-place không bị chồng lấn dữ liệu.
            step = c
            add = 1
            while step < size:
                np.minimum(dp[step:], dp[:-step] + add, out=dp[step:])
                step <<= 1
                add <<= 1
        else:
            # Đồng xu nhỏ: gom theo lớp đồng dư mod c.
            # Reshape theo bề rộng c thì các vị trí cùng số dư nằm chung một cột;
            # đặt g = dp - (chỉ số hàng), lấy prefix-min dọc cột (accumulate),
            # rồi cộng lại chỉ số hàng để ra dp mới của đồng xu này.
            L = rows * c
            if L != size:
                buf = np.empty(L, dtype=np.int32)
                buf[:size] = dp
                buf[size:] = INF
            else:
                buf = dp
            g = buf.reshape(rows, c)
            row_idx = base[:rows]
            # Làm tất cả in-place để giảm cấp phát bộ nhớ.
            g -= row_idx
            np.minimum.accumulate(g, axis=0, out=g)
            g += row_idx
            dp = buf.reshape(-1)[:size]
    ans = int(dp[x])
    return ans if ans <= x else -1


def solve_pure(n, x, coins):
    dp = [INF] * (x + 1)
    dp[0] = 0
    for c in coins:
        if c > x:
            continue
        # Vòng lặp xuôi cho phép dùng lại đồng xu (unbounded).
        for i in range(c, x + 1):
            v = dp[i - c] + 1
            if v < dp[i]:
                dp[i] = v
    ans = dp[x]
    return ans if ans <= x else -1


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    idx = 0
    n = int(data[idx]); idx += 1
    x = int(data[idx]); idx += 1
    coins = [int(v) for v in data[idx:idx + n]]
    # Loại trùng và bỏ đồng xu > x để giảm khối lượng tính toán.
    coins = sorted(set(c for c in coins if c <= x))

    if x == 0:
        sys.stdout.write("0\n")
        return

    try:
        import numpy as np
        ans = solve_numpy(n, x, coins, np)
    except ImportError:
        ans = solve_pure(n, x, coins)

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
