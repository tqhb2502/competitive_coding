# Teleporters Path - CSES 1693
# https://cses.fi/problemset/task/1693
# Directed Eulerian path from node 1 to node n, using every edge exactly once.
# Hierholzer's algorithm (iterative) in O(n + m).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    adj = [[] for _ in range(n + 1)]
    outdeg = [0] * (n + 1)
    indeg = [0] * (n + 1)

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        outdeg[a] += 1
        indeg[b] += 1

    # Degree conditions for a directed Eulerian path from node 1 to node n.
    # (n >= 2 so 1 != n always.)
    ok = True
    for v in range(1, n + 1):
        d = outdeg[v] - indeg[v]
        if v == 1:
            if d != 1:
                ok = False
                break
        elif v == n:
            if d != -1:
                ok = False
                break
        else:
            if d != 0:
                ok = False
                break

    if not ok:
        sys.stdout.write("IMPOSSIBLE\n")
        return

    # Iterative Hierholzer starting at node 1.
    ptr = [0] * (n + 1)
    stack = [1]
    path = []
    while stack:
        v = stack[-1]
        p = ptr[v]
        if p < len(adj[v]):
            ptr[v] = p + 1
            stack.append(adj[v][p])
        else:
            path.append(v)
            stack.pop()

    # Must use every edge exactly once -> path has m + 1 vertices.
    # This also enforces the connectivity requirement.
    if len(path) != m + 1:
        sys.stdout.write("IMPOSSIBLE\n")
        return

    path.reverse()
    sys.stdout.write(' '.join(map(str, path)))
    sys.stdout.write('\n')


main()
