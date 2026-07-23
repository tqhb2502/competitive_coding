import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    # parent[v] = sếp trực tiếp của v; depth[v] = độ sâu của v (gốc sâu 0)
    parent = [0] * (n + 1)
    depth = [0] * (n + 1)
    parent[1] = 1  # gốc tự trỏ chính nó để phép nâng luôn dừng ở gốc
    depth[1] = 0
    # Vì sếp có chỉ số nhỏ hơn nên duyệt tăng dần tính được depth ngay, không cần DFS
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
            a, b = b, a  # giữ a là nút sâu hơn
        # Đưa a lên cùng độ sâu với b theo các bit của hiệu độ sâu
        diff = depth[a] - depth[b]
        k = 0
        while diff:
            if diff & 1:
                a = up[k][a]
            diff >>= 1
            k += 1
        if a == b:
            ap(a)  # b là tổ tiên của a nên chính là LCA
            continue
        # Nâng đồng thời từ bit cao xuống thấp tới ngay dưới LCA
        for k in range(LOG - 1, -1, -1):
            uk = up[k]
            if uk[a] != uk[b]:
                a = uk[a]
                b = uk[b]
        ap(parent[a])  # sếp của a chính là LCA

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
