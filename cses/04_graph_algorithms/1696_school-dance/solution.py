# School Dance - CSES 1696
# https://cses.fi/problemset/task/1696
# Maximum bipartite matching using Kuhn's augmenting-path algorithm.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1   # number of boys (1..n)
    m = int(data[idx]); idx += 1   # number of girls (1..m)
    k = int(data[idx]); idx += 1   # number of potential pairs

    # adjacency list: for each boy, the girls he can dance with
    adj = [[] for _ in range(n + 1)]
    for _ in range(k):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)

    matchL = [-1] * (n + 1)   # matchL[boy]  = girl or -1
    matchR = [-1] * (m + 1)   # matchR[girl] = boy  or -1

    # Iterative DFS to find an augmenting path starting from boy `start`.
    # visited[] marks girls seen during the current search.
    visited = [0] * (m + 1)
    it = [0] * (n + 1)  # iterator pointer over adj[boy] within one search

    def try_augment(start, token):
        # Explicit stack of boys forming the alternating path.
        stack = [start]
        it[start] = 0
        while stack:
            u = stack[-1]
            advanced = False
            while it[u] < len(adj[u]):
                v = adj[u][it[u]]
                it[u] += 1
                if visited[v] == token:
                    continue
                visited[v] = token
                w = matchR[v]
                if w == -1:
                    # Free girl: augment along the whole stack.
                    # v is matched to u; propagate back up the chain.
                    cur_v = v
                    for boy in reversed(stack):
                        prev = matchL[boy]
                        matchR[cur_v] = boy
                        matchL[boy] = cur_v
                        cur_v = prev
                    return True
                else:
                    # Girl v is taken by boy w; try to re-route w.
                    stack.append(w)
                    it[w] = 0
                    advanced = True
                    break
            if not advanced:
                stack.pop()
        return False

    for boy in range(1, n + 1):
        try_augment(boy, boy)

    out = []
    r = 0
    for boy in range(1, n + 1):
        if matchL[boy] != -1:
            r += 1
    out.append(str(r))
    for boy in range(1, n + 1):
        if matchL[boy] != -1:
            out.append(f"{boy} {matchL[boy]}")

    sys.stdout.write("\n".join(out) + "\n")


main()
