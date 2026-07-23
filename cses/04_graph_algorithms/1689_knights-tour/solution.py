import sys


def main():
    data = sys.stdin.buffer.read().split()
    # x là cột, y là hàng (đánh số từ 1); ô xuất phát mang số 1
    x = int(data[0])
    y = int(data[1])

    N = 8
    r0 = y - 1
    c0 = x - 1

    # 8 nước đi hình chữ L của quân mã (knight)
    moves = ((-2, -1), (-2, 1), (-1, -2), (-1, 2),
             (1, -2), (1, 2), (2, -1), (2, 1))

    # board[r][c] = thứ tự bước; 0 nghĩa là chưa thăm
    board = [[0] * N for _ in range(N)]

    def count_onward(r, c):
        # Đếm số ô kề chưa thăm của (r, c) - dùng cho heuristic Warnsdorff
        cnt = 0
        for dr, dc in moves:
            nr = r + dr
            nc = c + dc
            if 0 <= nr < N and 0 <= nc < N and board[nr][nc] == 0:
                cnt += 1
        return cnt

    sys.setrecursionlimit(10000)

    def dfs(r, c, step):
        # DFS backtracking đánh số bước từ 1 tới 64
        board[r][c] = step
        if step == N * N:
            return True  # đã đi qua đủ 64 ô, tìm được đường đi Hamilton
        # Gom các ô kề chưa thăm, sắp theo heuristic Warnsdorff: số nước đi tiếp
        # theo tăng dần. Khi bằng nhau, ưu tiên ô xa tâm bàn cờ hơn (tọa độ nhân
        # đôi để tâm 3.5 thành 7). Phá hòa xác định này giúp nhánh đầu luôn thành
        # công nên DFS gần như không phải quay lui (64 lời gọi mỗi lần giải).
        cands = []
        for dr, dc in moves:
            nr = r + dr
            nc = c + dc
            if 0 <= nr < N and 0 <= nc < N and board[nr][nc] == 0:
                far = -((2 * nr - (N - 1)) ** 2 + (2 * nc - (N - 1)) ** 2)
                cands.append((count_onward(nr, nc), far, nr, nc))
        cands.sort()
        # Thử lần lượt từng ô kề theo thứ tự heuristic
        for _, _, nr, nc in cands:
            if dfs(nr, nc, step + 1):
                return True
        board[r][c] = 0  # ngõ cụt: quay lui, đặt lại ô chưa thăm
        return False

    dfs(r0, c0, 1)

    # In bàn cờ 8x8 với thứ tự bước ở mỗi ô
    out = []
    for r in range(N):
        out.append(' '.join(str(board[r][c]) for c in range(N)))
    sys.stdout.write('\n'.join(out) + '\n')


main()
