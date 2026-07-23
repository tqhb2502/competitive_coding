import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    segs = []
    for _ in range(n):
        x1 = int(data[pos]); y1 = int(data[pos + 1])
        x2 = int(data[pos + 2]); y2 = int(data[pos + 3])
        pos += 4
        # slope là số nguyên nên phép chia này chia hết -> integer division chính xác.
        a = (y2 - y1) // (x2 - x1)      # slope
        b = y1 - a * x1                 # intercept: y = a*x + b
        segs.append((a, b, x1, x2))

    # Sắp xếp theo slope tăng dần để danh sách line ở mỗi node của segment tree tự sắp.
    segs.sort(key=lambda t: t[0])

    # Segment tree dạng iterative trên trục x với các lá ứng với x = 0..m.
    N = m + 1
    size = 1
    while size < N:
        size <<= 1

    tree = [None] * (2 * size)

    # Phân rã mỗi khoảng [x1, x2] thành O(log m) canonical node, gắn line vào từng node;
    # line lưu ở một node sẽ phủ toàn bộ phạm vi x của node đó.
    for a, b, x1, x2 in segs:
        lo = x1 + size
        hi = x2 + size
        line = (a, b)
        while lo <= hi:
            if lo & 1:
                lst = tree[lo]
                if lst is None:
                    tree[lo] = [line]
                else:
                    lst.append(line)
                lo += 1
            if not (hi & 1):
                lst = tree[hi]
                if lst is None:
                    tree[hi] = [line]
                else:
                    lst.append(line)
                hi -= 1
            lo >>= 1
            hi >>= 1

    # Tính trước phạm vi x [nlo, nhi] mà mỗi node phủ.
    nlo = [0] * (2 * size)
    nhi = [0] * (2 * size)
    for p in range(size, 2 * size):
        pp = p - size
        if pp < N:
            nlo[p] = pp
            nhi[p] = pp
        else:
            # Lá đệm (padding) không thuộc canonical node thật -> khoảng rỗng.
            nlo[p] = N
            nhi[p] = -1
    for p in range(size - 1, 0, -1):
        nlo[p] = nlo[2 * p]
        nhi[p] = nhi[2 * p + 1]

    # ans[x] = -1 nếu không đoạn nào phủ; mọi giá trị phủ được đều không âm.
    ans = [-1] * N

    for p in range(1, 2 * size):
        lst = tree[p]
        if not lst:
            continue

        # Xây upper envelope (max) bằng Convex Hull Trick; lst đã sắp theo slope tăng.
        env_a = []
        env_b = []
        for a, b in lst:
            add = True
            while env_a:
                am = env_a[-1]
                bm = env_b[-1]
                if am == a:
                    # Cùng slope: chỉ giữ đường có intercept lớn hơn.
                    if bm >= b:
                        add = False
                        break
                    env_a.pop()
                    env_b.pop()
                    continue
                if len(env_a) >= 2:
                    al = env_a[-2]
                    bl = env_b[-2]
                    # Đỉnh stack thừa nếu thứ tự crossover không hợp lệ (cross-multiply).
                    if (bm - bl) * (am - a) >= (b - bm) * (al - am):
                        env_a.pop()
                        env_b.pop()
                        continue
                break
            if add:
                env_a.append(a)
                env_b.append(b)

        # Đánh giá envelope trên [L, R] bằng two-pointer (crossover tăng dần theo x).
        L = nlo[p]
        R = nhi[p]
        elen = len(env_a)
        j = 0
        for x in range(L, R + 1):
            while j + 1 < elen and env_a[j + 1] * x + env_b[j + 1] >= env_a[j] * x + env_b[j]:
                j += 1
            v = env_a[j] * x + env_b[j]
            if v > ans[x]:
                ans[x] = v

    sys.stdout.write(" ".join(map(str, ans)))
    sys.stdout.write("\n")


main()
