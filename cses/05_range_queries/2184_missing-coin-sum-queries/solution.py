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

    # Nén tọa độ theo giá trị đồng xu.
    vals = sorted(set(x))
    m = len(vals)
    comp = {v: i for i, v in enumerate(vals)}

    # Persistent segment tree trên trục giá trị [0, m-1], lưu bằng mảng phẳng.
    # SUM giữ tổng TIỀN của cây con (có thể tới ~2e14 nên dùng int64 'q').
    maxn = n * (m.bit_length() + 2) + 10
    LC = array('i', bytes(4 * maxn))
    RC = array('i', bytes(4 * maxn))
    SUM = array('q', bytes(8 * maxn))

    cnt = 1  # node 0 là null (SUM=0, LC=RC=0)

    # roots[i] = version sau khi chèn i đồng đầu tiên; roots[0] = cây rỗng.
    roots = [0] * (n + 1)

    m1 = m - 1
    for i in range(1, n + 1):
        val = x[i - 1]
        pos = comp[val]
        prev = roots[i - 1]
        # Sinh một root mới, chỉ nhân bản các node trên đường đi tới lá pos.
        new_root = cnt; cnt += 1
        cur_new = new_root
        cur_old = prev
        lo = 0; hi = m1
        while lo < hi:
            SUM[cur_new] = SUM[cur_old] + val  # cộng tiền lên đường đi
            mid = (lo + hi) >> 1
            if pos <= mid:
                RC[cur_new] = RC[cur_old]  # nhánh phải dùng chung với version cũ
                child = cnt; cnt += 1
                LC[cur_new] = child
                cur_old = LC[cur_old]
                cur_new = child
                hi = mid
            else:
                LC[cur_new] = LC[cur_old]  # nhánh trái dùng chung với version cũ
                child = cnt; cnt += 1
                RC[cur_new] = child
                cur_old = RC[cur_old]
                cur_new = child
                lo = mid + 1
        SUM[cur_new] = SUM[cur_old] + val  # lá: tổng tiền của giá trị này
        roots[i] = new_root

    # Xử lý từng truy vấn bằng greedy trên hiệu hai version.
    out = []
    ap = out.append
    _SUM = SUM; _LC = LC; _RC = RC; _vals = vals; _roots = roots
    for _ in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        na0 = _roots[a - 1]
        nb0 = _roots[b]
        # R = mọi tổng trong [0, R] đều tạo được từ đoạn [a, b].
        R = 0
        # DFS in-order theo thứ tự giá trị tăng dần; s = SUM[nb] - SUM[na].
        stack = [(na0, nb0, 0, m1)]
        while stack:
            na, nb, lo, hi = stack.pop()
            s = _SUM[nb] - _SUM[na]
            if s == 0:
                continue  # đoạn giá trị này không có đồng xu nào
            if _vals[lo] > R + 1:
                break  # giá trị nhỏ nhất còn lại > R+1 -> dừng
            if _vals[hi] <= R + 1:
                R += s  # gộp cả cây con vào reach một lần
                continue
            mid = (lo + hi) >> 1
            # Đẩy phải trước, trái sau -> pop trái trước (giữ đúng in-order).
            stack.append((_RC[na], _RC[nb], mid + 1, hi))
            stack.append((_LC[na], _LC[nb], lo, mid))
        ap(R + 1)

    sys.stdout.buffer.write(("\n".join(map(str, out)) + "\n").encode())


main()
