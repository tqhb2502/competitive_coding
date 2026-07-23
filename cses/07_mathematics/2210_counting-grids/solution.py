import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    MOD = 10 ** 9 + 7
    cells = n * n  # tổng số ô = n^2, có thể tối đa ~10^18 (Python big int)

    # Số orbit của mỗi phép xoay (bổ đề Burnside trên nhóm xoay C4).
    exp_id = cells                       # xoay 0 độ: mỗi ô một orbit => n^2 orbit
    if n % 2 == 0:
        exp_90 = cells // 4              # orbit kích thước 4
        exp_180 = cells // 2             # orbit kích thước 2
    else:
        exp_90 = (cells + 3) // 4        # thêm 1 orbit cho ô trung tâm
        exp_180 = (cells + 1) // 2       # thêm 1 orbit cho ô trung tâm

    # Fix(g) = 2^(số orbit của g): số cách tô bất biến dưới phép xoay g.
    fix_id = pow(2, exp_id, MOD)
    fix_90 = pow(2, exp_90, MOD)         # bằng Fix(270) theo đối xứng
    fix_180 = pow(2, exp_180, MOD)

    # Kết quả Burnside = ( Fix(id) + 2*Fix(90) + Fix(180) ) / 4.
    total = (fix_id + 2 * fix_90 + fix_180) % MOD
    inv4 = pow(4, MOD - 2, MOD)          # nghịch đảo modular của 4 (Fermat)
    ans = total * inv4 % MOD

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
