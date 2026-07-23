import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    # Số xu tại mỗi phòng (đánh số 1..n).
    coins = [0] * (n + 1)
    for i in range(1, n + 1):
        coins[i] = int(data[pos]); pos += 1

    # Lưu cạnh và đếm bậc ra để dựng danh sách kề dạng CSR.
    ea = [0] * m          # đỉnh đầu mỗi cạnh
    eb = [0] * m          # đỉnh cuối mỗi cạnh
    off = [0] * (n + 2)   # CSR offsets
    for e in range(m):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        ea[e] = a
        eb[e] = b
        off[a + 1] += 1
    for i in range(1, n + 2):
        off[i] += off[i - 1]

    # Điền các đỉnh kề vào mảng CSR liên tục.
    adj = [0] * m
    fill = off[:]  # con trỏ điền tạm
    for e in range(m):
        a = ea[e]
        adj[fill[a]] = eb[e]
        fill[a] += 1

    # ---- Bước 1: Tarjan SCC (iterative, dùng stack tường minh tránh đệ quy sâu) ----
    index_arr = [-1] * (n + 1)
    low = [0] * (n + 1)
    comp = [-1] * (n + 1)
    on_stack = bytearray(n + 1)
    ptr = off[:]  # con trỏ cạnh hiện tại cho mỗi đỉnh
    sccstack = []
    idx_counter = 0
    ncomp = 0

    for s in range(1, n + 1):
        if index_arr[s] != -1:
            continue
        work = [s]  # stack mô phỏng đệ quy DFS
        index_arr[s] = idx_counter
        low[s] = idx_counter
        idx_counter += 1
        sccstack.append(s)
        on_stack[s] = 1
        while work:
            v = work[-1]
            ei = ptr[v]
            end = off[v + 1]
            recurse = False
            while ei < end:
                w = adj[ei]
                ei += 1
                if index_arr[w] == -1:
                    # Cạnh cây: đi sâu xuống đỉnh chưa thăm.
                    ptr[v] = ei
                    index_arr[w] = idx_counter
                    low[w] = idx_counter
                    idx_counter += 1
                    sccstack.append(w)
                    on_stack[w] = 1
                    work.append(w)
                    recurse = True
                    break
                elif on_stack[w] and index_arr[w] < low[v]:
                    # Cạnh ngược tới đỉnh còn trên stack: cập nhật low.
                    low[v] = index_arr[w]
            if recurse:
                continue
            ptr[v] = ei
            if low[v] == index_arr[v]:
                # v là gốc SCC: pop toàn bộ thành phần và gán id.
                while True:
                    w = sccstack.pop()
                    on_stack[w] = 0
                    comp[w] = ncomp
                    if w == v:
                        break
                ncomp += 1
            work.pop()
            # Truyền low của con lên cha.
            if work:
                pv = work[-1]
                if low[v] < low[pv]:
                    low[pv] = low[v]

    # ---- Bước 2: Giá trị mỗi SCC = tổng xu các phòng bên trong ----
    comp_val = [0] * ncomp
    for v in range(1, n + 1):
        comp_val[comp[v]] += coins[v]

    # ---- Bước 3: Danh sách kề trên DAG condense (chỉ giữ cạnh giữa hai SCC khác nhau) ----
    cond_adj = [[] for _ in range(ncomp)]
    for v in range(1, n + 1):
        cv = comp[v]
        for idx in range(off[v], off[v + 1]):
            cw = comp[adj[idx]]
            if cv != cw:
                cond_adj[cv].append(cw)

    # ---- Bước 4: DP đường đi tổng lớn nhất. Id tăng dần là thứ tự topo đảo ngược,
    # mọi successor có id nhỏ hơn nên đã được tính dp trước. ----
    dp = [0] * ncomp
    ans = 0
    for c in range(ncomp):
        best = 0
        for cw in cond_adj[c]:
            d = dp[cw]
            if d > best:
                best = d
        cur = comp_val[c] + best
        dp[c] = cur
        if cur > ans:
            ans = cur

    sys.stdout.write(str(ans) + "\n")


main()
