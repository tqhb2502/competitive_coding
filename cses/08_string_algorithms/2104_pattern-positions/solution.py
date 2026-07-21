# Pattern Positions - CSES 2104
# https://cses.fi/problemset/task/2104
#
# Build a suffix automaton (SAM) of the text s. For each state keep firstpos =
# smallest (0-indexed) ending position of the substrings in that state's endpos
# class, i.e. the ending position of the first occurrence. To answer a pattern p,
# walk it through the automaton; if a transition is missing p is not a substring
# (-1). Otherwise we reach state v and the 1-indexed first-occurrence start is
# firstpos[v] - len(p) + 2.
#
# Build: O(n). Queries: O(total pattern length). Pure Python stdlib only.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]                     # text as bytes; iterating yields ints
    # data[1] is k; patterns are the rest.
    patterns = data[2:]

    n = len(s)
    max_states = 2 * n + 5

    trans = [None] * max_states     # list of dicts: byte -> state
    link = [0] * max_states
    length = [0] * max_states
    firstpos = [0] * max_states

    trans[0] = {}
    link[0] = -1
    length[0] = 0
    sz = 1
    last = 0

    for pos in range(n):
        c = s[pos]
        cur = sz
        sz += 1
        trans[cur] = {}
        length[cur] = length[last] + 1
        firstpos[cur] = length[cur] - 1          # == pos
        p = last
        while p != -1 and c not in trans[p]:
            trans[p][c] = cur
            p = link[p]
        if p == -1:
            link[cur] = 0
        else:
            q = trans[p][c]
            if length[p] + 1 == length[q]:
                link[cur] = q
            else:
                clone = sz
                sz += 1
                length[clone] = length[p] + 1
                trans[clone] = dict(trans[q])
                link[clone] = link[q]
                firstpos[clone] = firstpos[q]
                while p != -1 and trans[p].get(c) == q:
                    trans[p][c] = clone
                    p = link[p]
                link[q] = clone
                link[cur] = clone
        last = cur

    # Answer queries. Localize for speed.
    res = []
    append = res.append
    tr = trans
    fp = firstpos
    for pat in patterns:
        v = 0
        ok = True
        for c in pat:
            nxt = tr[v].get(c)
            if nxt is None:
                ok = False
                break
            v = nxt
        if ok:
            append(fp[v] - len(pat) + 2)
        else:
            append(-1)

    sys.stdout.write('\n'.join(map(str, res)))
    sys.stdout.write('\n')


main()
