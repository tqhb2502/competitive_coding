# Permutation Order - https://cses.fi/problemset/task/3397
# Factorial number system: decode k -> permutation (type 1),
# encode permutation -> rank k (type 2). n <= 20.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1

    # Precompute factorials 0..20
    fact = [1] * 21
    for i in range(1, 21):
        fact[i] = fact[i - 1] * i

    out = []
    for _ in range(t):
        qtype = data[idx]; idx += 1
        n = int(data[idx]); idx += 1

        if qtype == b'1':
            k = int(data[idx]); idx += 1
            kk = k - 1  # 0-indexed rank
            available = list(range(1, n + 1))
            res = []
            for i in range(n, 0, -1):
                f = fact[i - 1]
                j = kk // f
                kk %= f
                res.append(available.pop(j))
            out.append(' '.join(map(str, res)))
        else:  # type 2
            perm = [int(data[idx + i]) for i in range(n)]
            idx += n
            available = list(range(1, n + 1))
            rank = 0
            for i in range(n):
                x = perm[i]
                pos = available.index(x)  # available stays sorted
                rank += pos * fact[n - 1 - i]
                available.pop(pos)
            out.append(str(rank + 1))

    sys.stdout.write('\n'.join(out) + '\n')


main()
