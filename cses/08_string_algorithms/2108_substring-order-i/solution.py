import sys


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0]                      # xâu s dưới dạng bytes (mỗi ký tự là một int)
    k = int(data[1])
    n = len(s)

    # Xây dựng suffix automaton (SAM) của s bằng thuật toán online chuẩn.
    MAXST = 2 * n + 5
    sa_len = [0] * MAXST             # len của mỗi trạng thái
    sa_link = [-1] * MAXST           # suffix link
    sa_next = [None] * MAXST         # mỗi trạng thái giữ một dict transition
    sa_next[0] = {}
    sz = 1
    last = 0

    for ch in s:                     # ch là int trong [97, 122]
        cur = sz
        sa_len[cur] = sa_len[last] + 1
        sa_next[cur] = {}
        sz += 1
        # Đi ngược theo suffix link, thêm transition tới trạng thái mới.
        p = last
        while p != -1 and ch not in sa_next[p]:
            sa_next[p][ch] = cur
            p = sa_link[p]
        if p == -1:
            sa_link[cur] = 0
        else:
            q = sa_next[p][ch]
            if sa_len[p] + 1 == sa_len[q]:
                sa_link[cur] = q
            else:
                # Nhân bản (clone) trạng thái q để giữ tính chất của SAM.
                clone = sz
                sa_len[clone] = sa_len[p] + 1
                sa_next[clone] = dict(sa_next[q])
                sa_link[clone] = sa_link[q]
                sz += 1
                while p != -1 and sa_next[p].get(ch) == q:
                    sa_next[p][ch] = clone
                    p = sa_link[p]
                sa_link[q] = clone
                sa_link[cur] = clone
        last = cur

    # Xử lý các trạng thái theo thứ tự len GIẢM dần (thứ tự topo ngược hợp lệ,
    # vì mọi transition đều đi từ len nhỏ sang len lớn hơn hẳn).
    order = sorted(range(sz), key=sa_len.__getitem__, reverse=True)
    # DP: paths[v] = số xâu con phân biệt sinh ra khi rời v bằng ít nhất 1 ký tự.
    paths = [0] * sz
    for v in order:
        total = 0
        for u in sa_next[v].values():
            total += 1 + paths[u]
        paths[v] = total

    # Truy vết greedy từ root để dựng xâu con thứ k.
    res = bytearray()
    v = 0
    while True:
        nxt = sa_next[v]
        moved = False
        for c in sorted(nxt.keys()):     # xét ký tự theo thứ tự từ điển
            u = nxt[c]
            cnt = 1 + paths[u]           # số xâu con trong nhánh theo ký tự c
            if k <= cnt:
                # Chọn ký tự c; giảm k đi 1 ứng với xâu kết thúc ngay tại đây.
                res.append(c)
                k -= 1
                if k == 0:
                    sys.stdout.write(res.decode() + "\n")
                    return
                v = u
                moved = True
                break
            else:
                # Bỏ qua trọn nhánh này.
                k -= cnt
        if not moved:
            break                        # không xảy ra với k hợp lệ

    sys.stdout.write(res.decode() + "\n")


main()
