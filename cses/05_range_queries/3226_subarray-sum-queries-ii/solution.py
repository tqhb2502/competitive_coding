import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    # Segment tree bottom-up với size = n. Mỗi node lưu 4 mảng song song:
    # SUM (tổng đoạn), PRE (max prefix), SUF (max suffix), BEST (best subarray).
    size = n
    SUM = [0] * (2 * size)
    PRE = [0] * (2 * size)
    SUF = [0] * (2 * size)
    BEST = [0] * (2 * size)

    # Khởi tạo lá: với phần tử x, đặt p = max(x, 0) rồi gán PRE=SUF=BEST=p.
    for i in range(n):
        x = int(data[pos]); pos += 1
        p = x if x > 0 else 0
        j = size + i
        SUM[j] = x
        PRE[j] = p
        SUF[j] = p
        BEST[j] = p

    # Xây các node trong bằng cách merge hai con (trái = 2*i, phải = 2*i+1).
    for i in range(size - 1, 0, -1):
        l = 2 * i
        r = l + 1
        ls = SUM[l]; rs = SUM[r]
        lp = PRE[l]; rp = PRE[r]
        lsuf = SUF[l]; rsuf = SUF[r]
        lb = BEST[l]; rb = BEST[r]
        SUM[i] = ls + rs
        t = ls + rp
        PRE[i] = lp if lp > t else t
        t = rs + lsuf
        SUF[i] = rsuf if rsuf > t else t
        cross = lsuf + rp
        m = lb if lb > rb else rb
        BEST[i] = m if m > cross else cross

    # Bind cục bộ cho vòng lặp nóng (tăng tốc truy cập).
    sum_ = SUM; pre_ = PRE; suf_ = SUF; best_ = BEST

    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        l = size + a - 1
        r = size + b          # biên phải nửa mở
        # Merge không giao hoán: bộ tích lũy trái gộp các mảnh từ trái sang, bộ
        # tích lũy phải gộp các mảnh từ phải sang. Identity = (0, 0, 0, 0).
        Lsum = Lpre = Lsuf = Lbest = 0
        Rsum = Rpre = Rsuf = Rbest = 0
        while l < r:
            if l & 1:
                # merge(L_acc, node) — node đứng bên phải bộ tích lũy trái.
                nsum = sum_[l]; npre = pre_[l]; nsuf = suf_[l]; nbest = best_[l]
                t = Lsum + npre
                cpre = Lpre if Lpre > t else t
                t = nsum + Lsuf
                csuf = nsuf if nsuf > t else t
                cross = Lsuf + npre
                m = Lbest if Lbest > nbest else nbest
                Lbest = m if m > cross else cross
                Lsum = Lsum + nsum
                Lpre = cpre
                Lsuf = csuf
                l += 1
            if r & 1:
                r -= 1
                # merge(node, R_acc) — node đứng bên trái bộ tích lũy phải.
                nsum = sum_[r]; npre = pre_[r]; nsuf = suf_[r]; nbest = best_[r]
                t = nsum + Rpre
                cpre = npre if npre > t else t
                t = Rsum + nsuf
                csuf = Rsuf if Rsuf > t else t
                cross = nsuf + Rpre
                m = nbest if nbest > Rbest else Rbest
                Rbest = m if m > cross else cross
                Rsum = nsum + Rsum
                Rpre = cpre
                Rsuf = csuf
            l >>= 1
            r >>= 1
        # Gộp cuối cùng merge(L_acc, R_acc), chỉ cần trường best làm đáp án.
        cross = Lsuf + Rpre
        m = Lbest if Lbest > Rbest else Rbest
        ap(m if m > cross else cross)

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
