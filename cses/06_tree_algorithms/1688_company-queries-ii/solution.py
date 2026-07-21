# Company Queries II - https://cses.fi/problemset/task/1688
# LCA (lowest common boss) bằng binary lifting, iterative, thuần stdlib.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    parent = [0] * (n + 1)
    depth = [0] * (n + 1)
    parent[1] = 1  # gốc tự trỏ vào chính nó để phép nâng luôn dừng ở gốc
    depth[1] = 0
    for i in range(2, n + 1):
        p = int(data[pos]); pos += 1
        parent[i] = p
        depth[i] = depth[p] + 1

    LOG = max(1, n.bit_length())  # số mức đủ để bao phủ độ sâu lớn nhất

    # up[k][v] = tổ tiên thứ 2^k của v (dựng lặp, không đệ quy)
    up = [parent]
    for _ in range(1, LOG):
        prev = up[-1]
        cur = [0] * (n + 1)
        for v in range(1, n + 1):
            cur[v] = prev[prev[v]]
        up.append(cur)

    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        if depth[a] < depth[b]:
            a, b = b, a
        # đưa a lên cùng độ sâu với b
        diff = depth[a] - depth[b]
        k = 0
        while diff:
            if diff & 1:
                a = up[k][a]
            diff >>= 1
            k += 1
        if a == b:
            ap(a)
            continue
        # nâng đồng thời từ bit cao xuống
        for k in range(LOG - 1, -1, -1):
            uk = up[k]
            if uk[a] != uk[b]:
                a = uk[a]
                b = uk[b]
        ap(parent[a])

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
