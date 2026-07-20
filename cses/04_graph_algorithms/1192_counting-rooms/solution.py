# Counting Rooms - CSES 1192
# https://cses.fi/problemset/task/1192
# Đếm số thành phần liên thông (connected components) của các ô sàn trên lưới
# bằng flood fill (iterative DFS). Lưới được đệm (padding) một viền tường để
# tránh kiểm tra biên.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    rows = data[2:2 + n]

    # Bảng chuyển: '.' (46) -> 1 (sàn), mọi ký tự khác -> 0 (tường)
    table = bytes(1 if c == 46 else 0 for c in range(256))

    W = m + 2  # chiều rộng sau khi đệm viền
    g = bytearray((n + 2) * W)  # mặc định toàn 0 = tường (gồm cả viền)
    for i in range(n):
        base = (i + 1) * W + 1
        g[base:base + m] = rows[i].translate(table)

    count = 0
    stack = []
    push = stack.append
    pop = stack.pop
    total = len(g)

    for start in range(total):
        if g[start]:
            count += 1
            g[start] = 0
            push(start)
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
