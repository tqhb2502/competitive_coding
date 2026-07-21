# Josephus Queries - CSES 2164
# https://cses.fi/problemset/task/2164
#
# n children stand in a circle, numbered 1..n. Starting the count before child 1,
# every second child is removed (so the first removed is child 2). For each query
# (n, k) print which child is the k-th one to be removed.
import sys


def solve(n, k):
    # Return the 1-indexed position of the k-th eliminated child.
    # We reduce the problem: in the first pass around the circle the children at
    # even positions 2,4,...,2*floor(n/2) are removed. If the wanted k falls in
    # this first pass the answer is 2*k. Otherwise the survivors are exactly the
    # odd positions and we recurse on the smaller circle, keeping a linear map
    # answer = A + B * (subproblem answer).
    A = 0
    B = 1
    while True:
        m = n // 2                      # removals during the first pass
        if k <= m:                      # k-th removal happens in the first pass
            return A + B * (2 * k)
        r = k - m                       # index inside the smaller (odd) circle
        if n % 2 == 0:
            # Survivors 1,3,...,n-1 (m of them). Next removed is 3 -> a fresh
            # "skip first, remove second" problem on m children.
            # original_pos = 2 * sub - 1  ->  A -= B, B *= 2
            A -= B
            B *= 2
            n = m
            k = r
        else:
            # Survivors 1,3,...,n (m+1 of them). The last child n is skipped, then
            # child 1 is removed immediately.
            if r == 1:
                return A + B            # child 1 (position -> 2*1-1 = 1)
            # After removing child 1 it becomes a fresh "skip first, remove second"
            # problem on the remaining m children (positions 3,5,...,n).
            # original_pos = 2 * (1 + sub) - 1 = 1 + 2 * sub  ->  A += B, B *= 2
            A += B
            B *= 2
            n = m
            k = r - 1


def main():
    data = sys.stdin.buffer.read().split()
    q = int(data[0])
    out = []
    idx = 1
    for _ in range(q):
        n = int(data[idx]); k = int(data[idx + 1]); idx += 2
        out.append(str(solve(n, k)))
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
