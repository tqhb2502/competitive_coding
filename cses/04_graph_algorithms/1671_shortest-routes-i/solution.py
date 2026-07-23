import sys
from heapq import heappush, heappop


def main():
    # Đọc toàn bộ input dạng nhị phân và chuyển sang int trong một lượt ở tầng C
    # (nhanh hơn nhiều so với gọi int() từng token trong vòng lặp Python).
    nums = list(map(int, sys.stdin.buffer.read().split()))
    n = nums[0]
    m = nums[1]
    us = nums[2::3]                # đỉnh nguồn a_i của mỗi cạnh
    vs = nums[3::3]                # đỉnh đích b_i của mỗi cạnh
    ws = nums[4::3]                # trọng số c_i của mỗi cạnh

    # Dựng đồ thị ở dạng CSR (compressed sparse row) cho gọn và nhanh.
    # head[i] là vị trí bắt đầu của danh sách kề đỉnh i sau khi lấy tổng tiền tố.
    head = [0] * (n + 2)          # đếm bậc rồi cộng dồn thành tổng tiền tố
    for a in us:
        head[a + 1] += 1
    for i in range(1, n + 2):
        head[i] += head[i - 1]

    to = [0] * m
    wt = [0] * m
    fill = head[:]                 # vị trí ghi hiện tại của mỗi đỉnh
    for i in range(m):
        a = us[i]
        p = fill[a]
        to[p] = vs[i]
        wt[p] = ws[i]
        fill[a] = p + 1

    # Dijkstra từ đỉnh nguồn 1: khởi tạo dist và min-heap.
    INF = float('inf')
    dist = [INF] * (n + 1)
    dist[1] = 0
    heap = [(0, 1)]

    # Gán tham chiếu cục bộ để tăng tốc.
    _push = heappush
    _pop = heappop
    while heap:
        d, u = _pop(heap)
        # Bỏ qua bản ghi cũ (lazy deletion) nếu không còn là khoảng cách tối ưu.
        if d > dist[u]:
            continue
        # Relax mọi cạnh xuất phát từ u.
        start = head[u]
        end = head[u + 1]
        for e in range(start, end):
            v = to[e]
            nd = d + wt[e]
            if nd < dist[v]:
                dist[v] = nd
                _push(heap, (nd, v))

    # In khoảng cách ngắn nhất từ đỉnh 1 tới mọi đỉnh.
    out = ' '.join(map(str, dist[1:n + 1]))
    sys.stdout.write(out + '\n')


main()
