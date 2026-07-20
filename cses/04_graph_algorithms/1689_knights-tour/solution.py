# Knight's Tour (CSES 1689)
# https://cses.fi/problemset/task/1689
#
# Find a Hamiltonian path of a knight over the 8x8 board starting at (x, y),
# where x is the column and y is the row (1-indexed). We use backtracking DFS
# ordered by Warnsdorff's heuristic (try squares with the fewest onward moves
# first), which finds a valid tour almost instantly on the fixed 8x8 board.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    x = int(data[0])
    y = int(data[1])

    N = 8
    # x = column, y = row (1-indexed). Cell "1" must sit at the start square.
    r0 = y - 1
    c0 = x - 1

    moves = ((-2, -1), (-2, 1), (-1, -2), (-1, 2),
             (1, -2), (1, 2), (2, -1), (2, 1))

    board = [[0] * N for _ in range(N)]

    def count_onward(r, c):
        # number of unvisited squares reachable from (r, c)
        cnt = 0
        for dr, dc in moves:
            nr = r + dr
            nc = c + dc
            if 0 <= nr < N and 0 <= nc < N and board[nr][nc] == 0:
                cnt += 1
        return cnt

    sys.setrecursionlimit(10000)

    def dfs(r, c, step):
        board[r][c] = step
        if step == N * N:
            return True
        # collect unvisited neighbours, order by Warnsdorff heuristic.
        # Ties in the onward-move count are broken by preferring squares that
        # are farther from the board centre (doubled integer coordinates so the
        # centre 3.5 becomes 7). This deterministic tie-break makes the very
        # first branch always succeed, so DFS never backtracks on the 8x8 board
        # (64 calls per solve), removing any TLE risk.
        cands = []
        for dr, dc in moves:
            nr = r + dr
            nc = c + dc
            if 0 <= nr < N and 0 <= nc < N and board[nr][nc] == 0:
                far = -((2 * nr - (N - 1)) ** 2 + (2 * nc - (N - 1)) ** 2)
                cands.append((count_onward(nr, nc), far, nr, nc))
        cands.sort()
        for _, _, nr, nc in cands:
            if dfs(nr, nc, step + 1):
                return True
        board[r][c] = 0  # backtrack
        return False

    dfs(r0, c0, 1)

    out = []
    for r in range(N):
        out.append(' '.join(str(board[r][c]) for c in range(N)))
    sys.stdout.write('\n'.join(out) + '\n')


main()
