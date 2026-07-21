# Next Prime - https://cses.fi/problemset/task/3396
# For each n (n <= 10^12) find the smallest prime strictly greater than n.
# Uses deterministic Miller-Rabin primality test (bases valid for n < 3.3e24).

import sys

# Deterministic Miller-Rabin witnesses: correct for all n < 3.3 * 10^24.
_MR_BASES = (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37)


def is_prime(n):
    if n < 2:
        return False
    # small primes / quick divisibility
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % p == 0:
            return n == p
    # write n - 1 = d * 2^s with d odd
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for a in _MR_BASES:
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1:
                break
        else:
            return False
    return True


def next_prime(n):
    # smallest prime strictly greater than n
    if n < 2:
        return 2
    cand = n + 1
    if cand == 2:
        return 2
    if cand % 2 == 0:
        cand += 1  # move to first odd candidate (> 2)
    while not is_prime(cand):
        cand += 2
    return cand


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    out = []
    for i in range(1, t + 1):
        n = int(data[i])
        out.append(str(next_prime(n)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
