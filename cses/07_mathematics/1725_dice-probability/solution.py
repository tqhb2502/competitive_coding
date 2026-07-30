import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    a = int(data[1])
    b = int(data[2])

    # dp[s] = xác suất để tổng hiện tại bằng s.
    # Khởi tạo: chưa tung con nào nên tổng chắc chắn bằng 0.
    dp = [0.0] * (6 * n + 1)
    dp[0] = 1.0
    p = 1.0 / 6.0
    cur_max = 0

    # Thêm lần lượt từng con xúc xắc: mỗi mặt 1..6 xuất hiện với xác suất 1/6,
    # tương đương tích chập phân phối cũ với phân phối đều {1..6}.
    for _ in range(n):
        ndp = [0.0] * (6 * n + 1)
        for s in range(cur_max + 1):
            v = dp[s]
            if v == 0.0:
                continue
            v *= p
            ndp[s + 1] += v
            ndp[s + 2] += v
            ndp[s + 3] += v
            ndp[s + 4] += v
            ndp[s + 5] += v
            ndp[s + 6] += v
        cur_max += 6
        dp = ndp

    # Đáp số: tổng xác suất trên đoạn [a, b], in với 6 chữ số thập phân.
    ans = sum(dp[a:b + 1])
    sys.stdout.write("%.6f\n" % ans)


main()
