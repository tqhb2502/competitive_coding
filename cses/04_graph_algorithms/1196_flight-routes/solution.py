import sys
from heapq import heappush, heappop


def main():
    # Đọc toàn bộ input theo buffer cho nhanh.
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    k = int(data[idx]); idx += 1

    # Đồ thị có hướng dạng adjacency list: mỗi cạnh là (v, w) = (đỉnh đích, giá vé).
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        adj[a].append((b, c))

    # cnt[u] = số lần đỉnh u đã được pop và finalize (tối đa k lần).
    cnt = [0] * (n + 1)
    heap = [(0, 1)]              # min-heap gồm các trạng thái (khoảng cách, đỉnh)
    results = []

    # Gắn hàm hot-loop vào biến cục bộ để giảm chi phí tra cứu mỗi vòng lặp.
    push = heappush
    pop = heappop

    # Dijkstra mở rộng: dừng khi đã thu đủ k đường đi rẻ nhất tới đỉnh n.
    while heap and len(results) < k:
        d, u = pop(heap)
        # Đỉnh này đã đủ k khoảng cách tốt nhất, bỏ qua.
        if cnt[u] >= k:
            continue
        # Lần pop hợp lệ thứ i của u cho đường đi ngắn thứ i tới u.
        cnt[u] += 1
        if u == n:
            results.append(d)
            if len(results) == k:
                break
        # Mở rộng sang các đỉnh kề chưa đủ k lần finalize.
        for v, w in adj[u]:
            if cnt[v] < k:
                push(heap, (d + w, v))

    # In k tổng giá theo thứ tự không giảm.
    sys.stdout.write(' '.join(map(str, results)))


main()
