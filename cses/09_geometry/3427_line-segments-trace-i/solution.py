import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Quy mỗi đoạn về đường thẳng f(x) = s*x + b, với s là slope nguyên và
    # b = giá trị tại x = 0. Cùng một slope chỉ giữ intercept lớn nhất.
    best = {}
    for _ in range(n):
        y1 = int(data[idx]); y2 = int(data[idx + 1]); idx += 2
        s = (y2 - y1) // m          # slope nguyên, phép chia là chính xác
        b = y1                      # giá trị tại x = 0
        cur = best.get(s)
        if cur is None or b > cur:
            best[s] = b

    # Sắp xếp các đường theo slope tăng dần.
    lines = sorted(best.items())    # [(slope, intercept), ...]

    # Xây upper hull cho phép lấy MAX (Convex Hull Trick đơn điệu).
    hm = []   # danh sách slope
    hc = []   # danh sách intercept
    for s, c in lines:
        while len(hm) >= 2:
            m1 = hm[-2]; c1 = hc[-2]
            m2 = hm[-1]; c2 = hc[-1]
            # Loại l2 nếu giao(l2, l3) <= giao(l1, l2) (cross-multiplication nguyên):
            # (c2 - c)*(m2 - m1) <= (c1 - c2)*(s - m2)
            if (c2 - c) * (m2 - m1) <= (c1 - c2) * (s - m2):
                hm.pop(); hc.pop()
            else:
                break
        hm.append(s); hc.append(c)

    # Trả lời truy vấn x = 0..m bằng con trỏ tiến dần (chỉ số tối ưu không giảm).
    L = len(hm)
    out = []
    ptr = 0
    for x in range(m + 1):
        while ptr + 1 < L and hm[ptr + 1] * x + hc[ptr + 1] >= hm[ptr] * x + hc[ptr]:
            ptr += 1
        out.append(hm[ptr] * x + hc[ptr])

    sys.stdout.write(' '.join(map(str, out)))
    sys.stdout.write('\n')


main()
