import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    edges = []
    for _ in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        edges.append((a, b, c))

    # Khởi tạo dist = 0 cho MỌI đỉnh (tương đương thêm đỉnh nguồn ảo nối tới
    # tất cả các đỉnh với trọng số 0) để phát hiện chu trình âm ở bất kỳ đâu,
    # kể cả các chu trình không đến được từ đỉnh 1.
    dist = [0] * (n + 1)
    parent = [-1] * (n + 1)

    # Bellman-Ford: lặp đúng n vòng, mỗi vòng relax toàn bộ m cạnh.
    x = -1
    for _ in range(n):
        x = -1
        for a, b, c in edges:
            if dist[a] + c < dist[b]:
                dist[b] = dist[a] + c
                parent[b] = a
                x = b  # ghi lại đỉnh vừa được relax
        if x == -1:  # không còn relax được -> đã hội tụ -> không có chu trình âm
            break

    if x == -1:
        sys.stdout.write("NO\n")
        return

    # x được relax ở vòng thứ n => đi ngược theo parent n bước chắc chắn rơi
    # vào một đỉnh nằm trong chu trình âm.
    y = x
    for _ in range(n):
        y = parent[y]

    # Tái tạo chu trình theo chuỗi parent (hướng ngược), sau đó đảo lại.
    cycle = [y]
    cur = parent[y]
    while cur != y:
        cycle.append(cur)
        cur = parent[cur]
    cycle.append(y)
    cycle.reverse()

    sys.stdout.write("YES\n" + " ".join(map(str, cycle)) + "\n")


main()
