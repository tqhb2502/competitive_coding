# Grid Paths - CSES 1625
# https://cses.fi/problemset/task/1625
#
# Count Hamiltonian paths on a 7x7 grid from the upper-left square to the
# lower-left square that match a 48-character description (D/U/L/R with '?'
# acting as a wildcard). Solved with pruned backtracking.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0].decode() if data else ""

    # 9x9 padded grid so the border is always "blocked" -> no bound checks.
    W = 9
    N = W * W
    occ = bytearray(N)
    for i in range(W):
        occ[i] = 1              # top border row
        occ[(W - 1) * W + i] = 1  # bottom border row
        occ[i * W] = 1          # left border col
        occ[i * W + W - 1] = 1  # right border col

    START = W + 1        # real cell (row 1, col 1)
    END = 7 * W + 1      # real cell (row 7, col 1) = lower-left

    D, U, L, R = W, -W, -1, 1
    dmap = {
        'D': (D,), 'U': (U,), 'L': (L,), 'R': (R,),
        '?': (D, U, L, R),
    }
    steps = [dmap[c] for c in s]

    count = 0
    occ[START] = 1

    sys.setrecursionlimit(1000)

    def dfs(pos, step):
        nonlocal count
        if pos == END:
            # A valid full path ends at the lower-left square after 48 moves
            # (having visited all 49 squares). Reaching it earlier is a dead end.
            if step == 48:
                count += 1
            return
        if step == 48:
            return

        # Split pruning: if we can only continue perpendicular to a corridor
        # whose two ends are both blocked, the grid is cut into two parts that
        # both still contain unvisited squares -> impossible to finish.
        up = occ[pos - W]
        down = occ[pos + W]
        left = occ[pos - 1]
        right = occ[pos + 1]
        if up and down and not (left or right):
            return
        if left and right and not (up or down):
            return

        for d in steps[step]:
            npos = pos + d
            if not occ[npos]:
                occ[npos] = 1
                dfs(npos, step + 1)
                occ[npos] = 0

    dfs(START, 0)
    sys.stdout.write(str(count) + "\n")


main()
