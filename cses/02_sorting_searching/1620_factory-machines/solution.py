# Factory Machines - CSES 1620
# https://cses.fi/problemset/task/1620
#
# Binary search on the answer: find the smallest time T such that the machines
# together can make at least t products, i.e. sum(T // k_i) >= t.
#
# Pure standard library only. Two tricks keep it fast in CPython:
#   1. Tight search bounds derived from S = sum(1/k_i): the answer lies in
#      [t/S, (t+n)/S] (the floor loses at most 1 per machine, so at most n
#      total), which is a window of width ~n/S -> at most ~30 iterations.
#   2. The per-check total sum(T // k_i) is computed as
#      sum(map(mid.__floordiv__, k)), which runs the whole loop at C speed.

import sys


def solve(n, t, k):
    # min(k)*t is always a valid upper bound: the fastest machine alone finishes
    # t products in min(k)*t seconds, so f(min(k)*t) >= t.
    cap = min(k) * t

    # S = sum(1/k_i). With EXACT arithmetic, f(T) = sum(T // k_i) satisfies
    #   T*S - n < f(T) <= T*S,  so the answer lies in [t/S, (t+n)/S], a window
    # of width ~n/S -> only ~30 binary-search steps. S is computed in floating
    # point, so these are used only as a fast hint: the exact integer guards
    # below make the final bracket provably correct regardless of fp error.
    S = 0.0
    for x in k:
        S += 1.0 / x

    # Generous relative pad (1e-6) absorbs the floating-point error of S (which,
    # for n up to 2*10^5, is many orders of magnitude smaller) so that the hint
    # bracket stays tight in the common case.
    lo = int(t / S * (1.0 - 1e-6)) - 5
    if lo < 1:
        lo = 1
    hi = int((t + n) / S * (1.0 + 1e-6)) + 5
    if hi > cap or hi < 1:
        hi = cap

    # Exact integer guards: guarantee the true answer A (smallest T with
    # f(T) >= t) satisfies lo <= A <= hi, so correctness never depends on the
    # accuracy of the float hint.
    #   * A <= hi  iff  f(hi) >= t.   If not, widen hi to the guaranteed cap.
    #   * A >= lo  iff  f(lo-1) < t.  If not, lo overshot the answer; reset to 1.
    if sum(map(hi.__floordiv__, k)) < t:
        hi = cap
    if lo > 1 and sum(map((lo - 1).__floordiv__, k)) >= t:
        lo = 1
    if lo > hi:            # numerical safety fallback: guaranteed valid bracket
        lo = 1

    # Binary search for the smallest T with sum(T // k_i) >= t.
    while lo < hi:
        mid = (lo + hi) >> 1
        if sum(map(mid.__floordiv__, k)) >= t:
            hi = mid
        else:
            lo = mid + 1

    return lo


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    t = int(data[1])
    k = list(map(int, data[2:2 + n]))
    sys.stdout.write(str(solve(n, t, k)) + "\n")


if __name__ == "__main__":
    main()
