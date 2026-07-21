# Increasing Array Queries - https://cses.fi/problemset/task/2416
#
# ans(a,b) = sum_{i=a}^{b} max(x[a..i]) - (prefX[b]-prefX[a-1]).
# Offline: nhóm truy vấn theo a, duyệt L=n..1, monotonic stack các đoạn
# (value,count) mô tả g[i]=max(x[L..i]), lưu g trong Fenwick/BIT range-update
# range-sum (kỹ thuật 2 BIT). Độ phức tạp O((n+q) log n).
# LƯU Ý TLE: pure CPython với 2e5*log có thể TLE trên CSES (giới hạn cho C++).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    q = int(next(it))

    x = [0] * (n + 1)
    prefX = [0] * (n + 1)
    for i in range(1, n + 1):
        v = int(next(it))
        x[i] = v
        prefX[i] = prefX[i - 1] + v

    # nhóm truy vấn theo điểm trái a
    qa = [[] for _ in range(n + 1)]
    for k in range(q):
        a = int(next(it))
        b = int(next(it))
        qa[a].append((b, k))

    ans = [0] * q

    N = n
    b1 = [0] * (n + 2)   # BIT hệ số
    b2 = [0] * (n + 2)   # BIT phần bù
    st_val = []          # giá trị các đoạn
    st_cnt = []          # số vị trí các đoạn (đỉnh = đoạn trái nhất, bắt đầu tại L+1)

    for L in range(n, 0, -1):
        v = x[L]
        pos = L + 1
        acc = 0
        # pop các đoạn trái nhất có value <= v, gộp thành giá trị v
        while st_val and st_val[-1] <= v:
            sv = st_val.pop()
            sc = st_cnt.pop()
            r = pos + sc - 1          # đoạn phụ [pos, r]
            d = v - sv
            if d:
                # range_add(pos, r, d): _upd(pos, d), _upd(r+1, -d)
                i = pos
                vi = d * (pos - 1)
                while i <= N:
                    b1[i] += d
                    b2[i] += vi
                    i += i & (-i)
                i = r + 1
                nd = -d
                vi = nd * r
                while i <= N:
                    b1[i] += nd
                    b2[i] += vi
                    i += i & (-i)
            pos += sc
            acc += sc
        # thêm vị trí L với giá trị v: range_add(L, L, v)
        i = L
        vi = v * (L - 1)
        while i <= N:
            b1[i] += v
            b2[i] += vi
            i += i & (-i)
        i = L + 1
        nv = -v
        vi2 = nv * L
        while i <= N:
            b1[i] += nv
            b2[i] += vi2
            i += i & (-i)
        st_val.append(v)
        st_cnt.append(acc + 1)

        bucket = qa[L]
        if bucket:
            pxL1 = prefX[L - 1]
            Lm1 = L - 1
            for bq, k in bucket:
                # prefix(bq) = s1*bq - s2
                s1 = 0
                s2 = 0
                j = bq
                while j > 0:
                    s1 += b1[j]
                    s2 += b2[j]
                    j -= j & (-j)
                qb = s1 * bq - s2
                # prefix(L-1)
                s1 = 0
                s2 = 0
                j = Lm1
                while j > 0:
                    s1 += b1[j]
                    s2 += b2[j]
                    j -= j & (-j)
                qL = s1 * Lm1 - s2
                ans[k] = (qb - qL) - (prefX[bq] - pxL1)

    out = "\n".join(map(str, ans))
    sys.stdout.buffer.write(out.encode())
    sys.stdout.buffer.write(b"\n")


main()
