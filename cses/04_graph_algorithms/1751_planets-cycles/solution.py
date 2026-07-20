# Planets Cycles - CSES 1751
# https://cses.fi/problemset/task/1751
# Functional graph: mỗi đỉnh có đúng 1 cạnh ra. Với mỗi đỉnh, tính số bước tới
# khi gặp lại một đỉnh đã thăm = (khoảng cách vào cycle) + (độ dài cycle).
# Duyệt lặp (iterative), không đệ quy, O(n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    t = [0] * (n + 1)
    for i in range(1, n + 1):
        t[i] = int(data[i])

    state = [0] * (n + 1)   # 0 = chưa thăm, 1 = đang trong chain, 2 = đã xong
    order = [0] * (n + 1)   # vị trí của đỉnh trong chain hiện tại
    ans = [0] * (n + 1)

    for s in range(1, n + 1):
        if state[s] != 0:
            continue

        chain = []
        node = s
        # Đi theo cổng dịch chuyển cho tới khi gặp đỉnh biên (state != 0).
        while state[node] == 0:
            state[node] = 1
            order[node] = len(chain)
            chain.append(node)
            node = t[node]

        if state[node] == 1:
            # Phát hiện chu trình mới nằm trong chain hiện tại.
            cyc_start = order[node]
            cyc_len = len(chain) - cyc_start
            # Các đỉnh trên chu trình: đáp án = cyc_len.
            for i in range(cyc_start, len(chain)):
                v = chain[i]
                ans[v] = cyc_len
                state[v] = 2
            # Các đỉnh đuôi trước chu trình: tăng dần +1 khi đi ngược lại.
            base = cyc_len
            for i in range(cyc_start - 1, -1, -1):
                v = chain[i]
                base += 1
                ans[v] = base
                state[v] = 2
        else:
            # state[node] == 2: node đã có đáp án; cả chain là đuôi.
            base = ans[node]
            for i in range(len(chain) - 1, -1, -1):
                v = chain[i]
                base += 1
                ans[v] = base
                state[v] = 2

    sys.stdout.write(' '.join(map(str, ans[1:])))


main()
