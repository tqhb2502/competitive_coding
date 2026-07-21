# Counting Grids - https://cses.fi/problemset/task/2210
# Bổ đề Burnside trên nhóm xoay C4 của hình vuông (0, 90, 180, 270 độ).
# Số lưới phân biệt = (Fix(id) + 2*Fix(90) + Fix(180)) / 4  (mod 1e9+7).

import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    MOD = 10 ** 9 + 7
    cells = n * n  # tổng số ô, có thể tối đa ~10^18 (Python big int)

    # Số mũ (số orbit) cho từng phép xoay.
    exp_id = cells                       # xoay 0 độ: n^2 orbit
    if n % 2 == 0:
        exp_90 = cells // 4              # orbit kích thước 4
        exp_180 = cells // 2             # orbit kích thước 2
    else:
        exp_90 = (cells + 3) // 4        # +1 orbit trung tâm
        exp_180 = (cells + 1) // 2       # +1 orbit trung tâm

    fix_id = pow(2, exp_id, MOD)
    fix_90 = pow(2, exp_90, MOD)         # bằng Fix(270) theo đối xứng
    fix_180 = pow(2, exp_180, MOD)

    total = (fix_id + 2 * fix_90 + fix_180) % MOD
    inv4 = pow(4, MOD - 2, MOD)          # nghịch đảo modular của 4 (Fermat)
    ans = total * inv4 % MOD

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
