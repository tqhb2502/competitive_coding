# Salary Queries - CSES 1144
# https://cses.fi/problemset/task/1144
#
# Fenwick/BIT trên chỉ số đã coordinate compression (offline).
# - cnt[v] = số nhân viên có lương bằng v; "? a b" = tổng cnt trên [a, b];
#   "! k x" = point-update: giảm 1 ở lương cũ, tăng 1 ở lương mới.
# - Nén mọi lương có thể xuất hiện: p_i ban đầu + mọi x của truy vấn "!".
# - Truy vấn range-count qua hiệu hai prefix-sum bằng bisect.

import sys
from bisect import bisect_left, bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    p = [0] * n
    for i in range(n):
        p[i] = int(data[idx]); idx += 1

    # Đọc offline toàn bộ truy vấn, gom mọi giá trị lương có thể xuất hiện.
    qtype = [0] * q     # 1 = update "!", 0 = query "?"
    qa = [0] * q
    qb = [0] * q
    all_vals = list(p)
    for j in range(q):
        t = data[idx]; idx += 1
        aa = int(data[idx]); idx += 1
        bb = int(data[idx]); idx += 1
        qa[j] = aa
        qb[j] = bb
        if t == b'!':
            qtype[j] = 1        # aa = k, bb = x (lương mới)
            all_vals.append(bb)
        else:
            qtype[j] = 0        # aa = a, bb = b (khoảng truy vấn)

    vals = sorted(set(all_vals))
    m = len(vals)

    tree = [0] * (m + 1)

    # cur[k] = vị trí nén (1-indexed) của lương hiện tại của nhân viên k.
    cur = [0] * (n + 1)

    # Khởi tạo: cộng 1 vào vị trí nén của lương ban đầu.
    for i in range(n):
        pos = bisect_left(vals, p[i]) + 1
        cur[i + 1] = pos
        # update(pos, +1)
        j = pos
        while j <= m:
            tree[j] += 1
            j += j & (-j)

    out = []
    ap = out.append
    for jq in range(q):
        a = qa[jq]
        b = qb[jq]
        if qtype[jq] == 1:
            k = a
            x = b
            # update(cur[k], -1)
            j = cur[k]
            while j <= m:
                tree[j] -= 1
                j += j & (-j)
            pos = bisect_left(vals, x) + 1
            cur[k] = pos
            # update(pos, +1)
            j = pos
            while j <= m:
                tree[j] += 1
                j += j & (-j)
        else:
            # đáp án = prefix(hi) - prefix(lo)
            hi = bisect_right(vals, b)   # số lương <= b
            lo = bisect_left(vals, a)    # số lương < a
            s = 0
            j = hi
            while j > 0:
                s += tree[j]
                j -= j & (-j)
            j = lo
            while j > 0:
                s -= tree[j]
                j -= j & (-j)
            ap(s)

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + ('\n' if out else '')).encode())


main()
