import sys
from bisect import bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    ptr = 0
    n = int(data[ptr]); ptr += 1
    q = int(data[ptr]); ptr += 1

    starts = [0] * n
    endsArr = [0] * n
    for i in range(n):
        starts[i] = int(data[ptr]); endsArr[i] = int(data[ptr + 1]); ptr += 2

    # Sắp xếp phim theo start tăng dần, chuẩn bị suffix-min của end.
    order = sorted(range(n), key=starts.__getitem__)
    sortedStart = [starts[i] for i in order]
    sortedEnd = [endsArr[i] for i in order]

    # suffMin[i] = end nhỏ nhất trong các phim (đã sắp) có chỉ số >= i.
    INF = 1 << 30
    suffMin = [INF] * (n + 1)
    for i in range(n - 1, -1, -1):
        e = sortedEnd[i]
        sm = suffMin[i + 1]
        suffMin[i] = e if e < sm else sm

    # Node hóa các giá trị end phân biệt; index m = sentinel "không còn phim".
    endsDistinct = sorted(set(endsArr))
    m = len(endsDistinct)
    nodeVal = endsDistinct + [INF]

    # up0[i] = node của f(endsDistinct[i]) = end nhỏ nhất với start >= end đó;
    # sentinel nếu không còn phim nào.
    up0 = [0] * (m + 1)
    bl_start = sortedStart
    for i in range(m):
        v = endsDistinct[i]
        j = bisect_left(bl_start, v)          # phim đầu tiên có start >= v
        fe = suffMin[j]                        # end nhỏ nhất trong các phim đó
        if fe >= INF:
            up0[i] = m
        else:
            up0[i] = bisect_left(endsDistinct, fe)
    up0[m] = m

    # Binary lifting: up[k][i] = trạng thái sau 2^k bước nhảy.
    # Tầng sau ghép đôi tầng trước: up[k] = [up[k-1][e] for e in up[k-1]].
    LOG = 18  # đáp án <= n <= 2e5 < 2^18
    up = [up0]
    prev = up0
    for _ in range(1, LOG):
        cur = [prev[e] for e in prev]
        up.append(cur)
        prev = cur

    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[ptr]); b = int(data[ptr + 1]); ptr += 2
        # Phim đầu tiên: f(a). Nếu end nhỏ nhất đã vượt b thì không xem được.
        j = bisect_left(bl_start, a)
        fe = suffMin[j]
        if fe > b:
            ap(b'0')
            continue
        # Đã xem 1 phim; nhảy binary lifting tối đa số bước còn hợp lệ (end <= b).
        count = 1
        cur = bisect_left(endsDistinct, fe)   # node của end phim đầu tiên
        for k in range(LOG - 1, -1, -1):
            nxt = up[k][cur]
            if nodeVal[nxt] <= b:
                count += 1 << k
                cur = nxt
        ap(str(count).encode())

    sys.stdout.buffer.write(b'\n'.join(out) + b'\n')


main()
