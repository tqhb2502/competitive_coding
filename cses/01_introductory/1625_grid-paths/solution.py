import sys


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0].decode() if data else ""

    # Lưới đệm 9x9: viền ngoài luôn được đánh dấu "bị chặn" -> khỏi kiểm tra biên.
    W = 9
    N = W * W
    occ = bytearray(N)
    for i in range(W):
        occ[i] = 1                # hàng biên trên
        occ[(W - 1) * W + i] = 1  # hàng biên dưới
        occ[i * W] = 1            # cột biên trái
        occ[i * W + W - 1] = 1    # cột biên phải

    START = W + 1        # ô thực (hàng 1, cột 1) = góc trên-trái
    END = 7 * W + 1      # ô thực (hàng 7, cột 1) = góc dưới-trái

    # Mã hóa 4 hướng D/U/L/R thành độ lệch chỉ số trên lưới đệm.
    D, U, L, R = W, -W, -1, 1
    dmap = {
        'D': (D,), 'U': (U,), 'L': (L,), 'R': (R,),
        '?': (D, U, L, R),  # dấu '?' là ký tự đại diện: thử cả 4 hướng
    }
    # Với mỗi bước của chuỗi mô tả, xác định trước tập hướng cần thử.
    steps = [dmap[c] for c in s]

    count = 0
    occ[START] = 1  # ô xuất phát coi như đã thăm

    sys.setrecursionlimit(1000)

    # Quay lui (DFS): đi theo từng bước của chuỗi mô tả, đếm số đường Hamilton hợp lệ.
    def dfs(pos, step):
        nonlocal count
        # Cắt theo đích đến: chỉ hợp lệ khi tới ô đích đúng ở bước cuối (step == 48);
        # tới sớm hơn là ngõ cụt vì ô đích chỉ được thăm 1 lần.
        if pos == END:
            if step == 48:
                count += 1
            return
        if step == 48:
            return

        # Trạng thái 4 ô kề: giá trị 1 nghĩa là "bị chặn" (tường biên hoặc đã thăm).
        up = occ[pos - W]
        down = occ[pos + W]
        left = occ[pos - 1]
        right = occ[pos + 1]

        # Cắt theo "chia đôi lưới": nếu buộc phải rẽ ngang một hành lang mà hai đầu
        # đều bị chặn thì lưới bị cắt rời thành hai phần vẫn còn ô chưa thăm -> dừng.
        if up and down and not (left or right):
            return
        if left and right and not (up or down):
            return

        # Chỉ đi sang ô chưa thăm; đánh dấu, đệ quy rồi bỏ đánh dấu khi quay lui.
        for d in steps[step]:
            npos = pos + d
            if not occ[npos]:
                occ[npos] = 1
                dfs(npos, step + 1)
                occ[npos] = 0

    dfs(START, 0)
    sys.stdout.write(str(count) + "\n")


main()
