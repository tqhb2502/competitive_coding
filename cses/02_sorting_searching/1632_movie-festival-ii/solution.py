import sys
from bisect import bisect_right, bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); k = int(data[pos + 1]); pos += 2

    a_list = [0] * n
    b_list = [0] * n
    coords = [0]  # Giá trị 0 là thời điểm rảnh ban đầu của mọi thành viên.
    for i in range(n):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        a_list[i] = a
        b_list[i] = b
        coords.append(b)

    # Duyệt các phim theo thời gian kết thúc b tăng dần.
    order = sorted(range(n), key=lambda i: b_list[i])

    # Nén tọa độ trên toàn bộ giá trị có thể có trong multiset ({0} hợp {b_i}).
    V = sorted(set(coords))
    m = len(V)

    tree = [0] * (m + 1)

    # Khởi tạo Fenwick: k thành viên đều ở giá trị 0, tức V[0] -> chỉ số Fenwick 1.
    i = 1
    while i <= m:
        tree[i] += k
        i += i & (-i)

    # Lũy thừa 2 lớn nhất với (1 << LOG) <= m, dùng cho binary lifting find-k-th.
    LOG = 0
    while (1 << (LOG + 1)) <= m:
        LOG += 1

    count = 0
    for oi in order:
        a = a_list[oi]
        b = b_list[oi]

        # p = số giá trị nén <= a; chúng nằm ở các chỉ số Fenwick 1..p.
        p = bisect_right(V, a)

        # s = số thành viên hiện có end <= a (số slot bị chiếm trong đoạn 1..p).
        s = 0
        i = p
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        if s == 0:
            continue  # Không có thành viên rảnh nào với end <= a.

        # find_kth(s): chỉ số nhỏ nhất có prefix sum >= s. Vì có đúng s slot bị
        # chiếm trong [1..p], đây là chỉ số LỚN NHẤT <= p bị chiếm, tức thành viên
        # có end lớn nhất mà vẫn <= a.
        idx = 0
        rem = s
        pw = LOG
        while pw >= 0:
            nxt = idx + (1 << pw)
            if nxt <= m and tree[nxt] < rem:
                idx = nxt
                rem -= tree[nxt]
            pw -= 1
        idx += 1

        # Xóa thời điểm rảnh cũ của thành viên đó khỏi multiset.
        i = idx
        while i <= m:
            tree[i] -= 1
            i += i & (-i)

        # Chèn thời điểm rảnh mới b cho thành viên đó.
        bidx = bisect_left(V, b) + 1
        i = bidx
        while i <= m:
            tree[i] += 1
            i += i & (-i)

        count += 1

    sys.stdout.write(str(count) + "\n")


main()
