import sys


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0]
    k = int(data[1])
    n = len(s)

    # ---- Xây Suffix Automaton (SAM) ----
    size_cap = 2 * n + 5
    length = [0] * size_cap
    link = [-1] * size_cap
    nxt = [None] * size_cap
    cnt = [0] * size_cap
    nxt[0] = {}
    sz = 1
    last = 0

    for c in s:  # duyệt bytes -> c là giá trị int của ký tự
        cur = sz
        sz += 1
        length[cur] = length[last] + 1
        cnt[cur] = 1  # state "gốc" -> cnt = 1
        nxt[cur] = {}
        p = last
        while p != -1 and c not in nxt[p]:
            nxt[p][c] = cur
            p = link[p]
        if p == -1:
            link[cur] = 0
        else:
            q = nxt[p][c]
            if length[p] + 1 == length[q]:
                link[cur] = q
            else:
                clone = sz
                sz += 1
                length[clone] = length[p] + 1
                link[clone] = link[q]
                nxt[clone] = dict(nxt[q])
                cnt[clone] = 0  # clone -> cnt = 0
                while p != -1 and nxt[p].get(c) == q:
                    nxt[p][c] = clone
                    p = link[p]
                link[q] = clone
                link[cur] = clone
        last = cur

    ns = sz

    # ---- Sắp xếp state theo length tăng dần bằng counting sort ----
    maxlen = n
    bucket = [0] * (maxlen + 2)
    for v in range(ns):
        bucket[length[v]] += 1
    for i in range(1, maxlen + 2):
        bucket[i] += bucket[i - 1]
    order = [0] * ns
    for v in range(ns):
        L = length[v]
        bucket[L] -= 1
        order[bucket[L]] = v
    # order: các state theo length tăng dần

    # ---- Tính cnt (endpos size): cộng ngược theo suffix link ----
    for i in range(ns - 1, -1, -1):
        v = order[i]
        p = link[v]
        if p >= 0:
            cnt[p] += cnt[v]

    # ---- Tính g[v] theo length giảm dần (transition luôn đi tới length lớn hơn) ----
    g = [0] * ns
    for i in range(ns - 1, -1, -1):
        v = order[i]
        tot = 0
        for u in nxt[v].values():
            tot += cnt[u] + g[u]
        g[v] = tot

    # ---- Đi tìm substring thứ k theo thứ tự từ điển ----
    res = bytearray()
    v = 0
    done = False
    while not done:
        trans = nxt[v]
        for c in range(97, 123):  # duyệt 'a'..'z'
            u = trans.get(c)
            if u is None:
                continue
            occ = cnt[u]
            if k <= occ:
                res.append(c)
                done = True
                break
            k -= occ
            gu = g[u]
            if k <= gu:
                res.append(c)
                v = u
                break
            k -= gu

    sys.stdout.write(res.decode() + "\n")


main()
