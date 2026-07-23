import sys
from bisect import bisect_left, bisect_right
from collections import defaultdict


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    pos = 0
    n = int(data[pos]); pos += 1

    # Dựng dãy đoạn của polyline theo thứ tự đi.
    seg_kind = []   # True = dọc (vertical), False = ngang (horizontal)
    seg_x = []      # đoạn dọc: cột x
    seg_y = []      # đoạn ngang: hàng y
    seg_lo = []     # đầu thấp theo trục biến thiên
    seg_hi = []     # đầu cao theo trục biến thiên
    seg_s = []      # toạ độ bắt đầu theo trục biến thiên
    seg_e = []      # toạ độ kết thúc theo trục biến thiên
    seg_ps = []     # quãng đường prefix đã đi trước đoạn này

    cx = cy = 0
    prefix = 0
    total = 0
    for _ in range(n):
        d = data[pos]; dist = int(data[pos + 1]); pos += 2
        total += dist
        if d == b'U' or d == b'D':
            ys = cy
            ye = cy + dist if d == b'U' else cy - dist
            seg_kind.append(True)
            seg_x.append(cx); seg_y.append(0)
            seg_lo.append(ys if ys < ye else ye)
            seg_hi.append(ye if ye > ys else ys)
            seg_s.append(ys); seg_e.append(ye)
            seg_ps.append(prefix)
            prefix += dist; cy = ye
        else:
            xs = cx
            xe = cx + dist if d == b'R' else cx - dist
            seg_kind.append(False)
            seg_x.append(0); seg_y.append(cy)
            seg_lo.append(xs if xs < xe else xe)
            seg_hi.append(xe if xe > xs else xs)
            seg_s.append(xs); seg_e.append(xe)
            seg_ps.append(prefix)
            prefix += dist; cx = xe

    INF = float('inf')
    best = INF

    # ---------- Pha B: giao vuông góc (sweep line theo trục x) ----------
    # Nén các hàng y của đoạn ngang để đặt lên Fenwick tree.
    hy_vals = sorted({seg_y[i] for i in range(n) if not seg_kind[i]})
    if hy_vals:
        M = len(hy_vals)
        ycomp = {y: k for k, y in enumerate(hy_vals)}
        tree = [0] * (M + 1)

        def fen_update(i, delta):
            while i <= M:
                tree[i] += delta
                i += i & (-i)

        def fen_prefix(i):
            s = 0
            while i > 0:
                s += tree[i]
                i -= i & (-i)
            return s

        LOG = M.bit_length()

        def fen_findrank(k):
            # Chỉ số nhỏ nhất có prefix sum >= k (mỗi hàng chỉ mang 0/1) - binary lifting.
            posi = 0
            rem = k
            pw = 1 << LOG
            while pw:
                nxt = posi + pw
                if nxt <= M and tree[nxt] < rem:
                    posi = nxt
                    rem -= tree[nxt]
                pw >>= 1
            return posi + 1

        buckets = [None] * M

        # Sự kiện sắp theo (x, loại): thêm(0) trước truy vấn(1) trước xoá(2).
        events = []
        ap = events.append
        for i in range(n):
            if seg_kind[i]:
                ap((seg_x[i], 1, i))
            else:
                ap((seg_lo[i], 0, i))
                ap((seg_hi[i], 2, i))
        events.sort(key=lambda e: (e[0], e[1]))

        for ex, typ, i in events:
            if typ == 0:
                # Thêm đoạn ngang vào tập active (đánh dấu hàng y của nó).
                c = ycomp[seg_y[i]]
                b = buckets[c]
                if b is None:
                    buckets[c] = [i]
                    fen_update(c + 1, 1)
                else:
                    b.append(i)
            elif typ == 2:
                # Xoá đoạn ngang khỏi tập active.
                c = ycomp[seg_y[i]]
                b = buckets[c]
                b.remove(i)
                if not b:
                    buckets[c] = None
                    fen_update(c + 1, -1)
            else:
                # Đoạn dọc: liệt kê các hàng active nằm trong [ylo, yhi] - đó là giao điểm.
                vlo = seg_lo[i]; vhi = seg_hi[i]
                clo = bisect_left(hy_vals, vlo)
                chi = bisect_right(hy_vals, vhi) - 1
                if clo > chi:
                    continue
                base = fen_prefix(clo)
                cnt = fen_prefix(chi + 1) - base
                if cnt <= 0:
                    continue
                vx = seg_x[i]; vsy = seg_s[i]; vps = seg_ps[i]
                for r in range(1, cnt + 1):
                    p = fen_findrank(base + r)
                    for hidx in buckets[p - 1]:
                        # Đoạn "đến sau" (chỉ số lớn hơn) là đoạn robot dùng để quay lại.
                        if hidx > i:
                            t = vx - seg_s[hidx]
                            if t < 0:
                                t = -t
                            if t > 0:   # t == 0 là góc cua, bỏ qua
                                r2 = seg_ps[hidx] + t
                                if r2 < best:
                                    best = r2
                        else:
                            t = seg_y[hidx] - vsy
                            if t < 0:
                                t = -t
                            if t > 0:   # t == 0 là góc cua, bỏ qua
                                r2 = vps + t
                                if r2 < best:
                                    best = r2

    # ---------- Pha C: chồng lấn collinear (các đoạn cùng một đường) -------
    vlines = defaultdict(list)
    hlines = defaultdict(list)
    for i in range(n):
        if seg_kind[i]:
            vlines[seg_x[i]].append(i)
        else:
            hlines[seg_y[i]].append(i)

    def process_line(idxs):
        nonlocal best
        cov = []      # các khoảng rời nhau [a, b] đã sắp
        starts = []   # danh sách song song các giá trị a
        for i in idxs:
            s = seg_s[i]; e = seg_e[i]
            lo = seg_lo[i]; hi = seg_hi[i]; ps = seg_ps[i]
            cand = None
            if e > s:                          # đi tăng, vùng cần xét (s, e]
                p = bisect_right(starts, s) - 1
                if p >= 0 and cov[p][1] > s:
                    cand = 0
                else:
                    q = bisect_right(starts, s)
                    if q < len(cov) and cov[q][0] <= e:
                        cand = cov[q][0] - s
            else:                              # đi giảm, vùng cần xét [e, s)
                p = bisect_left(starts, s) - 1
                if p >= 0:
                    b = cov[p][1]
                    if b >= s:
                        cand = 0
                    elif b >= e:
                        cand = s - b
            if cand is not None:
                r = ps + cand
                if r < best:
                    best = r
            # Gộp [lo, hi] vào hợp các khoảng đã phủ.
            L = bisect_left(starts, lo)
            if L > 0 and cov[L - 1][1] >= lo:
                L -= 1
            newlo = lo; newhi = hi
            R = L; m = len(cov)
            while R < m and cov[R][0] <= newhi:
                if cov[R][0] < newlo:
                    newlo = cov[R][0]
                if cov[R][1] > newhi:
                    newhi = cov[R][1]
                R += 1
            cov[L:R] = [[newlo, newhi]]
            starts[L:R] = [newlo]

    for idxs in vlines.values():
        process_line(idxs)
    for idxs in hlines.values():
        process_line(idxs)

    # Đáp án là reach-distance nhỏ nhất, hoặc tổng độ dài nếu không có giao nào.
    ans = best if best != INF else total
    sys.stdout.write(str(int(ans)) + "\n")


main()
