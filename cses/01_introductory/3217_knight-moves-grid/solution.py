# Knight Moves Grid - CSES 3217
# https://cses.fi/problemset/task/3217
# BFS from the top-left corner (0,0). Knight moves are symmetric, so the
# minimum number of moves from any square to (0,0) equals the BFS distance
# from (0,0) to that square.
#
# Performance: the grid has up to n*n = 10^6 cells. To keep the BFS fast in
# pure Python we lay the board out as a flat 1-D array surrounded by a 2-cell
# border ("padding"). Padding cells are pre-marked as visited (-2), so the
# eight knight moves never need in-bounds checks - a move that would leave the
# board simply lands on a padding cell and is skipped. The BFS runs layer by
# layer, so the distance of the current layer is known implicitly.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])

    W = n + 4                       # row width including a 2-cell border
    dist = [-2] * (W * W)           # -2 = padding / wall (skipped in BFS)
    for r in range(n):              # mark the real n x n interior as unvisited
        base = (r + 2) * W + 2
        dist[base:base + n] = [-1] * n

    start = 2 * W + 2               # padded index of corner (0,0)
    dist[start] = 0

    # Eight knight moves as flat offsets on the padded grid.
    offs = (W + 2, W - 2, -W + 2, -W - 2,
            2 * W + 1, 2 * W - 1, -2 * W + 1, -2 * W - 1)

    cur = [start]
    d = 1
    while cur:
        nxt = []
        ap = nxt.append
        for idx in cur:
            for off in offs:
                j = idx + off
                if dist[j] == -1:
                    dist[j] = d
                    ap(j)
        cur = nxt
        d += 1

    out = []
    for r in range(n):
        base = (r + 2) * W + 2
        out.append(' '.join(map(str, dist[base:base + n])))
    sys.stdout.buffer.write(('\n'.join(out) + '\n').encode())


main()
