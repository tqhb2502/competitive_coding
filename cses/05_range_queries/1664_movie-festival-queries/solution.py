# Movie Festival Queries - https://cses.fi/problemset/task/1664
#
# Với mỗi truy vấn (a, b): đếm số lượng phim tối đa xem được, mỗi phim phải
# có start >= a và end <= b, các phim không được chồng lên nhau (một phim có
# thể bắt đầu đúng lúc phim trước kết thúc).
#
# Ý tưởng: greedy interval scheduling (chọn phim kết thúc sớm nhất) + binary
# lifting trên "jump pointer".
#   - f(t) = end nhỏ nhất trong các phim có start >= t.
#   - Nếu đang ở thời điểm t (sớm nhất có thể bắt đầu phim tiếp theo) thì phim
#     greedy tối ưu kết thúc tại f(t), rồi ta chuyển sang trạng thái f(t).
#   - Các đích đến của jump luôn là một giá trị end nào đó -> chỉ cần node hóa
#     tập các end phân biệt, dùng binary lifting để nhảy 2^k bước.

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

    INF = 1 << 30
    suffMin = [INF] * (n + 1)
    for i in range(n - 1, -1, -1):
        e = sortedEnd[i]
        sm = suffMin[i + 1]
        suffMin[i] = e if e < sm else sm

    # Node hóa các giá trị end phân biệt.
    endsDistinct = sorted(set(endsArr))
    m = len(endsDistinct)
    nodeVal = endsDistinct + [INF]  # index m = sentinel "không còn phim nào"

    # up0[i] = node của f(endsDistinct[i]); sentinel nếu không còn phim.
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
    # cur[i] = prev[prev[i]] == [prev[e] for e in prev].
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
        # Phim đầu tiên: f(a).
        j = bisect_left(bl_start, a)
        fe = suffMin[j]
        if fe > b:
            ap(b'0')
            continue
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
