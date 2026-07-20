# Hamiltonian Flights - https://cses.fi/problemset/task/1690
# Đếm số đường đi Hamilton từ thành phố 1 đến thành phố n (đi qua mỗi thành phố đúng một lần).
# Bitmask DP: dp[mask][j] = số cách xuất phát từ 0, thăm đúng tập thành phố `mask`, kết thúc tại j.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    MOD = 10 ** 9 + 7

    # inadj[j] = bitmask các đỉnh i có cạnh i -> j (danh sách đỉnh vào của j)
    inadj = [0] * n
    for _ in range(m):
        a = int(data[idx]) - 1; idx += 1
        b = int(data[idx]) - 1; idx += 1
        inadj[b] |= (1 << a)

    full = (1 << n) - 1

    # dp được lưu theo từng mask. Mỗi mask "hợp lệ" luôn chứa bit 0 (đỉnh xuất phát).
    dp = [None] * (1 << n)
    base = [0] * n
    base[0] = 1                 # chỉ thăm đỉnh 0, đang ở đỉnh 0
    dp[1] = base

    # Duyệt mask tăng dần: prev = mask ^ jbit < mask nên luôn được tính trước.
    # Chỉ xử lý các mask lẻ (chứa bit 0). Chỉ kết thúc tại j != 0.
    for mask in range(3, full + 1):
        if not (mask & 1):
            continue
        row = [0] * n
        tmp = mask & ~1          # các đỉnh khác đỉnh 0 có trong mask -> ứng viên làm điểm kết thúc j
        while tmp:
            jbit = tmp & (-tmp)
            tmp ^= jbit
            j = jbit.bit_length() - 1
            prev_mask = mask ^ jbit
            cand = prev_mask & inadj[j]   # các đỉnh i trước j, i nằm trong prev_mask
            if cand:
                prev = dp[prev_mask]
                s = 0
                c = cand
                while c:
                    ibit = c & (-c)
                    c ^= ibit
                    s += prev[ibit.bit_length() - 1]
                row[j] = s % MOD
        dp[mask] = row

    ans = 0
    if n == 1:
        ans = 1
    else:
        fr = dp[full]
        if fr is not None:
            ans = fr[n - 1] % MOD
    sys.stdout.write(str(ans) + "\n")


main()
