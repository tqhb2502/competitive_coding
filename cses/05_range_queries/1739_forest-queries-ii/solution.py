import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    q = int(data[1])
    W = n + 1
    size = W * W

    bit = [0] * size          # Fenwick 2 chiều, chỉ số phẳng i*W + j (1-indexed)
    cur = bytearray(size)     # trạng thái 0/1 hiện tại của từng ô, để biết chiều toggle
    trans = bytes.maketrans(b'.*', bytes((0, 1)))

    # Nạp lưới ban đầu; gán slice chạy ở tốc độ C.
    for i in range(1, n + 1):
        base = i * W
        seg = data[1 + i].translate(trans)   # data[2] là hàng 1 -> data[1 + i] là hàng i
        bit[base + 1: base + 1 + n] = seg
        cur[base + 1: base + 1 + n] = seg

    # Xây Fenwick "tại chỗ" trong O(n^2): propagate theo hàng, rồi theo cột.
    for i in range(1, n + 1):
        base = i * W
        for j in range(1, n + 1):
            k = j + (j & -j)
            if k <= n:
                bit[base + k] += bit[base + j]
    for j in range(1, n + 1):
        for i in range(1, n + 1):
            k = i + (i & -i)
            if k <= n:
                bit[k * W + j] += bit[i * W + j]

    # Gán biến local để tăng tốc truy cập trong vòng lặp xử lý truy vấn.
    b = bit
    data_local = data
    L = len(data_local)
    idx = 2 + n
    out = []
    ap = out.append

    while idx < L:
        t = data_local[idx]
        if t == b'1':
            # Toggle ô (y, x): xác định delta rồi point-update trên 2D BIT, O(log^2 n).
            y = int(data_local[idx + 1])
            x = int(data_local[idx + 2])
            idx += 3
            p = y * W + x
            if cur[p]:
                cur[p] = 0
                delta = -1
            else:
                cur[p] = 1
                delta = 1
            ii = y
            while ii <= n:
                base = ii * W
                jj = x
                while jj <= n:
                    b[base + jj] += delta
                    jj += jj & -jj
                ii += ii & -ii
        else:
            # Đếm cây trong hình chữ nhật qua bao-trừ 4 góc: Q(y2,x2)-Q(y1-1,x2)-Q(y2,x1-1)+Q(y1-1,x1-1).
            y1 = int(data_local[idx + 1])
            x1 = int(data_local[idx + 2])
            y2 = int(data_local[idx + 3])
            x2 = int(data_local[idx + 4])
            idx += 5
            # Phân tích bit của cột một lần, dùng lại cho mọi hàng trong phân tích của y.
            c2 = []
            jj = x2
            while jj > 0:
                c2.append(jj)
                jj -= jj & -jj
            c1 = []
            jj = x1 - 1
            while jj > 0:
                c1.append(jj)
                jj -= jj & -jj

            res = 0
            # Cộng phần tiền tố tới hàng y2.
            ii = y2
            while ii > 0:
                base = ii * W
                s = 0
                for j in c2:
                    s += b[base + j]
                for j in c1:
                    s -= b[base + j]
                res += s
                ii -= ii & -ii
            # Trừ phần tiền tố tới hàng y1-1.
            ii = y1 - 1
            while ii > 0:
                base = ii * W
                s = 0
                for j in c2:
                    s += b[base + j]
                for j in c1:
                    s -= b[base + j]
                res -= s
                ii -= ii & -ii
            ap(res)

    if out:
        sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
