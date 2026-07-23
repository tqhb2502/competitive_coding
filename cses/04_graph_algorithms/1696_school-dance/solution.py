import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1   # số bạn nam (1..n)
    m = int(data[idx]); idx += 1   # số bạn nữ (1..m)
    k = int(data[idx]); idx += 1   # số cặp tiềm năng

    # Danh sách kề: với mỗi nam, các nữ mà nam đó có thể nhảy cùng.
    adj = [[] for _ in range(n + 1)]
    for _ in range(k):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)

    matchL = [-1] * (n + 1)   # matchL[nam] = nữ đang ghép, hoặc -1
    matchR = [-1] * (m + 1)   # matchR[nữ]  = nam đang ghép, hoặc -1

    # DFS dạng lặp tìm augmenting path bắt đầu từ bạn nam `start`.
    # visited[] đánh dấu (theo token) các nữ đã thăm trong lần tìm kiếm hiện tại;
    # it[] là con trỏ duyệt qua adj[nam] trong một lần tìm kiếm.
    visited = [0] * (m + 1)
    it = [0] * (n + 1)

    def try_augment(start, token):
        # Ngăn xếp tường minh gồm các nam tạo thành đường xen kẽ (alternating path).
        stack = [start]
        it[start] = 0
        while stack:
            u = stack[-1]
            advanced = False
            while it[u] < len(adj[u]):
                v = adj[u][it[u]]
                it[u] += 1
                if visited[v] == token:
                    continue
                visited[v] = token
                w = matchR[v]
                if w == -1:
                    # Gặp nữ còn trống: tăng luồng dọc theo cả ngăn xếp,
                    # lần ngược lên chuỗi để cập nhật lại từng cặp ghép.
                    cur_v = v
                    for boy in reversed(stack):
                        prev = matchL[boy]
                        matchR[cur_v] = boy
                        matchL[boy] = cur_v
                        cur_v = prev
                    return True
                else:
                    # Nữ v đang bị nam w giữ; thử đẩy w sang nữ khác.
                    stack.append(w)
                    it[w] = 0
                    advanced = True
                    break
            if not advanced:
                stack.pop()
        return False

    # Lần lượt thử tăng matching cho từng bạn nam.
    for boy in range(1, n + 1):
        try_augment(boy, boy)

    # Đếm số cặp cực đại và xuất danh sách các cặp đã ghép.
    out = []
    r = 0
    for boy in range(1, n + 1):
        if matchL[boy] != -1:
            r += 1
    out.append(str(r))
    for boy in range(1, n + 1):
        if matchL[boy] != -1:
            out.append(f"{boy} {matchL[boy]}")

    sys.stdout.write("\n".join(out) + "\n")


main()
