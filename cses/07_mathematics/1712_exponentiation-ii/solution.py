# Exponentiation II - CSES 1712
# https://cses.fi/problemset/task/1712
# Tính a^(b^c) mod (10^9+7) bằng định lý Fermat nhỏ (Fermat's little theorem).
import sys


def main():
    data = sys.stdin.buffer.read().split()
    MOD = 1000000007        # prime
    PHI = MOD - 1           # 10^9+6 = phi(MOD) vì MOD là số nguyên tố
    idx = 1
    n = int(data[0])
    out = []
    for _ in range(n):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        if a == 0:
            # Mũ thật sự e = b^c. e == 0 chỉ khi b == 0 và c >= 1.
            if b == 0 and c >= 1:
                out.append("1")   # 0^0 = 1 theo định nghĩa của bài
            else:
                out.append("0")   # 0^(số dương) = 0
        else:
            # gcd(a, MOD) = 1 vì 0 < a <= 10^9 < MOD (prime),
            # nên a^e = a^(e mod PHI) (mod MOD) theo Fermat.
            e = pow(b, c, PHI)
            out.append(str(pow(a, e, MOD)))
    sys.stdout.write("\n".join(out) + "\n")


main()
