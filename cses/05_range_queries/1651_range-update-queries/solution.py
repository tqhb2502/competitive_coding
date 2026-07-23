import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    a = data[idx:idx + n]
    idx += n

    # Fenwick tree (BIT) lưu mảng hiệu; prefix(k) = giá trị hiện tại của a[k].
    tree = [0] * (n + 2)

    # Cộng delta vào vị trí i của mảng hiệu.
    def update(i, delta):
        while i <= n:
            tree[i] += delta
            i += i & (-i)

    # Tổng tiền tố đến vị trí i.
    def prefix(i):
        s = 0
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        return s

    # Khởi tạo mảng hiệu: mỗi vị trí i thêm chênh lệch a[i] - a[i-1].
    prev = 0
    for i in range(1, n + 1):
        cur = int(a[i - 1])
        update(i, cur - prev)
        prev = cur

    out = []
    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'1':
            # Cập nhật đoạn [av, bv] += u bằng hai cập nhật điểm ở biên.
            av = int(data[idx]); bv = int(data[idx + 1]); u = int(data[idx + 2])
            idx += 3
            update(av, u)
            if bv + 1 <= n:
                update(bv + 1, -u)
        else:
            # Truy vấn điểm: tổng tiền tố của mảng hiệu đến vị trí k.
            k = int(data[idx]); idx += 1
            out.append(prefix(k))

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
