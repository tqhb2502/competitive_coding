# Polygon Area - CSES task 2191
# https://cses.fi/problemset/task/2191
# Shoelace formula with exact integer arithmetic; print 2a = |sum(x_i*y_{i+1} - x_{i+1}*y_i)|.

import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    # Read coordinates as integers.
    xs = [0] * n
    ys = [0] * n
    idx = 1
    for i in range(n):
        xs[i] = int(data[idx])
        ys[i] = int(data[idx + 1])
        idx += 2

    # Shoelace: sum of cross products (x_i * y_{i+1} - x_{i+1} * y_i), indices mod n.
    total = 0
    for i in range(n):
        j = i + 1 if i + 1 < n else 0
        total += xs[i] * ys[j] - xs[j] * ys[i]

    # Print doubled area (always a non-negative integer).
    sys.stdout.write(str(abs(total)))


if __name__ == "__main__":
    main()
