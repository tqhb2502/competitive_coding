# Lines and Queries II - CSES task 3430
# https://cses.fi/problemset/task/3430
#
# Type 1: "1 a b l r" -> add line y = a*x + b, active for query positions x in [l, r].
# Type 2: "2 x"       -> print max over all currently-active lines evaluated at x, or NO.
#
# Idea: Li Chao segment tree over the position axis [0, M] (M = 1e5).
#   - To add a line active on [l, r], decompose [l, r] into O(log M) canonical
#     segment-tree nodes and perform a Li Chao insert of the line into each such
#     node's subtree (the line is active over the whole node range, since the node
#     is fully contained in [l, r]).
#   - Any line stored at a node N is active over all of N's range, so for a point
#     query at x we walk the root->leaf path to position x and take the max of the
#     stored lines: exactly one canonical node of every relevant [l, r] lies on that
#     path (segment-tree point/interval property), so all active lines are counted.
#   - All arithmetic is exact integer arithmetic (a*x+b); Python big ints, no floats.
#
# Complexity: insert O(log^2 M), query O(log M).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1

    M = 100000
    SZ = 1
    while SZ < M + 1:
        SZ <<= 1                 # SZ = 131072 = 2^17
    H = SZ.bit_length() - 1      # tree height = 17

    LA = [None] * (2 * SZ)       # slope stored at each node (None = empty)
    LB = [None] * (2 * SZ)       # intercept stored at each node

    out = []
    b1 = b'1'

    for _ in range(n):
        if data[pos] == b1:
            a = int(data[pos + 1]); b = int(data[pos + 2])
            l = int(data[pos + 3]); r = int(data[pos + 4])
            pos += 5

            # Canonical decomposition of [l, r] (half-open [l, r+1)).
            lp = l + SZ
            rp = r + SZ + 1
            nodes = []
            while lp < rp:
                if lp & 1:
                    nodes.append(lp); lp += 1
                if rp & 1:
                    rp -= 1; nodes.append(rp)
                lp >>= 1; rp >>= 1

            for node0 in nodes:
                # Range covered by node0.
                L = node0.bit_length() - 1
                span = SZ >> L
                lo = (node0 - (1 << L)) * span
                hi = lo + span - 1
                # Li Chao insert of line (a, b) into subtree rooted at node0.
                node = node0
                m = a; k = b
                while True:
                    cm = LA[node]
                    if cm is None:
                        LA[node] = m; LB[node] = k
                        break
                    ck = LB[node]
                    mid = (lo + hi) >> 1
                    if m * mid + k > cm * mid + ck:
                        LA[node] = m; LB[node] = k
                        wm = m; wk = k
                        m = cm; k = ck          # loser = old node line
                    else:
                        wm = cm; wk = ck        # loser = incoming line
                    if lo == hi:
                        break
                    if m * lo + k > wm * lo + wk:
                        node = node * 2; hi = mid
                    else:
                        node = node * 2 + 1; lo = mid + 1
        else:
            x = int(data[pos + 1]); pos += 2
            leaf = SZ + x
            best = None
            for s in range(H, -1, -1):
                node = leaf >> s
                a = LA[node]
                if a is not None:
                    v = a * x + LB[node]
                    if best is None or v > best:
                        best = v
            out.append(b"NO" if best is None else str(best).encode())

    sys.stdout.buffer.write(b"\n".join(out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
