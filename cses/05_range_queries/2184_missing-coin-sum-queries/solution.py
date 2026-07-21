# Missing Coin Sum Queries - https://cses.fi/problemset/task/2184
#
# Với mỗi truy vấn [a, b] ta phải tìm tổng dương nhỏ nhất KHÔNG thể tạo ra từ
# tập con các đồng xu ở vị trí a..b.
#
# Thuật toán greedy kinh điển cho một mảng tĩnh: sắp xếp giá trị tăng dần, giữ
# R = "reach" (mọi tổng trong [0, R] đều tạo được). Duyệt các giá trị tăng dần;
# nếu giá trị hiện tại v > R + 1 thì đáp án là R + 1; ngược lại R += (tổng tiền
# của tất cả đồng xu có giá trị v). Kết quả cuối cùng là R + 1.
#
# Để trả lời trên đoạn [a, b] bất kỳ ta dùng persistent segment tree trên trục
# GIÁ TRỊ (đã nén), version i = đã chèn x_1..x_i (prefix theo vị trí). Với truy
# vấn (a, b) ta có hai root: root[b] và root[a-1]; tổng tiền của các đồng xu có
# giá trị thuộc một khoảng = SUM[nb] - SUM[na]. Ta chạy greedy bằng MỘT lần DFS
# in-order trên cây: node phủ giá trị [lo, hi]:
#   - nếu tổng = 0: bỏ qua
#   - nếu giá trị nhỏ nhất của node > R + 1: dừng (đáp án = R + 1)
#   - nếu giá trị lớn nhất của node <= R + 1: cộng hết (R += tổng), tỉa cả cây con
#   - ngược lại: đệ quy trái rồi phải (in-order đảm bảo R đã cập nhật)
# Vì R tăng dần và DFS duyệt theo thứ tự giá trị tăng dần nên khi gặp node có
# min > R+1 ta chắc chắn mọi giá trị còn lại đều lớn hơn -> dừng ngay.

import sys
from array import array


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    x = data[idx:idx + n]
    idx += n
    x = [int(v) for v in x]

    # nén tọa độ theo giá trị
    vals = sorted(set(x))
    m = len(vals)
    comp = {v: i for i, v in enumerate(vals)}

    # persistent segment tree trên [0, m-1]
    maxn = n * (m.bit_length() + 2) + 10
    LC = array('i', bytes(4 * maxn))
    RC = array('i', bytes(4 * maxn))
    SUM = array('q', bytes(8 * maxn))  # tổng TIỀN (có thể tới ~2e14)

    cnt = 1  # node 0 là null (SUM=0, LC=RC=0)

    roots = [0] * (n + 1)  # roots[0] = 0 (cây rỗng)

    m1 = m - 1
    for i in range(1, n + 1):
        val = x[i - 1]
        pos = comp[val]
        prev = roots[i - 1]
        new_root = cnt; cnt += 1
        cur_new = new_root
        cur_old = prev
        lo = 0; hi = m1
        while lo < hi:
            SUM[cur_new] = SUM[cur_old] + val
            mid = (lo + hi) >> 1
            if pos <= mid:
                RC[cur_new] = RC[cur_old]
                child = cnt; cnt += 1
                LC[cur_new] = child
                cur_old = LC[cur_old]
                cur_new = child
                hi = mid
            else:
                LC[cur_new] = LC[cur_old]
                child = cnt; cnt += 1
                RC[cur_new] = child
                cur_old = RC[cur_old]
                cur_new = child
                lo = mid + 1
        SUM[cur_new] = SUM[cur_old] + val  # leaf
        roots[i] = new_root

    # xử lý truy vấn
    out = []
    ap = out.append
    _SUM = SUM; _LC = LC; _RC = RC; _vals = vals; _roots = roots
    for _ in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        na0 = _roots[a - 1]
        nb0 = _roots[b]
        R = 0
        stack = [(na0, nb0, 0, m1)]
        while stack:
            na, nb, lo, hi = stack.pop()
            s = _SUM[nb] - _SUM[na]
            if s == 0:
                continue
            if _vals[lo] > R + 1:
                break  # giá trị nhỏ nhất còn lại > R+1 -> dừng
            if _vals[hi] <= R + 1:
                R += s
                continue
            mid = (lo + hi) >> 1
            # đẩy phải trước, trái sau -> pop trái trước (in-order)
            stack.append((_RC[na], _RC[nb], mid + 1, hi))
            stack.append((_LC[na], _LC[nb], lo, mid))
        ap(R + 1)

    sys.stdout.buffer.write(("\n".join(map(str, out)) + "\n").encode())


main()
