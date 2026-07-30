import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    t = [0] * (n + 1)
    for i in range(1, n + 1):
        t[i] = int(data[pos]); pos += 1

    # ---- 1) Tìm các đỉnh nằm trên cycle (iterative) ----
    state = [0] * (n + 1)      # 0 chưa thăm, 1 đang trên đường đi, 2 đã xong
    order = [0] * (n + 1)      # chỉ số của đỉnh trong đường đi hiện tại
    on_cycle = bytearray(n + 1)
    for start in range(1, n + 1):
        if state[start] != 0:
            continue
        path = []
        v = start
        while state[v] == 0:
            state[v] = 1
            order[v] = len(path)
            path.append(v)
            v = t[v]
        if state[v] == 1:  # gặp lại đỉnh đang trên đường đi -> cycle mới
            si = order[v]
            for j in range(si, len(path)):
                on_cycle[path[j]] = 1
        for node in path:
            state[node] = 2

    # ---- 2) Gán cycle_id, pos_in_cycle, độ dài cycle ----
    cycle_id = [-1] * (n + 1)
    pos_in_cycle = [0] * (n + 1)
    cyclen = []
    cid = 0
    for v in range(1, n + 1):
        if on_cycle[v] and cycle_id[v] == -1:
            cur = v
            p = 0
            while cycle_id[cur] == -1:
                cycle_id[cur] = cid
                pos_in_cycle[cur] = p
                p += 1
                cur = t[cur]
            cyclen.append(p)
            cid += 1

    # ---- 3) Rừng cây (cạnh ngược, bỏ cạnh cycle) ----
    children = [[] for _ in range(n + 1)]
    for u in range(1, n + 1):
        if not on_cycle[u]:
            children[t[u]].append(u)

    depth = [0] * (n + 1)
    root = [0] * (n + 1)
    in_t = [0] * (n + 1)
    out_t = [0] * (n + 1)
    it = [0] * (n + 1)   # con trỏ con cho DFS iterative

    timer = 0
    for c in range(1, n + 1):
        if not on_cycle[c]:
            continue
        root[c] = c
        depth[c] = 0
        in_t[c] = timer
        timer += 1
        stack = [c]
        while stack:
            node = stack[-1]
            i = it[node]
            chl = children[node]
            if i < len(chl):
                it[node] = i + 1
                ch = chl[i]
                depth[ch] = depth[node] + 1
                root[ch] = c
                in_t[ch] = timer
                timer += 1
                stack.append(ch)
            else:
                out_t[node] = timer - 1
                stack.pop()

    # ---- 4) Trả lời truy vấn ----
    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        if on_cycle[b]:
            ra = root[a]
            cb = cycle_id[b]
            if cycle_id[ra] == cb:
                L = cyclen[cb]
                ap(depth[a] + (pos_in_cycle[b] - pos_in_cycle[ra]) % L)
            else:
                ap(-1)
        else:
            if in_t[b] <= in_t[a] <= out_t[b]:
                ap(depth[a] - depth[b])
            else:
                ap(-1)

    sys.stdout.write('\n'.join(map(str, out)))
    if out:
        sys.stdout.write('\n')


main()
