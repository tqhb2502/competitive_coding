# Coin Piles - CSES task 1754
# https://cses.fi/problemset/task/1754

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    idx = 1
    out = []
    for _ in range(t):
        a = int(data[idx])
        b = int(data[idx + 1])
        idx += 2
        if (a + b) % 3 == 0 and 2 * a >= b and 2 * b >= a:
            out.append("YES")
        else:
            out.append("NO")
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
