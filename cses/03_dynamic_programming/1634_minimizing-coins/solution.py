import sys


def main():
    # Đọc toàn bộ dữ liệu một lần cho nhanh (vào/ra dạng buffer).
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])
    coins = data[2:2 + n]

    # INF = giá trị "không tạo được". Vì mỗi đồng xu >= 1 nên đáp án thật luôn
    # <= x, do đó dùng x + 1 làm INF vừa an toàn vừa giữ số nhỏ.
    INF = x + 1

    # Loại đồng xu trùng nhau và bỏ đồng xu có giá trị > x (không dùng được).
    coin_vals = sorted({int(c) for c in coins if int(c) <= x})

    # dp[s] = số đồng xu ít nhất để tạo tổng s. Cơ sở: dp[0] = 0.
    dp = [INF] * (x + 1)
    dp[0] = 0

    # Unbounded knapsack: quét i tăng dần (vòng lặp xuôi) cho phép dùng lại một
    # đồng xu tùy ý. Chuyển: dp[i] = min(dp[i], dp[i - c] + 1).
    for c in coin_vals:
        d = dp  # Gán biến cục bộ để CPython truy cập nhanh hơn trong vòng lặp nóng.
        for i in range(c, x + 1):
            v = d[i - c] + 1
            if v < d[i]:
                d[i] = v

    # Nếu dp[x] vẫn > x (còn là INF) thì không tạo được, in -1.
    ans = dp[x]
    sys.stdout.write(str(ans if ans <= x else -1) + "\n")


if __name__ == "__main__":
    main()
