# Substring Distribution - CSES 2110
# https://cses.fi/problemset/task/2110
#
# Đếm số xâu con (substring) phân biệt của mỗi độ dài L = 1..n.
# Dùng suffix automaton: mỗi state (trừ state khởi tạo) biểu diễn một lớp
# endpos, chứa các xâu có độ dài trong khoảng [len[link]+1, len[state]],
# và mỗi độ dài trong khoảng đó ứng với đúng một xâu con phân biệt.
# Dùng difference array cộng 1 trên từng khoảng -> prefix sum ra đáp án.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]                      # bytes; lặp qua cho ra các int
    n = len(s)

    max_states = 2 * n + 5
    sa_len = [0] * max_states
    sa_link = [-1] * max_states
    sa_next = [None] * max_states
    sa_next[0] = {}
    size = 1
    last = 0

    for ch in s:                     # ch là int (giá trị byte)
        cur = size
        size += 1
        sa_len[cur] = sa_len[last] + 1
        sa_next[cur] = {}
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
                clone = size
                size += 1
                sa_len[clone] = sa_len[p] + 1
                sa_next[clone] = dict(sa_next[q])
                sa_link[clone] = sa_link[q]
                while p != -1 and sa_next[p].get(ch) == q:
                    sa_next[p][ch] = clone
                    p = sa_link[p]
                sa_link[q] = clone
                sa_link[cur] = clone
        last = cur

    # difference array: mỗi state cộng 1 vào các độ dài [len[link]+1, len]
    diff = [0] * (n + 2)
    for v in range(1, size):
        L = sa_len[sa_link[v]] + 1
        R = sa_len[v]
        diff[L] += 1
        diff[R + 1] -= 1

    out = []
    run = 0
    for L in range(1, n + 1):
        run += diff[L]
        out.append(run)

    sys.stdout.write(' '.join(map(str, out)))
    sys.stdout.write('\n')


main()
