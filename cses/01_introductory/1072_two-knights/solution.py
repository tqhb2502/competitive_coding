# Two Knights - CSES 1072
# https://cses.fi/problemset/task/1072
#
# For each board size k (1..n), count placements of two knights that do not
# attack each other:
#   answer(k) = C(k*k, 2) - 4*(k-1)*(k-2)
#             = k*k*(k*k - 1)//2 - 4*(k-1)*(k-2)

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    out = []
    for k in range(1, n + 1):
        total = k * k * (k * k - 1) // 2
        attacking = 4 * (k - 1) * (k - 2)
        out.append(str(total - attacking))

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
