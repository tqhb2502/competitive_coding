import sys


def main():
    # Đọc bàn cờ 8x8: mỗi ô là '.' (tự do) hoặc '*' (bị cấm).
    data = sys.stdin.buffer.read().split()
    board = [row.decode() for row in data[:8]]

    # Mảng đánh dấu trạng thái đã bị chiếm.
    col = [False] * 8      # cột j đã có quân hậu chưa
    diag1 = [False] * 15   # đường chéo chính, chỉ số (row - j + 7)
    diag2 = [False] * 15   # đường chéo phụ, chỉ số (row + j)

    count = 0

    # Quay lui theo từng hàng, mỗi hàng đặt đúng một quân hậu.
    def backtrack(row):
        nonlocal count
        # Đặt xong cả 8 hàng => tìm được một cách hợp lệ.
        if row == 8:
            count += 1
            return
        # Thử đặt quân hậu vào từng cột của hàng hiện tại.
        for j in range(8):
            if board[row][j] == '*':  # ô bị cấm, không đặt được
                continue
            d1 = row - j + 7
            d2 = row + j
            # Bỏ qua nếu cột hoặc đường chéo đã bị chiếm.
            if col[j] or diag1[d1] or diag2[d2]:
                continue
            # Đánh dấu trạng thái rồi đệ quy sang hàng tiếp theo.
            col[j] = diag1[d1] = diag2[d2] = True
            backtrack(row + 1)
            # Bỏ đánh dấu (backtrack) để thử cột khác.
            col[j] = diag1[d1] = diag2[d2] = False

    backtrack(0)
    sys.stdout.write(str(count) + "\n")


main()
