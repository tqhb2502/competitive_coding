# Range Update Queries - https://cses.fi/problemset/task/1651
# Fenwick tree (BIT) on the difference array:
#   range update [a,b] += u  <=>  point update d[a]+=u, d[b+1]-=u
#   point query k            <=>  prefix sum of the difference array up to position k
import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    a = data[idx:idx + n]
    idx += n

    # Fenwick tree stores the difference array; prefix_sum(k) = current value a[k].
    tree = [0] * (n + 2)

    def update(i, delta):
        while i <= n:
            tree[i] += delta
            i += i & (-i)

    def prefix(i):
        s = 0
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        return s

    # Build: initialise the difference array with a[i] - a[i-1].
    prev = 0
    for i in range(1, n + 1):
        cur = int(a[i - 1])
        update(i, cur - prev)
        prev = cur

    out = []
    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'1':
            av = int(data[idx]); bv = int(data[idx + 1]); u = int(data[idx + 2])
            idx += 3
            update(av, u)
            if bv + 1 <= n:
                update(bv + 1, -u)
        else:
            k = int(data[idx]); idx += 1
            out.append(prefix(k))

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
