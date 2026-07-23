import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    grid = data[2:2 + n]

    # Bản đồ phẳng để truy cập O(1): flat[r*m + c] là ký tự của ô (r, c).
    flat = b"".join(grid)

    # Xác định chỉ số phẳng của ô xuất phát 'A' và ô đích 'B'.
    start = -1
    end = -1
    for r in range(n):
        row = grid[r]
        a = row.find(b"A")
        if a != -1:
            start = r * m + a
        b = row.find(b"B")
        if b != -1:
            end = r * m + b

    WALL = 35  # ord('#')

    # visited vừa đánh dấu đã thăm vừa lưu hướng để truy vết:
    # 0 = chưa thăm, 1=U, 2=D, 3=L, 4=R, 5 = ô xuất phát A.
    visited = bytearray(n * m)
    visited[start] = 5

    dq = deque()
    dq.append(start)
    append = dq.append
    popleft = dq.popleft

    # BFS từ 'A': mọi cạnh trọng số 1 nên đường đi tìm được là ngắn nhất theo số bước.
    while dq:
        cur = popleft()
        if cur == end:
            break
        r = cur // m
        c = cur - r * m

        # Xét 4 ô kề cạnh; ô nào trong lưới, không phải tường và chưa thăm thì thăm.
        if c > 0:
            nb = cur - 1
            if not visited[nb] and flat[nb] != WALL:
                visited[nb] = 3  # L
                append(nb)
        if c + 1 < m:
            nb = cur + 1
            if not visited[nb] and flat[nb] != WALL:
                visited[nb] = 4  # R
                append(nb)
        if r > 0:
            nb = cur - m
            if not visited[nb] and flat[nb] != WALL:
                visited[nb] = 1  # U
                append(nb)
        if r + 1 < n:
            nb = cur + m
            if not visited[nb] and flat[nb] != WALL:
                visited[nb] = 2  # D
                append(nb)

    # Nếu 'B' chưa được thăm thì không tồn tại đường đi.
    if not visited[end]:
        sys.stdout.buffer.write(b"NO\n")
        return

    letters = {1: b"U", 2: b"D", 3: b"L", 4: b"R"}
    # Offset để lùi về ô liền trước tùy theo hướng đã đi.
    back = {1: m, 2: -m, 3: 1, 4: -1}

    # Truy vết ngược từ 'B' về 'A' rồi đảo lại để có thứ tự từ 'A' tới 'B'.
    path = []
    cur = end
    while cur != start:
        code = visited[cur]
        path.append(letters[code])
        cur += back[code]
    path.reverse()

    sys.stdout.buffer.write(b"YES\n%d\n%s\n" % (len(path), b"".join(path)))


if __name__ == "__main__":
    main()
