import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # Làm tròn kích thước lên lũy thừa của 2 cho segment tree lặp.
    size = 1
    while size < n:
        size <<= 1

    # Mỗi node lưu song song 2 mảng: tot = tổng đoạn, bst = maximum prefix sum
    # (đã tính cả prefix rỗng nên bst >= 0).
    tot = [0] * (2 * size)
    bst = [0] * (2 * size)

    # Nạp giá trị vào lá: lá (một phần tử v) có tot = v, bst = max(0, v).
    for i in range(n):
        v = int(data[idx]); idx += 1
        tot[size + i] = v
        bst[size + i] = v if v > 0 else 0

    # Build bottom-up: mỗi node trong = merge của hai con (inline phép merge).
    for i in range(size - 1, 0, -1):
        li = i + i
        tl = tot[li]
        tot[i] = tl + tot[li + 1]
        b = bst[li]
        b2 = tl + bst[li + 1]
        bst[i] = b if b > b2 else b2

    out = []
    ap = out.append

    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'1':
            # Point update: gán lại lá rồi đi ngược lên gốc cập nhật tổ tiên.
            k = int(data[idx]); idx += 1
            u = int(data[idx]); idx += 1
            p = size + k - 1
            tot[p] = u
            bst[p] = u if u > 0 else 0
            p >>= 1
            while p:
                li = p + p
                tl = tot[li]
                tot[p] = tl + tot[li + 1]
                b = bst[li]
                b2 = tl + bst[li + 1]
                bst[p] = b if b > b2 else b2
                p >>= 1
        else:
            # Truy vấn đoạn [a, b]: vì merge KHÔNG giao hoán nên gom biên trái vào
            # accumulator (tL, bL) theo thứ tự trái->phải, gom biên phải vào
            # accumulator (tR, bR) bằng cách prepend khi r giảm.
            a = int(data[idx]); idx += 1
            bb = int(data[idx]); idx += 1
            l = a - 1 + size
            r = bb + size  # (bb - 1) + size + 1
            tL = 0; bL = 0   # accumulator trái
            tR = 0; bR = 0   # accumulator phải
            while l < r:
                if l & 1:
                    # resL = merge(resL, node[l])
                    nb = tL + bst[l]
                    if bL > nb:
                        nb = bL
                    tL += tot[l]
                    bL = nb
                    l += 1
                if r & 1:
                    r -= 1
                    # resR = merge(node[r], resR)
                    nb = tot[r] + bR
                    if bst[r] > nb:
                        nb = bst[r]
                    tR = tot[r] + tR
                    bR = nb
                l >>= 1
                r >>= 1
            # merge(resL, resR) cuối cùng; cho prefix rỗng nên đáp án >= 0.
            ans = tL + bR
            if bL > ans:
                ans = bL
            ap(ans)

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + ('\n' if out else '')).encode())


main()
