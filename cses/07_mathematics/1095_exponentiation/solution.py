# Exponentiation - https://cses.fi/problemset/task/1095
# Tính a^b mod (10^9 + 7) cho mỗi truy vấn bằng built-in pow (binary exponentiation).
# Quy ước đề bài: 0^0 = 1 (pow(0, 0, MOD) trả về 1).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    MOD = 1_000_000_007
    n = int(data[0])
    out = []
    idx = 1
    for _ in range(n):
        a = int(data[idx])
        b = int(data[idx + 1])
        idx += 2
        out.append(str(pow(a, b, MOD)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
