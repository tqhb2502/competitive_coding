import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    rows = data[2:2 + n]

    # Bảng chuyển: '.' (mã 46) -> 1 (sàn), mọi ký tự khác -> 0 (tường).
    table = bytes(1 if c == 46 else 0 for c in range(256))

    # Đệm một viền tường quanh lưới: mảng phẳng (n+2) x (m+2), mặc định toàn 0
    # (tường) để khỏi kiểm tra biên khi mở rộng lân cận. bytes.translate đổi
    # từng dòng '.'/'#' sang 1/0 ở tốc độ C.
    W = m + 2  # chiều rộng sau khi đệm viền
    g = bytearray((n + 2) * W)
    for i in range(n):
        base = (i + 1) * W + 1
        g[base:base + m] = rows[i].translate(table)

    count = 0
    stack = []
    push = stack.append  # bind cục bộ để tăng tốc vòng lặp
    pop = stack.pop
    total = len(g)

    # Duyệt toàn lưới: mỗi ô sàn chưa thăm mở đầu một phòng mới.
    for start in range(total):
        if g[start]:
            count += 1
            g[start] = 0  # đánh dấu đã thăm bằng cách đặt lại thành tường
            push(start)
            # Flood fill (DFS lặp) quét trọn thành phần liên thông hiện tại,
            # xét lần lượt 4 lân cận phải/trái/trên/dưới.
            while stack:
                cur = pop()
                r = cur + 1
                if g[r]:
                    g[r] = 0
                    push(r)
                l = cur - 1
                if g[l]:
                    g[l] = 0
                    push(l)
                u = cur - W
                if g[u]:
                    g[u] = 0
                    push(u)
                d = cur + W
                if g[d]:
                    g[d] = 0
                    push(d)

    sys.stdout.write(str(count) + "\n")


main()
