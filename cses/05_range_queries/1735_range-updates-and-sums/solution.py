# Range Updates and Sums - CSES 1735
# https://cses.fi/problemset/task/1735
#
# Ba loại truy vấn trên một mảng:
#   1 a b x : cộng x vào mọi phần tử trong [a, b]   (range add)
#   2 a b x : gán x cho mọi phần tử trong [a, b]     (range assign)
#   3 a b   : in tổng các phần tử trong [a, b]        (range sum)
#
# Dùng lazy-propagation segment tree (kiểu iterative / bottom-up của AtCoder)
# với HAI thẻ lazy lồng nhau: lz_a (assign) và lz_d (add).
# Ý nghĩa lazy của một node: nếu lz_a != NONE thì trước hết GÁN lz_a, sau đó CỘNG lz_d.
#   - assign x  -> lazy = (x, 0)  (ghi đè mọi thứ bên dưới)
#   - add x     -> lz_d += x      (giữ nguyên lz_a nếu có)
# Khi push xuống con: con nhận giá trị (lz_a + lz_d) nếu có assign, ngược lại chỉ cộng lz_d.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    q = int(data[1])
    pos = 2

    size = 1
    while size < n:
        size <<= 1
    log = size.bit_length() - 1
    NONE = -1  # giá trị x >= 1 nên -1 an toàn làm sentinel "không có assign"

    seg = [0] * (2 * size)       # tổng của đoạn
    lz_a = [NONE] * (2 * size)   # lazy assign
    lz_d = [0] * (2 * size)      # lazy add
    slen = [1] * (2 * size)      # số lá (độ dài) mà node phủ

    for i in range(n):
        seg[size + i] = int(data[pos + i])
    pos += n
    for i in range(size - 1, 0, -1):
        i2 = i + i
        seg[i] = seg[i2] + seg[i2 + 1]
        slen[i] = slen[i2] + slen[i2 + 1]

    def push(p):
        # Đẩy lazy của p xuống hai con rồi xóa lazy của p.
        a = lz_a[p]
        c = p + p
        if a != NONE:
            s = a + lz_d[p]  # gán a rồi cộng lz_d == gán (a + lz_d)
            seg[c] = s * slen[c]
            seg[c + 1] = s * slen[c + 1]
            if c < size:  # con là node trong -> gán lazy cho chúng
                lz_a[c] = s
                lz_d[c] = 0
                lz_a[c + 1] = s
                lz_d[c + 1] = 0
            lz_a[p] = NONE
            lz_d[p] = 0
        else:
            d = lz_d[p]
            if d:
                seg[c] += d * slen[c]
                seg[c + 1] += d * slen[c + 1]
                if c < size:
                    lz_d[c] += d
                    lz_d[c + 1] += d
                lz_d[p] = 0

    out = []
    ap = out.append

    for _ in range(q):
        t = data[pos]
        if t == b'3':
            l = int(data[pos + 1]) - 1 + size
            r = int(data[pos + 2]) + size
            pos += 3
            # push dọc theo biên của truy vấn
            for i in range(log, 0, -1):
                if ((l >> i) << i) != l:
                    push(l >> i)
                if ((r >> i) << i) != r:
                    push((r - 1) >> i)
            res = 0
            ll = l
            rr = r
            while ll < rr:
                if ll & 1:
                    res += seg[ll]
                    ll += 1
                if rr & 1:
                    rr -= 1
                    res += seg[rr]
                ll >>= 1
                rr >>= 1
            ap(res)
        else:
            l = int(data[pos + 1]) - 1 + size
            r = int(data[pos + 2]) + size
            x = int(data[pos + 3])
            pos += 4
            # push dọc theo biên
            for i in range(log, 0, -1):
                if ((l >> i) << i) != l:
                    push(l >> i)
                if ((r >> i) << i) != r:
                    push((r - 1) >> i)
            ll = l
            rr = r
            if t == b'2':  # gán x (assign)
                while ll < rr:
                    if ll & 1:
                        seg[ll] = x * slen[ll]
                        if ll < size:
                            lz_a[ll] = x
                            lz_d[ll] = 0
                        ll += 1
                    if rr & 1:
                        rr -= 1
                        seg[rr] = x * slen[rr]
                        if rr < size:
                            lz_a[rr] = x
                            lz_d[rr] = 0
                    ll >>= 1
                    rr >>= 1
            else:  # cộng x (add), t == b'1'
                while ll < rr:
                    if ll & 1:
                        seg[ll] += x * slen[ll]
                        if ll < size:
                            lz_d[ll] += x
                        ll += 1
                    if rr & 1:
                        rr -= 1
                        seg[rr] += x * slen[rr]
                        if rr < size:
                            lz_d[rr] += x
                    ll >>= 1
                    rr >>= 1
            # pull up: cập nhật tổng cho các tổ tiên trên biên (lazy đã được xóa ở bước push)
            for i in range(1, log + 1):
                if ((l >> i) << i) != l:
                    p = l >> i
                    c = p + p
                    seg[p] = seg[c] + seg[c + 1]
                if ((r >> i) << i) != r:
                    p = (r - 1) >> i
                    c = p + p
                    seg[p] = seg[c] + seg[c + 1]

    if out:
        sys.stdout.buffer.write(b'\n'.join(str(v).encode() for v in out))
        sys.stdout.buffer.write(b'\n')


main()
