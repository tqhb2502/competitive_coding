import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    MOD = 10 ** 9 + 7

    # inadj[j] = bitmask các đỉnh i có cạnh i -> j (danh sách các đỉnh vào của j).
    inadj = [0] * n
    for _ in range(m):
        a = int(data[idx]) - 1; idx += 1
        b = int(data[idx]) - 1; idx += 1
        inadj[b] |= (1 << a)

    full = (1 << n) - 1

    # dp[mask] = danh sách dp[mask][j]: số cách xuất phát từ đỉnh 0, đã thăm đúng
    # tập đỉnh `mask` và đang đứng ở đỉnh j. Mỗi mask "hợp lệ" luôn chứa bit 0.
    dp = [None] * (1 << n)
    base = [0] * n
    base[0] = 1                 # cơ sở: chỉ thăm đỉnh 0, đang đứng ở đỉnh 0
    dp[1] = base

    # Duyệt mask tăng dần: prev_mask = mask ^ jbit < mask nên luôn được tính trước.
    # Chỉ xử lý các mask lẻ (chứa bit 0) và chỉ cho phép kết thúc tại j != 0.
    for mask in range(3, full + 1):
        if not (mask & 1):
            continue
        row = [0] * n
        tmp = mask & ~1          # các đỉnh khác đỉnh 0 trong mask -> ứng viên điểm kết thúc j
        while tmp:
            jbit = tmp & (-tmp)
            tmp ^= jbit
            j = jbit.bit_length() - 1
            prev_mask = mask ^ jbit
            cand = prev_mask & inadj[j]   # các đỉnh liền trước i trong prev_mask có cạnh i -> j
            if cand:
                # Chuyển trạng thái kiểu "pull": cộng dp[prev_mask][i] theo mọi i hợp lệ.
                prev = dp[prev_mask]
                s = 0
                c = cand
                while c:
                    ibit = c & (-c)
                    c ^= ibit
                    s += prev[ibit.bit_length() - 1]
                row[j] = s % MOD
        dp[mask] = row

    # Đáp án: đã thăm hết mọi đỉnh và kết thúc tại đỉnh n-1.
    ans = 0
    if n == 1:
        ans = 1
    else:
        fr = dp[full]
        if fr is not None:
            ans = fr[n - 1] % MOD
    sys.stdout.write(str(ans) + "\n")


main()
