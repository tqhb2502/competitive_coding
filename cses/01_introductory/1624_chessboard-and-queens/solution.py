# Chessboard and Queens - https://cses.fi/problemset/task/1624
import sys


def main():
    data = sys.stdin.buffer.read().split()
    board = [row.decode() for row in data[:8]]

    col = [False] * 8
    diag1 = [False] * 15  # hàng - j + 7
    diag2 = [False] * 15  # hàng + j

    count = 0

    def backtrack(row):
        nonlocal count
        if row == 8:
            count += 1
            return
        for j in range(8):
            if board[row][j] == '*':
                continue
            d1 = row - j + 7
            d2 = row + j
            if col[j] or diag1[d1] or diag2[d2]:
                continue
            col[j] = diag1[d1] = diag2[d2] = True
            backtrack(row + 1)
            col[j] = diag1[d1] = diag2[d2] = False

    backtrack(0)
    sys.stdout.write(str(count) + "\n")


main()
