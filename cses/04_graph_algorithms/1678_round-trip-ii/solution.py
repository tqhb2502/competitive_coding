# Round Trip II - CSES 1678
# https://cses.fi/problemset/task/1678
# Find a directed cycle in a directed graph using a 3-color DFS.
# Written iteratively (explicit stack) to avoid recursion limit / stack overflow.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    m = int(data[1])

    adj = [[] for _ in range(n + 1)]
    pos = 2
    for _ in range(m):
        a = int(data[pos])
        b = int(data[pos + 1])
        pos += 2
        adj[a].append(b)

    color = [0] * (n + 1)   # 0 white, 1 gray (on the current stack), 2 black (done)
    parent = [0] * (n + 1)
    it = [0] * (n + 1)      # per-node index pointer into the adjacency list

    cycle = None

    for start in range(1, n + 1):
        if color[start] != 0:
            continue
        color[start] = 1
        parent[start] = 0
        stack = [start]
        while stack:
            node = stack[-1]
            adj_node = adj[node]
            if it[node] < len(adj_node):
                nxt = adj_node[it[node]]
                it[node] += 1
                c = color[nxt]
                if c == 0:
                    color[nxt] = 1
                    parent[nxt] = node
                    stack.append(nxt)
                elif c == 1:
                    # back edge node -> nxt: found a cycle nxt -> ... -> node -> nxt
                    cyc = [nxt]
                    cur = node
                    while cur != nxt:
                        cyc.append(cur)
                        cur = parent[cur]
                    cyc.append(nxt)
                    cyc.reverse()   # [nxt, ..., node, nxt]
                    cycle = cyc
                    break
                # c == 2 (black): skip, this edge cannot form a new cycle
            else:
                color[node] = 2
                stack.pop()
        if cycle is not None:
            break

    if cycle is None:
        sys.stdout.write("IMPOSSIBLE\n")
    else:
        out = str(len(cycle)) + "\n" + " ".join(map(str, cycle)) + "\n"
        sys.stdout.write(out)


main()
