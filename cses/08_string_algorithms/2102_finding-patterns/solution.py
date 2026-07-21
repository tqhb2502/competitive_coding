# Finding Patterns - https://cses.fi/problemset/task/2102
# Xây dựng suffix automaton (SAM) của text, sau đó với mỗi pattern chỉ cần đi
# theo các transition từ trạng thái khởi đầu: nếu đi hết pattern thì in "YES",
# nếu tắc (không còn transition) thì in "NO". SAM nhận diện đúng tập hợp mọi
# substring của text, nên đây là thuật toán deterministic (không dùng hashing).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    k = int(data[1]) if len(data) > 1 else 0
    patterns = data[2:2 + k]

    n = len(s)
    max_states = 2 * n + 5

    sa_len = [0] * max_states      # độ dài chuỗi dài nhất của mỗi trạng thái
    sa_link = [-1] * max_states    # suffix link
    sa_next = [None] * max_states  # dict transition cho mỗi trạng thái

    sa_next[0] = {}
    sa_len[0] = 0
    sa_link[0] = -1
    size = 1
    last = 0

    for c in s:  # c là giá trị byte (int) của ký tự
        cur = size
        sa_len[cur] = sa_len[last] + 1
        sa_link[cur] = -1
        sa_next[cur] = {}
        size += 1

        p = last
        while p != -1 and c not in sa_next[p]:
            sa_next[p][c] = cur
            p = sa_link[p]

        if p == -1:
            sa_link[cur] = 0
        else:
            q = sa_next[p][c]
            if sa_len[p] + 1 == sa_len[q]:
                sa_link[cur] = q
            else:
                clone = size
                sa_len[clone] = sa_len[p] + 1
                sa_next[clone] = dict(sa_next[q])
                sa_link[clone] = sa_link[q]
                size += 1
                while p != -1 and sa_next[p].get(c) == q:
                    sa_next[p][c] = clone
                    p = sa_link[p]
                sa_link[q] = clone
                sa_link[cur] = clone
        last = cur

    trans = sa_next
    out = []
    ap = out.append
    for pat in patterns:
        state = 0
        ok = True
        for c in pat:
            nxt = trans[state]
            t = nxt.get(c, -1)
            if t == -1:
                ok = False
                break
            state = t
        ap(b"YES" if ok else b"NO")

    sys.stdout.buffer.write(b"\n".join(out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
