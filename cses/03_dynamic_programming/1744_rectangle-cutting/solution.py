import sys
from operator import add


def main():
    data = sys.stdin.buffer.read().split()
    a = int(data[0])
    b = int(data[1])
    # Hình vuông sẵn: không cần cắt lần nào.
    if a == b:
        sys.stdout.write("0\n")
        return

    # dp[i][j] = số lần cắt tối thiểu để chia hình chữ nhật i × j thành các hình
    # vuông. Bảng (N+1) x (N+1) khởi tạo 0 nên đường chéo dp[i][i] = 0 có sẵn.
    N = a if a > b else b
    dp = [[0] * (N + 1) for _ in range(N + 1)]

    # Gán các tên nóng vào biến cục bộ để tăng tốc trong CPython.
    _min = min
    _map = map

    # Chỉ tính nửa tam giác trên (i < j) rồi soi gương, vì dp[i][j] = dp[j][i].
    for i in range(1, N + 1):
        row_i = dp[i]
        hi = i >> 1  # i // 2
        for j in range(i + 1, N + 1):
            # Cắt theo cạnh dài j (giữ nguyên i): dp[i][k] + dp[i][j-k], k = 1..j//2.
            # Gộp min bằng map(add, lát_trái, lát_phải) để phần cộng chạy trong C.
            jh = j >> 1
            best = _min(_map(add, row_i[1:jh + 1], row_i[j - 1:j - 1 - jh:-1]))
            # Cắt theo cạnh dài i (giữ nguyên j): dp[k][j] + dp[i-k][j], k = 1..i//2.
            # Nhờ đối xứng dp[k][j] = dp[j][k] = row_j[k], các ô này đã được điền
            # (hàng j đã được soi gương ở các i nhỏ hơn). Chỉ có hướng này khi i >= 2.
            if hi:
                row_j = dp[j]
                v = _min(_map(add, row_j[1:hi + 1], row_j[i - 1:i - 1 - hi:-1]))
                if v < best:
                    best = v
            best += 1  # cộng 1 cho chính lần cắt đầu tiên
            row_i[j] = best
            dp[j][i] = best  # soi gương

    # Đáp án là dp[a][b].
    sys.stdout.write(str(dp[a][b]) + "\n")


main()
