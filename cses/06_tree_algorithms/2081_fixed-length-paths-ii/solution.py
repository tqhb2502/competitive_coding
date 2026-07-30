import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    k1 = int(data[pos]); pos += 1
    k2 = int(data[pos]); pos += 1

    # Danh sách kề của cây
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        adj[b].append(a)

    removed = bytearray(n + 1)   # đỉnh đã bị chọn làm centroid và xóa
    subsize = [0] * (n + 1)      # kích thước cây con trong component hiện tại
    par = [0] * (n + 1)          # cha khi duyệt component để tính size
    cpar = [0] * (n + 1)         # cha khi duyệt depth từ centroid
    depthv = [0] * (n + 1)       # depth (số cạnh) từ centroid tới mỗi đỉnh

    k1m1 = k1 - 1

    def count_leq(arr, X):
        # Số cặp không thứ tự (i < j) có arr[i] + arr[j] <= X, arr đã sắp tăng.
        # Two-pointer O(len).
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
    stack = [1]   # các component chờ xử lý (đại diện bởi một đỉnh bất kỳ)

    while stack:
        start = stack.pop()
        if removed[start]:
            continue

        # --- Duyệt component (iterative) và tính subtree size khi root tại start ---
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
        # Duyệt ngược thứ tự phát hiện để cộng size từ lá lên gốc
        for u in reversed(comp):
            s = 1
            pu = par[u]
            for v in adj[u]:
                if not removed[v] and v != pu:
                    s += subsize[v]
            subsize[u] = s
        total = len(comp)

        # --- Tìm centroid: đi về phía cây con có kích thước > total/2 ---
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

        # --- Đóng góp của các path đi qua centroid c (inclusion-exclusion) ---
        # D_all = centroid (depth 0) + mọi node trong component (depth tính từ c).
        # contrib = count_range(D_all) - sum_child count_range(child).
        D_all = [0]
        contrib = 0
        for nb in adj[c]:
            if removed[nb]:
                continue
            # DFS iterative trong một cây con của c để lấy mảng depth
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
            # Trừ đi các cặp cùng một cây con (đường đi thật giữa chúng không qua c)
            if len(child) >= 2:
                contrib -= count_leq(child, k2) - count_leq(child, k1m1)
            D_all += child
        # count_range trên toàn bộ D_all rồi cộng vào đáp số
        D_all.sort()
        contrib += count_leq(D_all, k2) - count_leq(D_all, k1m1)
        ans += contrib

        # --- Xóa centroid, đẩy các component con vào stack để đệ quy ---
        removed[c] = 1
        for nb in adj[c]:
            if not removed[nb]:
                stack.append(nb)

    sys.stdout.buffer.write((str(ans) + "\n").encode())


main()
