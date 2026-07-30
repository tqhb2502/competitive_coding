import sys
from bisect import bisect_left, bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1

    horizontals = []  # (x_left, x_right, y)
    verticals = []    # (x, y_low, y_high)
    ys = []           # tất cả y của đoạn ngang, dùng để nén tọa độ

    # Phân loại từng đoạn thành ngang (y1 == y2) hoặc dọc (x1 == x2).
    for _ in range(n):
        x1 = int(data[pos]); y1 = int(data[pos + 1])
        x2 = int(data[pos + 2]); y2 = int(data[pos + 3])
        pos += 4
        if y1 == y2:
            # đoạn ngang: theo ràng buộc x1 <= x2
            horizontals.append((x1, x2, y1))
            ys.append(y1)
        else:
            # đoạn dọc: x1 == x2, theo ràng buộc y1 <= y2
            verticals.append((x1, y1, y2))

    # Nén tọa độ y của các đoạn ngang thành rank.
    ys = sorted(set(ys))
    m = len(ys)

    # Tạo các sự kiện sweep line; type: add=0 < query=1 < remove=2 (thứ tự khi trùng x).
    # Nhờ bảo đảm không endpoint nào là giao điểm nên thứ tự này an toàn.
    events = []
    ap = events.append

    # Mỗi đoạn ngang -> sự kiện add tại xl và remove tại xr (rank 1-based cho BIT).
    for xl, xr, y in horizontals:
        r = bisect_left(ys, y) + 1
        ap((xl, 0, r, 0))
        ap((xr, 2, r, 0))

    # Mỗi đoạn dọc -> sự kiện query lấy khoảng rank [lo, hi] ứng với [y_low, y_high].
    for x, ylo, yhi in verticals:
        lo = bisect_left(ys, ylo)   # số phần tử < ylo
        hi = bisect_right(ys, yhi)  # số phần tử <= yhi
        if hi > lo:
            # range sum trên rank (lo+1 .. hi) = prefix(hi) - prefix(lo)
            ap((x, 1, lo, hi))

    events.sort()

    # BIT (Fenwick tree) trên trục y, kích thước m+1 (1-based).
    sz = m + 1
    tree = [0] * sz
    ans = 0

    # Quét theo x: add/remove cập nhật BIT (+1/-1), query cộng số đoạn ngang trong khoảng.
    for _x, typ, p, q in events:
        if typ == 1:
            # range sum = prefix(q) - prefix(p)
            s = 0
            i = q
            while i > 0:
                s += tree[i]
                i -= i & (-i)
            i = p
            while i > 0:
                s -= tree[i]
                i -= i & (-i)
            ans += s
        elif typ == 0:
            # point update +1: đoạn ngang bắt đầu hoạt động
            i = p
            while i < sz:
                tree[i] += 1
                i += i & (-i)
        else:  # typ == 2
            # point update -1: đoạn ngang kết thúc hoạt động
            i = p
            while i < sz:
                tree[i] -= 1
                i += i & (-i)

    sys.stdout.write(str(ans) + "\n")


main()
