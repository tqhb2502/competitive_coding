import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 1000000007

    # total = tổng 1..n; nếu lẻ thì không thể chia đều -> đáp án 0
    total = n * (n + 1) // 2
    if total & 1:
        sys.stdout.write("0\n")
        return
    # Mỗi tập phải có tổng target = total/2
    target = total // 2

    # Chỉ đếm subset của {1, ..., n-1} (bỏ qua n) để mỗi cách chia được đếm đúng 1 lần
    m = n - 1
    s_full = m * (m + 1) // 2          # tổng của tất cả các item 1..m
    gap = s_full - target              # tổng tối đa có thể "bỏ đi" khỏi target

    # dp[j] = số subset (mỗi số dùng tối đa 1 lần) có tổng đúng bằng j, lấy mod
    dp = [0] * (target + 1)
    dp[0] = 1                          # base case: tập rỗng cho tổng 0

    s_k = 0                            # tổng các item đã xét 1..k
    for k in range(1, m + 1):
        s_k += k
        # Cửa sổ [lo, hi] các chỉ số j thực sự cần cập nhật (cắt tỉa hai đầu):
        #   hi = min(target, s_k)      -> tổng lớn nhất đạt được với item 1..k
        #   lo = max(k, s_k - gap)     -> dưới ngưỡng này không thể chạm target
        hi = target if s_k > target else s_k
        lo = s_k - gap
        if lo < k:
            lo = k
        # Duyệt j giảm dần để mỗi item chỉ dùng một lần trong một subset (0/1 knapsack)
        for j in range(hi, lo - 1, -1):
            v = dp[j] + dp[j - k]
            # Cộng rồi trừ MOD một lần, nhanh hơn toán tử % trong CPython
            if v >= MOD:
                v -= MOD
            dp[j] = v

    sys.stdout.write(str(dp[target]) + "\n")


main()
