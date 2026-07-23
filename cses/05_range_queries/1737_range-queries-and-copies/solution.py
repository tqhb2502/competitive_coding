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

    # Persistent segment tree lưu trong các mảng phẳng để tiết kiệm bộ nhớ:
    # left[], right[] kiểu int32; val[] kiểu int64. Node 0 dành cho null.
    # Số node tối đa: build ~ (2n-1); mỗi update type-1 thêm <= (height+1) <= 20 node.
    MAX = 2 * n + q * 20 + 10
    left = array('i', bytes(4 * MAX))
    right = array('i', bytes(4 * MAX))
    val = array('q', bytes(8 * MAX))
    cnt = 1  # node 0 dành cho null

    # ---- Build cây ban đầu bằng vòng lặp (iterative, tránh đệ quy) ----
    root0 = cnt; cnt += 1
    order = []            # các internal node theo thứ tự pre-order
    st = [(root0, 1, n)]
    while st:
        node, lo, hi = st.pop()
        if lo == hi:
            val[node] = arr[lo]  # node lá: gán giá trị phần tử
            continue
        mid = (lo + hi) >> 1
        l = cnt; cnt += 1
        r = cnt; cnt += 1
        left[node] = l
        right[node] = r
        order.append(node)
        st.append((l, lo, mid))
        st.append((r, mid + 1, hi))
    # Cộng tổng từ dưới lên: cha xuất hiện trước con trong order -> duyệt ngược.
    for node in reversed(order):
        val[node] = val[left[node]] + val[right[node]]

    roots = [0, root0]    # roots[0] không dùng, roots[1] = mảng ban đầu

    out = []
    append = out.append
    dat = data

    for _ in range(q):
        t = dat[pos]; pos += 1
        if t == b'2':
            # Truy vấn "2 k a b": tổng đoạn [a, b] trên cây roots[k] (duyệt bằng stack).
            k = int(dat[pos]); a = int(dat[pos + 1]); b = int(dat[pos + 2]); pos += 3
            res = 0
            stack = [(roots[k], 1, n)]
            sp = stack.pop
            spush = stack.append
            while stack:
                node, lo, hi = sp()
                if a <= lo and hi <= b:
                    res += val[node]  # node phủ trọn trong [a, b] -> cộng thẳng
                    continue
                mid = (lo + hi) >> 1
                if a <= mid:
                    spush((left[node], lo, mid))
                if b > mid:
                    spush((right[node], mid + 1, hi))
            append(res)
        elif t == b'1':
            # Truy vấn "1 k a x": point update theo path copying, tạo phiên bản mới.
            k = int(dat[pos]); a = int(dat[pos + 1]); x = int(dat[pos + 2]); pos += 3
            # Đi từ gốc xuống lá vị trí a, ghi nhớ đường đi (node cũ + hướng rẽ).
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
            # Tạo lá mới mang giá trị x.
            newchild = cnt; cnt += 1
            val[newchild] = x
            # Đi ngược lên: mỗi tầng tạo node MỚI, một con là node vừa tạo, con còn
            # lại DÙNG CHUNG con cũ của node trên đường đi.
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
            roots[k] = newchild  # gán lại gốc mới; các bản sao cũ không bị ảnh hưởng
        else:  # t == b'3' -> sao chép mảng k: mảng mới dùng chung gốc, tốn O(1)
            k = int(dat[pos]); pos += 1
            roots.append(roots[k])

    sys.stdout.buffer.write(("\n".join(map(str, out)) + ("\n" if out else "")).encode())


main()
