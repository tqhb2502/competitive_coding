# Subarray Sum Queries - CSES 1190
# https://cses.fi/problemset/task/1190
#
# Segment tree lặp (iterative); mỗi node lưu (tot, pre, suf, best):
#   tot  = tổng cả đoạn
#   pre  = tiền tố lớn nhất (>= 0, cho phép rỗng)
#   suf  = hậu tố lớn nhất (>= 0, cho phép rỗng)
#   best = tổng đoạn con lớn nhất trong đoạn (>= 0, cho phép rỗng)
# merge(L, R):
#   tot  = L.tot + R.tot
#   pre  = max(L.pre, L.tot + R.pre)
#   suf  = max(R.suf, R.tot + L.suf)
#   best = max(L.best, R.best, L.suf + R.pre)
# Chỉ cần đáp án của toàn mảng => đọc best[1] sau mỗi update.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    size = 1
    while size < n:
        size <<= 1

    N2 = 2 * size
    tot = [0] * N2
    pre = [0] * N2
    suf = [0] * N2
    best = [0] * N2

    # Khởi tạo lá
    for i in range(n):
        v = int(data[idx]); idx += 1
        p = size + i
        tot[p] = v
        mv = v if v > 0 else 0
        pre[p] = mv
        suf[p] = mv
        best[p] = mv

    # Xây dựng từ dưới lên
    for p in range(size - 1, 0, -1):
        l = p << 1
        r = l | 1
        tl = tot[l]; tr = tot[r]
        sl = suf[l]; prr = pre[r]
        tot[p] = tl + tr
        cand = tl + prr
        pl = pre[l]
        pre[p] = pl if pl > cand else cand
        cand2 = tr + sl
        sr = suf[r]
        suf[p] = sr if sr > cand2 else cand2
        cross = sl + prr
        b = best[l]
        br = best[r]
        if br > b:
            b = br
        if cross > b:
            b = cross
        best[p] = b

    out = []
    ap = out.append
    for _ in range(m):
        k = int(data[idx]); idx += 1
        x = int(data[idx]); idx += 1
        p = size + k - 1
        tot[p] = x
        mv = x if x > 0 else 0
        pre[p] = mv
        suf[p] = mv
        best[p] = mv
        p >>= 1
        while p:
            l = p << 1
            r = l | 1
            tl = tot[l]; tr = tot[r]
            sl = suf[l]; prr = pre[r]
            tot[p] = tl + tr
            cand = tl + prr
            pl = pre[l]
            pre[p] = pl if pl > cand else cand
            cand2 = tr + sl
            sr = suf[r]
            suf[p] = sr if sr > cand2 else cand2
            cross = sl + prr
            b = best[l]
            br = best[r]
            if br > b:
                b = br
            if cross > b:
                b = cross
            best[p] = b
            p >>= 1
        ap(best[1])

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
