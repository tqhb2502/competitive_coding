import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    MOD = 1000000007

    # dp[v] = số cách điền tiền tố sao cho phần tử cuối mang giá trị v.
    # Độ dài m+2, chỉ số 0 và m+1 là ô đệm (padding) luôn = 0 để tránh kiểm tra biên.
    dp = [0] * (m + 2)

    # Cơ sở (i = 0): x[0] == 0 thì mọi v đều được phép, ngược lại chỉ dùng x[0].
    x0 = int(data[2])
    if x0 == 0:
        for v in range(1, m + 1):
            dp[v] = 1
    else:
        dp[x0] = 1

    # cnt: đếm số bước đã hoãn mod (defer modulo) liên tiếp.
    cnt = 0
    for i in range(1, n):
        xi = int(data[2 + i])
        if xi == 0:
            # Vị trí chưa biết: cộng ba lát cắt (slice) của list cùng lúc bằng zip.
            # core[k] = dp[k] + dp[k+1] + dp[k+2] => dp_new[v] = dp[v-1] + dp[v] + dp[v+1].
            core = [a + b + c for a, b, c in zip(dp, dp[1:], dp[2:])]
            cnt += 1
            if cnt == 16:
                # Sau 16 bước, giá trị < 3^16 * 1e9 ~ 4.3e16; Python int chính xác tuyệt đối.
                core = [v % MOD for v in core]
                cnt = 0
            # Bao lại hai ô đệm quanh core để giữ vững padding.
            dp = [0]
            dp += core
            dp.append(0)
        else:
            # Vị trí đã biết: chỉ tính đúng ô xi, lấy modulo ngay và reset bộ đếm.
            val = (dp[xi - 1] + dp[xi] + dp[xi + 1]) % MOD
            dp = [0] * (m + 2)
            dp[xi] = val
            cnt = 0

    # Đáp án: tổng dp[v] (lấy modulo).
    sys.stdout.write(str(sum(dp) % MOD) + "\n")


main()
