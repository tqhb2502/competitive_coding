# Range Queries and Copies - https://cses.fi/problemset/task/1737
#
# Persistent segment tree (cây phân đoạn bền vững, path-copying).
# - roots[k] = root node của mảng thứ k (đánh chỉ số từ 1).
# - Truy vấn "1 k a x": point update -> tạo một phiên bản mới bằng path copying
#   (chỉ copy O(log n) node trên đường đi), rồi GÁN LẠI vào roots[k]
#   (nghĩa là mảng k trở thành phiên bản mới; các bản copy trước đó không bị ảnh hưởng).
# - Truy vấn "2 k a b": range sum trên cây roots[k].
# - Truy vấn "3 k": copy mảng k -> mảng mới CHIA SẺ chung root (roots.append(roots[k])).
#
# Node được lưu trong các mảng phẳng: left[], right[] (int32), val[] (int64).
# Node 0 = null (không dùng).

import sys
from array import array


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    arr = [0] * (n + 1)
    for i in range(1, n + 1):
        arr[i] = int(data[pos]); pos += 1

    # Số node tối đa: build ~ (2n-1); mỗi update type-1 thêm <= (height+1) <= 20 node.
    MAX = 2 * n + q * 20 + 10
    left = array('i', bytes(4 * MAX))
    right = array('i', bytes(4 * MAX))
    val = array('q', bytes(8 * MAX))
    cnt = 1  # node 0 dành cho null

    # ---- Build cây ban đầu (iterative, tránh đệ quy) ----
    root0 = cnt; cnt += 1
    order = []            # pre-order các internal node
    st = [(root0, 1, n)]
    while st:
        node, lo, hi = st.pop()
        if lo == hi:
            val[node] = arr[lo]
            continue
        mid = (lo + hi) >> 1
        l = cnt; cnt += 1
        r = cnt; cnt += 1
        left[node] = l
        right[node] = r
        order.append(node)
        st.append((l, lo, mid))
        st.append((r, mid + 1, hi))
    # Sum bottom-up: parent xuất hiện trước con trong order -> duyệt ngược.
    for node in reversed(order):
        val[node] = val[left[node]] + val[right[node]]

    roots = [0, root0]    # roots[0] không dùng, roots[1] = mảng ban đầu

    out = []
    append = out.append
    dat = data

    for _ in range(q):
        t = dat[pos]; pos += 1
        if t == b'2':
            k = int(dat[pos]); a = int(dat[pos + 1]); b = int(dat[pos + 2]); pos += 3
            res = 0
            stack = [(roots[k], 1, n)]
            sp = stack.pop
            spush = stack.append
            while stack:
                node, lo, hi = sp()
                if a <= lo and hi <= b:
                    res += val[node]
                    continue
                mid = (lo + hi) >> 1
                if a <= mid:
                    spush((left[node], lo, mid))
                if b > mid:
                    spush((right[node], mid + 1, hi))
            append(res)
        elif t == b'1':
            k = int(dat[pos]); a = int(dat[pos + 1]); x = int(dat[pos + 2]); pos += 3
            cur = roots[k]
            lo, hi = 1, n
            pnodes = []
            pdir = []
            pn_app = pnodes.append
            pd_app = pdir.append
            while lo < hi:
                mid = (lo + hi) >> 1
                if a <= mid:
                    pn_app(cur); pd_app(0)
                    cur = left[cur]; hi = mid
                else:
                    pn_app(cur); pd_app(1)
                    cur = right[cur]; lo = mid + 1
            # tạo lá mới
            newchild = cnt; cnt += 1
            val[newchild] = x
            # đi ngược lên, copy từng node
            for i in range(len(pnodes) - 1, -1, -1):
                old = pnodes[i]
                node = cnt; cnt += 1
                if pdir[i] == 0:
                    r = right[old]
                    left[node] = newchild
                    right[node] = r
                    val[node] = val[newchild] + val[r]
                else:
                    l = left[old]
                    left[node] = l
                    right[node] = newchild
                    val[node] = val[l] + val[newchild]
                newchild = node
            roots[k] = newchild
        else:  # t == b'3'  -> copy mảng k
            k = int(dat[pos]); pos += 1
            roots.append(roots[k])

    sys.stdout.buffer.write(("\n".join(map(str, out)) + ("\n" if out else "")).encode())


main()
