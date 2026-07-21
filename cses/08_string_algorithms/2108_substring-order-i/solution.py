# Substring Order I  -  https://cses.fi/problemset/task/2108
#
# Given a string s and an integer k, print the k-th smallest DISTINCT
# substring of s in lexicographical order.
#
# Idea: build a suffix automaton (SAM). Every distinct non-empty substring of s
# corresponds to exactly one path starting from the initial state. For each SAM
# state v let paths[v] = number of distinct substrings obtainable by leaving v
# with at least one character (i.e. number of downward paths). Transitions
# strictly increase state length, so we can DP over states in decreasing order
# of len. Then we descend greedily from the root: at each state try characters
# 'a'..'z'; the branch on character c contributes (1 + paths[child]) substrings
# (the substring ending right after c, plus everything extending it).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0]                      # the string as bytes (each char is an int)
    k = int(data[1])
    n = len(s)

    MAXST = 2 * n + 5
    sa_len = [0] * MAXST
    sa_link = [-1] * MAXST
    sa_next = [None] * MAXST         # one dict of transitions per state
    sa_next[0] = {}
    sz = 1
    last = 0

    for ch in s:                     # ch is an int in [97, 122]
        cur = sz
        sa_len[cur] = sa_len[last] + 1
        sa_next[cur] = {}
        sz += 1
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

    # Process states in decreasing len order (a valid reverse topological order,
    # because every transition goes from smaller len to strictly larger len).
    order = sorted(range(sz), key=sa_len.__getitem__, reverse=True)
    paths = [0] * sz
    for v in order:
        total = 0
        for u in sa_next[v].values():
            total += 1 + paths[u]
        paths[v] = total

    # Greedy descent to build the k-th substring.
    res = bytearray()
    v = 0
    while True:
        nxt = sa_next[v]
        moved = False
        for c in sorted(nxt.keys()):
            u = nxt[c]
            cnt = 1 + paths[u]
            if k <= cnt:
                res.append(c)
                k -= 1                # account for the substring ending here
                if k == 0:
                    sys.stdout.write(res.decode() + "\n")
                    return
                v = u
                moved = True
                break
            else:
                k -= cnt
        if not moved:
            break                     # should not happen for valid k

    sys.stdout.write(res.decode() + "\n")


main()
