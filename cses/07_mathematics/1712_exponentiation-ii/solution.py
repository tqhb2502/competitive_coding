import sys


def main():
    data = sys.stdin.buffer.read().split()
    MOD = 1000000007        # số nguyên tố p
    PHI = MOD - 1           # phi(p) = p - 1 vì p nguyên tố
    idx = 1
    n = int(data[0])
    out = []
    for _ in range(n):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        if a == 0:
            # a = 0: không dùng Fermat được vì gcd(0, p) != 1.
            # Số mũ thật e = b^c bằng 0 chỉ khi b = 0 và c >= 1 -> 0^0 = 1 theo quy ước;
            # ngược lại e > 0 -> 0^(số dương) = 0.
            if b == 0 and c >= 1:
                out.append("1")
            else:
                out.append("0")
        else:
            # a > 0 nên gcd(a, p) = 1: theo Fermat, a^e = a^(e mod PHI) (mod p).
            # Rút gọn số mũ e = b^c mod PHI bằng modular exponentiation rồi lũy thừa theo mod p.
            e = pow(b, c, PHI)
            out.append(str(pow(a, e, MOD)))
    sys.stdout.write("\n".join(out) + "\n")


main()
