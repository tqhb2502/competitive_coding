# Fixed-Length Paths II - https://cses.fi/problemset/task/2081
# Đếm số đường đi (path) trong cây có số cạnh nằm trong [k1, k2].
# Kỹ thuật: Centroid Decomposition + two-pointer đếm cặp có tổng depth trong khoảng.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    k1 = int(data[pos]); pos += 1
    k2 = int(data[pos]); pos += 1

    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        adj[b].append(a)

    removed = bytearray(n + 1)
    subsize = [0] * (n + 1)
    par = [0] * (n + 1)
    cpar = [0] * (n + 1)
    depthv = [0] * (n + 1)

    k1m1 = k1 - 1

    def count_leq(arr, X):
        # Số cặp không thứ tự (i < j) có arr[i] + arr[j] <= X, arr đã sắp tăng.
        l = 0
        r = len(arr) - 1
        c = 0
        while l < r:
            if arr[l] + arr[r] <= X:
                c += r - l
                l += 1
            else:
                r -= 1
        return c

    ans = 0
    stack = [1]

    while stack:
        start = stack.pop()
        if removed[start]:
            continue

        # --- Tính component + subtree size khi root tại start (iterative) ---
        comp = []
        par[start] = 0
        st = [start]
        while st:
            u = st.pop()
            comp.append(u)
            pu = par[u]
            for v in adj[u]:
                if not removed[v] and v != pu:
                    par[v] = u
                    st.append(v)
        for u in reversed(comp):
            s = 1
            pu = par[u]
            for v in adj[u]:
                if not removed[v] and v != pu:
                    s += subsize[v]
            subsize[u] = s
        total = len(comp)

        # --- Tìm centroid ---
        u = start
        p = 0
        while True:
            best = -1
            for v in adj[u]:
                if removed[v] or v == p:
                    continue
                if 2 * subsize[v] > total:
                    best = v
                    break
            if best == -1:
                break
            p = u
            u = best
        c = u

        # --- Tính đóng góp của các path đi qua centroid c ---
        # D_all gồm centroid (depth 0) + tất cả node trong component (depth tính từ c).
        # contrib = count_range(D_all) - sum_child count_range(child)  (inclusion-exclusion).
        D_all = [0]
        contrib = 0
        for nb in adj[c]:
            if removed[nb]:
                continue
            cpar[nb] = c
            depthv[nb] = 1
            st2 = [nb]
            child = []
            capp = child.append
            while st2:
                x = st2.pop()
                dx = depthv[x]
                capp(dx)
                px = cpar[x]
                for v in adj[x]:
                    if not removed[v] and v != px:
                        cpar[v] = x
                        depthv[v] = dx + 1
                        st2.append(v)
            child.sort()
            if len(child) >= 2:
                contrib -= count_leq(child, k2) - count_leq(child, k1m1)
            D_all += child
        D_all.sort()
        contrib += count_leq(D_all, k2) - count_leq(D_all, k1m1)
        ans += contrib

        # --- Xóa centroid, đẩy các component con vào stack ---
        removed[c] = 1
        for nb in adj[c]:
            if not removed[nb]:
                stack.append(nb)

    sys.stdout.buffer.write((str(ans) + "\n").encode())


main()
