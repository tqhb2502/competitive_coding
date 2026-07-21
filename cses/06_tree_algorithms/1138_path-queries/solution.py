# Path Queries (CSES 1138)
# https://cses.fi/problemset/task/1138
#
# Euler tour (tin/tout) + Fenwick/BIT với kỹ thuật "điểm vào/ra":
#   cộng +val tại tin[v], -val tại tout[v]+1  =>  path-sum(root..s) = prefix_sum(tin[s]).
# DFS được làm theo kiểu iterative để tránh RecursionError khi n lớn.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    val = [0] * (n + 1)
    for i in range(1, n + 1):
        val[i] = int(data[pos]); pos += 1

    # Danh sách kề
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        adj[a].append(b)
        adj[b].append(a)

    # Euler tour bằng DFS lặp: tính tin[], tout[], parent[]
    tin = [0] * (n + 1)
    tout = [0] * (n + 1)
    parent = [0] * (n + 1)
    timer = 0
    stack = [1]
    while stack:
        v = stack.pop()
        if v > 0:
            timer += 1
            tin[v] = timer
            stack.append(-v)  # marker để gán tout khi ra khỏi v
            pv = parent[v]
            for u in adj[v]:
                if u != pv:
                    parent[u] = v
                    stack.append(u)
        else:
            v = -v
            tout[v] = timer

    # BIT có chỉ số 1..size, size = n+1 (vì có thể cộng tại tout[v]+1 = n+1)
    size = n + 1

    # Khởi tạo BIT trong O(n) từ mảng hiệu (difference array)
    diff = [0] * (size + 2)
    for v in range(1, n + 1):
        vv = val[v]
        diff[tin[v]] += vv
        diff[tout[v] + 1] -= vv

    bit = [0] * (size + 1)
    for i in range(1, size + 1):
        bit[i] += diff[i]
        j = i + (i & -i)
        if j <= size:
            bit[j] += bit[i]

    out = []
    for _ in range(q):
        t = data[pos]; pos += 1
        if t == b'2':
            s = int(data[pos]); pos += 1
            i = tin[s]
            ssum = 0
            while i > 0:
                ssum += bit[i]
                i -= i & -i
            out.append(ssum)
        else:  # b'1'
            s = int(data[pos]); pos += 1
            x = int(data[pos]); pos += 1
            delta = x - val[s]
            val[s] = x
            # point update: +delta tại tin[s], -delta tại tout[s]+1
            i = tin[s]
            while i <= size:
                bit[i] += delta
                i += i & -i
            i = tout[s] + 1
            nd = -delta
            while i <= size:
                bit[i] += nd
                i += i & -i

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + ('\n' if out else '')).encode())


main()
