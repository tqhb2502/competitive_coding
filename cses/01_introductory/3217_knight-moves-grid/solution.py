import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])

    # Trải bàn cờ thành mảng 1 chiều phẳng có viền dày 2 ô mỗi bên.
    # Viền = -2 (tường, coi như đã thăm nên bị bỏ qua trong BFS).
    W = n + 4                       # chiều rộng mỗi hàng, gồm cả viền 2 ô
    dist = [-2] * (W * W)
    for r in range(n):              # đánh dấu vùng n x n thực sự = -1 (chưa thăm)
        base = (r + 2) * W + 2
        dist[base:base + n] = [-1] * n

    start = 2 * W + 2               # chỉ số phẳng của góc (0,0)
    dist[start] = 0

    # 8 nước đi của quân mã đổi thành 8 độ lệch chỉ số trên mảng phẳng.
    offs = (W + 2, W - 2, -W + 2, -W - 2,
            2 * W + 1, 2 * W - 1, -2 * W + 1, -2 * W - 1)

    # BFS theo TỪNG LỚP: cur giữ các ô ở khoảng cách d-1, khoảng cách d biết sẵn
    # nên không cần lưu kèm trong hàng đợi. Viền -2 tự chặn nước đi ra ngoài biên.
    cur = [start]
    d = 1
    while cur:
        nxt = []
        ap = nxt.append
        for idx in cur:
            for off in offs:
                j = idx + off
                if dist[j] == -1:   # chỉ đi tới ô chưa thăm
                    dist[j] = d
                    ap(j)
        cur = nxt
        d += 1

    # In ma trận khoảng cách, chỉ lấy phần bên trong, các số cách nhau bởi dấu cách.
    out = []
    for r in range(n):
        base = (r + 2) * W + 2
        out.append(' '.join(map(str, dist[base:base + n])))
    sys.stdout.buffer.write(('\n'.join(out) + '\n').encode())


main()
