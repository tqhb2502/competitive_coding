import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    rows = data[2:2 + n]

    WALL = 35   # '#'
    A = 65      # 'A'
    M = 77      # 'M'

    size = n * m
    BIG = size + 5

    wall = bytearray(size)          # 1 nếu là tường
    mon = [BIG] * size              # thời điểm sớm nhất quái vật tới mỗi ô (BFS)
    dq = deque()
    start = -1

    # Đọc lưới; đánh dấu tường, nạp nguồn multi-source (ô 'M') và vị trí 'A'
    for r in range(n):
        row = rows[r]
        base = r * m
        for c in range(m):
            ch = row[c]
            if ch == WALL:
                wall[base + c] = 1
            elif ch == M:
                mon[base + c] = 0
                dq.append(base + c)
            elif ch == A:
                start = base + c

    # Lượt 1: multi-source BFS từ tất cả quái vật -> thời điểm sớm nhất mỗi ô bị chiếm
    while dq:
        cur = dq.popleft()
        d = mon[cur] + 1
        r = cur // m
        c = cur - r * m
        if r > 0:
            nb = cur - m
            if wall[nb] == 0 and mon[nb] == BIG:
                mon[nb] = d
                dq.append(nb)
        if r + 1 < n:
            nb = cur + m
            if wall[nb] == 0 and mon[nb] == BIG:
                mon[nb] = d
                dq.append(nb)
        if c > 0:
            nb = cur - 1
            if wall[nb] == 0 and mon[nb] == BIG:
                mon[nb] = d
                dq.append(nb)
        if c + 1 < m:
            nb = cur + 1
            if wall[nb] == 0 and mon[nb] == BIG:
                mon[nb] = d
                dq.append(nb)

    # Lượt 2: BFS người chơi, chỉ đi vào ô tới được TRƯỚC HẲN mọi quái vật
    dist = [BIG] * size
    par = [-1] * size
    mv = bytearray(size)            # ký tự di chuyển để tới ô này
    U = ord('U')
    D = ord('D')
    L = ord('L')
    R = ord('R')

    ans_cell = -1
    q = deque()

    # Xuất phát chỉ hợp lệ khi người chơi tới trước quái vật (0 < mon[start])
    if mon[start] > 0:
        dist[start] = 0
        sr = start // m
        sc = start - sr * m
        if sr == 0 or sr == n - 1 or sc == 0 or sc == m - 1:
            ans_cell = start          # 'A' đã ở biên
        else:
            q.append(start)

    while q and ans_cell == -1:
        cur = q.popleft()
        nd = dist[cur] + 1
        r = cur // m
        c = cur - r * m
        # lên (U)
        if r > 0:
            nb = cur - m
            if wall[nb] == 0 and dist[nb] == BIG and nd < mon[nb]:
                dist[nb] = nd
                par[nb] = cur
                mv[nb] = U
                if r - 1 == 0 or c == 0 or c == m - 1:
                    ans_cell = nb
                    break
                q.append(nb)
        # xuống (D)
        if r + 1 < n:
            nb = cur + m
            if wall[nb] == 0 and dist[nb] == BIG and nd < mon[nb]:
                dist[nb] = nd
                par[nb] = cur
                mv[nb] = D
                if r + 1 == n - 1 or c == 0 or c == m - 1:
                    ans_cell = nb
                    break
                q.append(nb)
        # trái (L)
        if c > 0:
            nb = cur - 1
            if wall[nb] == 0 and dist[nb] == BIG and nd < mon[nb]:
                dist[nb] = nd
                par[nb] = cur
                mv[nb] = L
                if c - 1 == 0 or r == 0 or r == n - 1:
                    ans_cell = nb
                    break
                q.append(nb)
        # phải (R)
        if c + 1 < m:
            nb = cur + 1
            if wall[nb] == 0 and dist[nb] == BIG and nd < mon[nb]:
                dist[nb] = nd
                par[nb] = cur
                mv[nb] = R
                if c + 1 == m - 1 or r == 0 or r == n - 1:
                    ans_cell = nb
                    break
                q.append(nb)

    if ans_cell == -1:
        sys.stdout.write("NO\n")
        return

    # Truy vết đường đi
    path = []
    cur = ans_cell
    while cur != start:
        path.append(chr(mv[cur]))
        cur = par[cur]
    path.reverse()

    sys.stdout.write("YES\n%d\n%s\n" % (len(path), "".join(path)))


main()
