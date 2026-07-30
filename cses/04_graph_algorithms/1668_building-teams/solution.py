import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    ints = list(map(int, data))
    n = ints[0]
    m = ints[1]

    # Xây dựng danh sách kề cho đồ thị vô hướng: mỗi cạnh thêm vào cả hai chiều.
    adj = [[] for _ in range(n + 1)]
    idx = 2
    for _ in range(m):
        a = ints[idx]
        b = ints[idx + 1]
        idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # color = 0 nghĩa là chưa tô màu; màu 1 và màu 2 tương ứng hai đội.
    color = [0] * (n + 1)
    # Duyệt từng thành phần liên thông và tô 2 màu (kiểm tra bipartite) bằng BFS.
    for s in range(1, n + 1):
        if color[s]:
            continue
        # Đỉnh gốc của thành phần: gán đội 1 rồi lan truyền bằng BFS.
        color[s] = 1
        q = deque([s])
        while q:
            u = q.popleft()
            other = 3 - color[u]  # Màu ngược với u dành cho các đỉnh kề.
            for w in adj[u]:
                c = color[w]
                if c == 0:
                    # Đỉnh chưa tô: gán màu ngược rồi đưa vào hàng đợi.
                    color[w] = other
                    q.append(w)
                elif c != other:
                    # Cạnh nối hai đỉnh cùng màu -> có chu trình lẻ -> không chia được.
                    sys.stdout.write("IMPOSSIBLE\n")
                    return
        # BFS lặp (không đệ quy) nên không bị RecursionError dù n tới 1e5.

    # Tô màu thành công: in đội của từng học sinh theo thứ tự 1..n.
    sys.stdout.write(" ".join(map(str, color[1:n + 1])))
    sys.stdout.write("\n")


main()
