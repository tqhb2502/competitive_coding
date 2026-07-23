import sys


def main():
    # Đọc toàn bộ dữ liệu một lần để tăng tốc I/O.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])
    coins = [int(v) for v in data[2:2 + n]]
    MOD = 1000000007

    # dp[s] = số cách CÓ THỨ TỰ tạo ra tổng đúng bằng s; cơ sở dp[0] = 1.
    dp = [0] * (x + 1)
    dp[0] = 1

    # Sắp xếp tăng dần để vòng trong break sớm khi c > s.
    coins.sort()
    cmax = coins[-1]

    # Vùng biên 1 <= s < cmax: chưa chắc mọi đồng xu đều dùng được nên phải kiểm
    # tra c <= s; coins đã sắp xếp tăng dần nên gặp c > s là break sớm.
    bhi = cmax if cmax <= x else x + 1
    for s in range(1, bhi):
        t = 0
        for c in coins:
            if c > s:
                break
            t += dp[s - c]
        dp[s] = t % MOD

    # Vùng nóng cmax <= s <= x: mọi đồng xu đều hợp lệ (s - c >= 0). Sinh sẵn một
    # hàm cộng "duỗi cuộn" (unrolled) bằng exec, nhúng thẳng các mệnh giá vào biểu
    # thức để bỏ vòng lặp bên trong -> CPython chạy nhanh hơn đáng kể.
    if x >= cmax:
        terms = " + ".join("d[s-%d]" % c for c in coins)
        src = (
            "def hot(d, lo, hi, MOD):\n"
            " for s in range(lo, hi):\n"
            "  d[s] = (" + terms + ") % MOD\n"
        )
        namespace = {}
        exec(src, namespace)
        namespace["hot"](dp, cmax, x + 1, MOD)

    # Đáp án: dp[x] mod MOD.
    sys.stdout.write(str(dp[x] % MOD) + "\n")


main()
