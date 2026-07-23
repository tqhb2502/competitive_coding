import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Kích thước cây làm tròn lên lũy thừa của 2.
    size = 1
    while size < n:
        size <<= 1

    # Mỗi node lưu 4 mảng song song: tot (tổng), pre (tiền tố), suf (hậu tố),
    # best (đoạn con lớn nhất); các đại lượng dương đều kẹp >= 0 (cho phép đoạn rỗng).
    N2 = 2 * size
    tot = [0] * N2
    pre = [0] * N2
    suf = [0] * N2
    best = [0] * N2

    # Khởi tạo lá từ giá trị đầu vào.
    for i in range(n):
        v = int(data[idx]); idx += 1
        p = size + i
        tot[p] = v
        mv = v if v > 0 else 0
        pre[p] = mv
        suf[p] = mv
        best[p] = mv

    # Dựng cây từ dưới lên; merge được inline để giảm hằng số.
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
        # best xét thêm đoạn vắt qua ranh giới: hậu tố của L nối tiền tố của R.
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
        # Point update: ghi giá trị mới vào lá.
        p = size + k - 1
        tot[p] = x
        mv = x if x > 0 else 0
        pre[p] = mv
        suf[p] = mv
        best[p] = mv
        # Đi ngược lên gốc, cập nhật lại các node cha bằng merge inline.
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
        # Đáp án của toàn mảng nằm ở node gốc best[1].
        ap(best[1])

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
