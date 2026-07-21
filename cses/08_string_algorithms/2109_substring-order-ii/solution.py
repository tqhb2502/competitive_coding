# Substring Order II - CSES task 2109
# https://cses.fi/problemset/task/2109
#
# Consider ALL substrings of the string counting repetitions (each occurrence
# position counts as its own element, n(n+1)/2 in total). Sort them
# lexicographically and print the k-th one.
#
# Suffix automaton (SAM), deterministic (no hashing):
#   - cnt[v] = number of occurrences (endpos size) of the substrings in state v.
#   - g[v]   = total number of (substring, counting repetitions) reachable by
#              continuing from v (excluding the string corresponding to v itself).
#   Then walk the automaton in alphabetical order to locate the k-th substring.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0]
    k = int(data[1])
    n = len(s)

    # ---- Build the suffix automaton ----
    size_cap = 2 * n + 5
    length = [0] * size_cap
    link = [-1] * size_cap
    nxt = [None] * size_cap
    cnt = [0] * size_cap
    nxt[0] = {}
    sz = 1
    last = 0

    for c in s:  # iterating bytes -> c is the int value of the character
        cur = sz
        sz += 1
        length[cur] = length[last] + 1
        cnt[cur] = 1
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
                cnt[clone] = 0
                while p != -1 and nxt[p].get(c) == q:
                    nxt[p][c] = clone
                    p = link[p]
                link[q] = clone
                link[cur] = clone
        last = cur

    ns = sz

    # ---- Sort states by length via counting sort (ascending order) ----
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
    # order: states in ascending length

    # ---- Compute cnt (endpos size) by propagating along suffix links ----
    for i in range(ns - 1, -1, -1):
        v = order[i]
        p = link[v]
        if p >= 0:
            cnt[p] += cnt[v]

    # ---- Compute g[v] in decreasing length order (transitions always go to larger length) ----
    g = [0] * ns
    for i in range(ns - 1, -1, -1):
        v = order[i]
        tot = 0
        for u in nxt[v].values():
            tot += cnt[u] + g[u]
        g[v] = tot

    # ---- Walk to find the k-th substring ----
    res = bytearray()
    v = 0
    done = False
    while not done:
        trans = nxt[v]
        for c in range(97, 123):  # 'a'..'z'
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
