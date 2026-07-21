# Lines and Queries I - CSES 3429
# https://cses.fi/problemset/task/3429
#
# Add lines y = a*x + b, and for a query position x report the maximum
# value over all added lines. Since 0 <= x <= 100000, we use a Li Chao
# Tree (max version) over the integer domain [0, N]. All comparisons use
# exact integer arithmetic (Python big ints), so there is no float error.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1

    N = 100000                 # x in [0, N]
    size = 4 * (N + 1)
    NEG = -(1 << 62)           # sentinel: below any real line value (~1e14)

    A = [0] * size             # slope stored at each node
    B = [NEG] * size           # intercept stored at each node (empty = NEG)

    out = []

    for _ in range(n):
        t = data[pos]; pos += 1
        if t == b'1':
            # add line y = nm*x + nb  (Li Chao insert, single downward path)
            nm = int(data[pos]); nb = int(data[pos + 1]); pos += 2
            node = 1
            l = 0
            r = N
            while True:
                m = (l + r) >> 1
                cm = A[node]; cb = B[node]
                # keep the line that is larger at the midpoint m in this node,
                # push the loser down. Only ONE evaluation pair is needed here.
                if nm * m + nb > cm * m + cb:
                    A[node] = nm; B[node] = nb   # new line wins at m
                    ws = nm                       # winner slope
                    nm, nb = cm, cb               # loser becomes the pushed line
                else:
                    ws = cm                       # old line (still in node) wins
                    # (nm, nb) is already the loser to push down
                if l == r:
                    break
                # The loser can only beat the winner on ONE side of m; the side
                # is decided by slope: smaller slope -> larger for x < m (left).
                if nm < ws:
                    node += node          # go left child
                    r = m
                else:
                    node += node + 1      # go right child
                    l = m + 1
        else:
            # query max at x
            x = int(data[pos]); pos += 1
            node = 1
            l = 0
            r = N
            best = NEG
            while True:
                v = A[node] * x + B[node]
                if v > best:
                    best = v
                if l == r:
                    break
                m = (l + r) >> 1
                if x <= m:
                    node += node
                    r = m
                else:
                    node += node + 1
                    l = m + 1
            out.append(best)

    sys.stdout.write('\n'.join(map(str, out)))
    if out:
        sys.stdout.write('\n')


main()
