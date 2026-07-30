import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc.
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); m = int(data[pos + 1]); q = int(data[pos + 2])
    pos += 3

    # INF lớn hơn khoảng cách thật lớn nhất (~ (n-1)*1e9 < 2^40): mọi tổng đi qua một
    # cạnh không tồn tại vẫn >= INF, không tạo ra "đường tắt" giả.
    INF = 1 << 40

    # Ma trận khoảng cách: dist[i][i] = 0, còn lại khởi tạo INF.
    dist = [[INF] * n for _ in range(n)]
    for i in range(n):
        dist[i][i] = 0

    # Nạp các cạnh vô hướng; nếu có nhiều đường giữa cùng một cặp thì giữ đường nhỏ nhất.
    for _ in range(m):
        a = int(data[pos]) - 1
        b = int(data[pos + 1]) - 1
        c = int(data[pos + 2])
        pos += 3
        if c < dist[a][b]:
            dist[a][b] = c
            dist[b][a] = c

    # Floyd-Warshall: k là đỉnh trung gian. Vòng lặp trong cùng viết bằng list
    # comprehension + zip (chạy ở mức C) để nhanh nhất có thể trong pure Python.
    for k in range(n):
        dk = dist[k]
        for di in dist:
            dik = di[k]
            # Bỏ qua hàng khi không tới được k -> không thể cải thiện gì.
            if dik < INF:
                # di[j] = min(di[j], dik + dk[j]); walrus tính dik + dkj một lần.
                di[:] = [dij if dij < (s := dik + dkj) else s
                         for dij, dkj in zip(di, dk)]

    # Trả lời truy vấn trong O(1); còn INF nghĩa là không có đường -> in -1.
    out = []
    append = out.append
    for _ in range(q):
        a = int(data[pos]) - 1
        b = int(data[pos + 1]) - 1
        pos += 2
        d = dist[a][b]
        append(str(d) if d < INF else "-1")

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
