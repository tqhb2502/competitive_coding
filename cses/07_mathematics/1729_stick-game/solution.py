import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    moves = tuple(int(data[2 + i]) for i in range(k))
    maxp = max(moves)

    # win[i] == 1 -> thế THẮNG; 0 -> thế THUA (giá trị mặc định).
    # Nới rộng thêm maxp để chỉ số win[i + p] luôn hợp lệ, khỏi cần kiểm tra biên.
    win = bytearray(n + maxp + 1)
    res = bytearray(n)  # ký tự kết quả cho các vị trí 1..n

    W = 87  # ord('W')
    L = 76  # ord('L')

    mv = moves
    w = win
    r = res
    # Lan truyền tiến: duyệt i tăng dần nên khi đọc win[i] mọi thế thua j < i đã
    # đánh dấu xong các thế thắng của nó.
    for i in range(n + 1):
        if w[i]:
            # Thế THẮNG (i >= 1 vì win[0] mặc định 0 là thế thua).
            r[i - 1] = W
        else:
            # Thế THUA: mọi trạng thái i + p (p thuộc P) đều là thế THẮNG.
            for p in mv:
                w[i + p] = 1
            if i:
                r[i - 1] = L

    sys.stdout.write(r.decode())


main()
