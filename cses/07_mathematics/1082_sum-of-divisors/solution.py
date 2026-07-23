import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 10 ** 9 + 7

    # S = sum_{d=1..n} d * floor(n/d), duyệt theo từng khối divisor-block.
    ans = 0
    d = 1
    while d <= n:
        q = n // d              # thương floor(n/d) không đổi trên khối
        r = n // q              # chỉ số lớn nhất giữ nguyên thương q
        block_sum = (d + r) * (r - d + 1) // 2   # tổng cấp số cộng d..r bằng số nguyên tùy ý
        ans = (ans + (q % MOD) * (block_sum % MOD)) % MOD
        d = r + 1

    sys.stdout.write(str(ans % MOD))


if __name__ == "__main__":
    main()
