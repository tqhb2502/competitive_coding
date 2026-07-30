import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Lưu cạnh dạng mảng song song và dựng đồ thị xuôi/đảo để cắt tỉa
    A = [0] * m
    B = [0] * m
    X = [0] * m
    fadj = [[] for _ in range(n + 1)]  # đồ thị xuôi
    radj = [[] for _ in range(n + 1)]  # đồ thị đảo ngược

    for i in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); x = int(data[idx + 2])
        idx += 3
        A[i] = a; B[i] = b; X[i] = x
        fadj[a].append(b)
        radj[b].append(a)

    # R1: các đỉnh tới được từ đỉnh 1 (BFS trên đồ thị xuôi)
    R1 = bytearray(n + 1)
    dq = deque([1]); R1[1] = 1
    while dq:
        u = dq.popleft()
        for v in fadj[u]:
            if not R1[v]:
                R1[v] = 1
                dq.append(v)

    # R2: các đỉnh có thể đi tới đỉnh n (BFS trên đồ thị đảo)
    R2 = bytearray(n + 1)
    dq = deque([n]); R2[n] = 1
    while dq:
        u = dq.popleft()
        for v in radj[u]:
            if not R2[v]:
                R2[v] = 1
                dq.append(v)

    # good = R1 giao R2: chỉ những đỉnh này mới ảnh hưởng tới kết quả
    good = bytearray(n + 1)
    cntgood = 0
    for i in range(1, n + 1):
        if R1[i] and R2[i]:
            good[i] = 1
            cntgood += 1

    # Chỉ giữ các cạnh có cả hai đầu mút thuộc good
    edges = []
    out_adj = [[] for _ in range(n + 1)]  # danh sách kề xuôi chỉ gồm cạnh good
    for i in range(m):
        a = A[i]; b = B[i]
        if good[a] and good[b]:
            x = X[i]
            edges.append((a, b, x))
            out_adj[a].append((b, x))

    # Bellman-Ford tối đa hóa: dist[1] = 0, các đỉnh khác = -vô cực
    NEG = -(1 << 60)
    dist = [NEG] * (n + 1)
    dist[1] = 0

    # Chạy Bellman-Ford theo vòng, nhưng mỗi vòng chỉ duyệt các đỉnh vừa được cập
    # nhật ở vòng trước (active set). Vẫn giữ chặn O(V*E) nhưng nhanh hơn nhiều trên
    # thực tế vì tránh quét lại toàn bộ cạnh khi phần lớn đỉnh đã ổn định.
    active = [1]
    inq = bytearray(n + 1)
    rounds = cntgood - 1 if cntgood > 0 else 0
    changed = True
    for _ in range(rounds):
        nxt = []
        for u in active:
            du = dist[u]
            for v, w in out_adj[u]:
                nd = du + w
                if nd > dist[v]:
                    dist[v] = nd
                    if not inq[v]:
                        inq[v] = 1
                        nxt.append(v)
        for v in nxt:
            inq[v] = 0
        active = nxt
        if not active:
            # đạt điểm dừng cố định -> chắc chắn không có chu trình dương
            changed = False
            break

    # Vòng kiểm tra: sau (cntgood - 1) vòng mà vẫn còn relax được thì có chu trình
    # dương. Vì mọi đỉnh good đều đi tới được n nên chu trình đó nằm trên đường đi
    # 1 -> n, đáp án là -1. Vòng này quét toàn bộ cạnh good, chi phí O(E).
    has_pos_cycle = False
    if changed:
        for a, b, x in edges:
            da = dist[a]
            if da != NEG and da + x > dist[b]:
                has_pos_cycle = True
                break

    if has_pos_cycle:
        sys.stdout.write("-1\n")
    else:
        sys.stdout.write(str(dist[n]) + "\n")


main()
