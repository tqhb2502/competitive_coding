# Forest Queries II - CSES 1739
# https://cses.fi/problemset/task/1739
#
# Grid n x n, each cell is a tree ('*') or empty ('.').
#   - Query type 1 "1 y x": toggle cell (y, x).
#   - Query type 2 "2 y1 x1 y2 x2": count trees inside a rectangle.
# Structure: 2D Fenwick tree (BIT) for point-update + rectangle prefix-sum.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    q = int(data[1])
    W = n + 1
    size = W * W

    bit = [0] * size          # 2D Fenwick, flat index i*W + j (1-indexed)
    cur = bytearray(size)     # current state of each cell (0/1) to know toggle direction
    trans = bytes.maketrans(b'.*', bytes((0, 1)))

    # Load the initial grid (slice assignment runs at C speed).
    for i in range(1, n + 1):
        base = i * W
        seg = data[1 + i].translate(trans)   # data[2] is row 1 -> data[1 + i] is row i
        bit[base + 1: base + 1 + n] = seg
        cur[base + 1: base + 1 + n] = seg

    # Build the Fenwick tree in place in O(n^2): propagate along rows, then columns.
    for i in range(1, n + 1):
        base = i * W
        for j in range(1, n + 1):
            k = j + (j & -j)
            if k <= n:
                bit[base + k] += bit[base + j]
    for j in range(1, n + 1):
        for i in range(1, n + 1):
            k = i + (i & -i)
            if k <= n:
                bit[k * W + j] += bit[i * W + j]

    b = bit
    data_local = data
    L = len(data_local)
    idx = 2 + n
    out = []
    ap = out.append

    while idx < L:
        t = data_local[idx]
        if t == b'1':
            y = int(data_local[idx + 1])
            x = int(data_local[idx + 2])
            idx += 3
            p = y * W + x
            if cur[p]:
                cur[p] = 0
                delta = -1
            else:
                cur[p] = 1
                delta = 1
            # point update on the 2D BIT: O(log^2 n)
            ii = y
            while ii <= n:
                base = ii * W
                jj = x
                while jj <= n:
                    b[base + jj] += delta
                    jj += jj & -jj
                ii += ii & -ii
        else:
            y1 = int(data_local[idx + 1])
            x1 = int(data_local[idx + 2])
            y2 = int(data_local[idx + 3])
            x2 = int(data_local[idx + 4])
            idx += 5
            # Decompose the columns once, reuse for every row in the y-decomposition.
            c2 = []
            jj = x2
            while jj > 0:
                c2.append(jj)
                jj -= jj & -jj
            c1 = []
            jj = x1 - 1
            while jj > 0:
                c1.append(jj)
                jj -= jj & -jj

            res = 0
            ii = y2
            while ii > 0:
                base = ii * W
                s = 0
                for j in c2:
                    s += b[base + j]
                for j in c1:
                    s -= b[base + j]
                res += s
                ii -= ii & -ii
            ii = y1 - 1
            while ii > 0:
                base = ii * W
                s = 0
                for j in c2:
                    s += b[base + j]
                for j in c1:
                    s -= b[base + j]
                res -= s
                ii -= ii & -ii
            ap(res)

    if out:
        sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
