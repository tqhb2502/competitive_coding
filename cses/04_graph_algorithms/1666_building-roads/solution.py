# Building Roads - CSES 1666
# https://cses.fi/problemset/task/1666
# Find connected components with DSU, then connect them with (c - 1) new roads.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    parent = list(range(n + 1))
    size = [1] * (n + 1)

    # find with path compression (iterative)
    def find(x):
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        ra, rb = find(a), find(b)
        if ra != rb:
            # union by size
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]

    # collect one representative vertex per component
    reps = []
    for v in range(1, n + 1):
        if find(v) == v:
            reps.append(v)

    out = []
    out.append(str(len(reps) - 1))
    base = reps[0]
    for i in range(1, len(reps)):
        out.append(str(base) + " " + str(reps[i]))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
