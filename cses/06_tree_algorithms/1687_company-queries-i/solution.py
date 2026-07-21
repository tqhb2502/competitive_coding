# Company Queries I - https://cses.fi/problemset/task/1687
# K-th ancestor via binary lifting.
# Fast input; build up[j][v] = ancestor of v that is 2^j steps up (0 = virtual node).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    LOG = 18  # 2^18 = 262144 > 2*10^5

    # parent[v], 1-indexed; parent[1] = 0 (root has no parent)
    parent = [0] * (n + 1)
    for v in range(2, n + 1):
        parent[v] = int(data[idx]); idx += 1

    # Binary lifting: up is a flat array of size LOG*(n+1)
    # up[j*(n+1) + v] = ancestor of v that is 2^j steps up
    size = n + 1
    up = [0] * (LOG * size)
    # level 0 = parent
    for v in range(1, n + 1):
        up[v] = parent[v]
    # higher levels
    for j in range(1, LOG):
        base = j * size
        prev = (j - 1) * size
        for v in range(1, n + 1):
            up[base + v] = up[prev + up[prev + v]]

    out = []
    for _ in range(q):
        x = int(data[idx]); k = int(data[idx + 1]); idx += 2
        # jump according to the set bits of k
        j = 0
        kk = k
        while kk and x:
            if kk & 1:
                x = up[j * size + x]
            kk >>= 1
            j += 1
        out.append(str(x) if x else "-1")

    sys.stdout.buffer.write("\n".join(out).encode())
    sys.stdout.buffer.write(b"\n")


if __name__ == "__main__":
    main()
