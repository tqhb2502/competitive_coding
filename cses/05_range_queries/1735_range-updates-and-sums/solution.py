import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    q = int(data[1])
    pos = 2

    # Kích thước cây là lũy thừa 2 nhỏ nhất >= n (segment tree iterative kiểu AtCoder)
    size = 1
    while size < n:
        size <<= 1
    log = size.bit_length() - 1
    NONE = -1  # giá trị x >= 1 nên -1 an toàn làm sentinel "không có assign"

    seg = [0] * (2 * size)       # tổng của đoạn
    lz_a = [NONE] * (2 * size)   # lazy assign (giá trị gán đang chờ)
    lz_d = [0] * (2 * size)      # lazy add (giá trị cộng đang chờ)
    slen = [1] * (2 * size)      # số lá (độ dài) mà node phủ

    # Đặt giá trị ban đầu vào lá rồi build tổng và độ dài từ dưới lên
    for i in range(n):
        seg[size + i] = int(data[pos + i])
    pos += n
    for i in range(size - 1, 0, -1):
        i2 = i + i
        seg[i] = seg[i2] + seg[i2 + 1]
        slen[i] = slen[i2] + slen[i2 + 1]

    def push(p):
        # Đẩy lazy của p xuống hai con rồi xóa lazy của p
        a = lz_a[p]
        c = p + p
        if a != NONE:
            # Có assign: "gán a rồi cộng lz_d" tương đương "gán (a + lz_d)"
            s = a + lz_d[p]
            seg[c] = s * slen[c]
            seg[c + 1] = s * slen[c + 1]
            if c < size:  # con là node trong -> ghi lazy assign cho chúng
                lz_a[c] = s
                lz_d[c] = 0
                lz_a[c + 1] = s
                lz_d[c + 1] = 0
            lz_a[p] = NONE
            lz_d[p] = 0
        else:
            # Chỉ có add: cộng lz_d xuống hai con
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
        if t == b'3':  # truy vấn tổng đoạn [a, b]
            l = int(data[pos + 1]) - 1 + size
            r = int(data[pos + 2]) + size
            pos += 3
            # push lazy dọc theo hai biên trái/phải từ trên xuống
            for i in range(log, 0, -1):
                if ((l >> i) << i) != l:
                    push(l >> i)
                if ((r >> i) << i) != r:
                    push((r - 1) >> i)
            # cộng seg của các node phủ trọn đoạn nửa mở [l, r)
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
        else:  # cập nhật đoạn: t == b'1' (add) hoặc t == b'2' (assign)
            l = int(data[pos + 1]) - 1 + size
            r = int(data[pos + 2]) + size
            x = int(data[pos + 3])
            pos += 4
            # push lazy dọc theo biên trước khi chạm các node bên trong
            for i in range(log, 0, -1):
                if ((l >> i) << i) != l:
                    push(l >> i)
                if ((r >> i) << i) != r:
                    push((r - 1) >> i)
            ll = l
            rr = r
            if t == b'2':  # gán x (assign): ghi đè seg và lz_a, xóa lz_d
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
            else:  # cộng x (add): tăng seg và lz_d, giữ nguyên lz_a
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
            # pull up: cập nhật lại tổng cho các tổ tiên trên biên (lazy đã xóa khi push)
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
