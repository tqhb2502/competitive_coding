# Static Range Sum Queries - CSES 1646
# https://cses.fi/problemset/task/1646
# Approach: prefix sums. Precompute prefix[i] = x[1]+...+x[i], then each
# query (a, b) answers prefix[b] - prefix[a-1] in O(1).

import sys
from itertools import accumulate


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    x = data[idx:idx + n]
    idx += n
    # prefix sum with a leading 0: prefix[0] = 0, prefix[i] = x[1]+...+x[i]
    prefix = list(accumulate((int(v) for v in x), initial=0))

    out = []
    out_append = out.append
    pref = prefix
    for _ in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        out_append(str(pref[b] - pref[a - 1]))

    sys.stdout.buffer.write(("\n".join(out) + "\n").encode())


if __name__ == "__main__":
    main()
