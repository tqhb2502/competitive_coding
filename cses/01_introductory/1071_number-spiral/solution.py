# Number Spiral - CSES 1071
# https://cses.fi/problemset/task/1071

import sys


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    idx = 1
    for _ in range(t):
        y = int(data[idx]); x = int(data[idx + 1]); idx += 2
        m = y if y > x else x  # max(y, x)
        if m % 2 == 0:
            if x == m:
                ans = (m - 1) * (m - 1) + y
            else:  # y == m
                ans = m * m - x + 1
        else:
            if y == m:
                ans = (m - 1) * (m - 1) + x
            else:  # x == m
                ans = m * m - y + 1
        out.append(ans)
    sys.stdout.write("\n".join(map(str, out)) + "\n")


if __name__ == "__main__":
    main()
