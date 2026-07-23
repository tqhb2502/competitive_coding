import sys


def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    q = int(next(it))

    # Đọc mảng x và tính prefix sum prefX.
    x = [0] * (n + 1)
    prefX = [0] * (n + 1)
    for i in range(1, n + 1):
        v = int(next(it))
        x[i] = v
        prefX[i] = prefX[i - 1] + v

    # Offline: nhóm các truy vấn theo điểm trái a.
    qa = [[] for _ in range(n + 1)]
    for k in range(q):
        a = int(next(it))
        b = int(next(it))
        qa[a].append((b, k))

    ans = [0] * q

    # Kỹ thuật 2 BIT cho range update + range sum: prefix(i) = s1*i - s2.
    N = n
    b1 = [0] * (n + 2)   # BIT hệ số
    b2 = [0] * (n + 2)   # BIT phần bù
    # Monotonic stack các đoạn hằng của g[i] = max(x[L..i]).
    st_val = []          # giá trị các đoạn
    st_cnt = []          # số vị trí mỗi đoạn (đỉnh = đoạn trái nhất, bắt đầu tại L+1)

    # Duyệt điểm trái L từ n về 1, thêm dần x[L] vào bên trái.
    for L in range(n, 0, -1):
        v = x[L]
        pos = L + 1
        acc = 0
        # Pop các đoạn trái nhất có value <= v và nâng chúng lên thành v.
        while st_val and st_val[-1] <= v:
            sv = st_val.pop()
            sc = st_cnt.pop()
            r = pos + sc - 1          # dải vị trí của đoạn bị pop: [pos, r]
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
        # Thêm vị trí L với giá trị v: range_add(L, L, v).
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
        # Đẩy đoạn mới (v, số vị trí vừa gộp + 1) vào stack.
        st_val.append(v)
        st_cnt.append(acc + 1)

        # Trả lời các truy vấn có điểm trái đúng bằng L.
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
                # Đáp án = tổng running max trên [L, bq] trừ tổng x trên [L, bq].
                ans[k] = (qb - qL) - (prefX[bq] - pxL1)

    out = "\n".join(map(str, ans))
    sys.stdout.buffer.write(out.encode())
    sys.stdout.buffer.write(b"\n")


main()
