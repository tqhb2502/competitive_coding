# Nested Ranges Check - https://cses.fi/problemset/task/2168
# Với mỗi đoạn, kiểm tra nó có chứa đoạn khác và có bị đoạn khác chứa hay không.
# Sort theo x rồi quét một lượt, giữ max_y (cho "bị chứa") và min_y (cho "chứa").

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    xs = [0] * n
    ys = [0] * n
    pos = 1
    for i in range(n):
        xs[i] = int(data[pos])
        ys[i] = int(data[pos + 1])
        pos += 2

    contains = bytearray(n)   # đoạn i có chứa đoạn khác?
    contained = bytearray(n)  # đoạn i có bị chứa bởi đoạn khác?

    # (A) "bị chứa": sort x tăng, y giảm -> giữ max_y của các đoạn đã xét.
    a = sorted((xs[i], -ys[i], i) for i in range(n))
    max_y = -1
    for _x, ny, i in a:
        y = -ny
        if y <= max_y:
            contained[i] = 1
        else:
            max_y = y

    # (B) "chứa đoạn khác": sort x giảm, y tăng -> giữ min_y của các đoạn đã xét.
    b = sorted((-xs[i], ys[i], i) for i in range(n))
    min_y = 1 << 62
    for _nx, y, i in b:
        if y >= min_y:
            contains[i] = 1
        else:
            min_y = y

    out1 = ' '.join('1' if v else '0' for v in contains)
    out2 = ' '.join('1' if v else '0' for v in contained)
    sys.stdout.write(out1 + '\n' + out2 + '\n')


main()
