# Palindrome Queries - CSES 2420
# https://cses.fi/problemset/task/2420
#
# Point updates + palindrome range queries.
# Double polynomial hashing over two Fenwick trees (forward + backward)
# with randomized bases to resist CSES anti-hash tests.
#
# A substring [a,b] is a palindrome iff its forward reading hash equals its
# backward reading hash. Storing val[i]*p^(i-1) in the forward tree and
# val[i]*p^(n-i) in the backward tree, the check (avoiding modular inverse) is:
#     Fsum(a,b) * p^(n-b)  ==  Bsum(a,b) * p^(a-1)   (mod M)

import sys
import random


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1
    s = data[pos]; pos += 1

    M1 = 1_000_000_007
    M2 = 998_244_353
    b1 = random.randrange(256, M1 - 1)
    b2 = random.randrange(256, M2 - 1)

    # Precompute powers of the bases up to n.
    P1 = [1] * (n + 1)
    P2 = [1] * (n + 1)
    for i in range(1, n + 1):
        P1[i] = P1[i - 1] * b1 % M1
        P2[i] = P2[i - 1] * b2 % M2

    # Character values 1..26 (so 'a' != 0).
    val = [0] * (n + 1)
    for i in range(1, n + 1):
        val[i] = s[i - 1] - 96

    # Four Fenwick trees: forward/backward for each modulus.
    F1 = [0] * (n + 1)
    F2 = [0] * (n + 1)
    B1 = [0] * (n + 1)
    B2 = [0] * (n + 1)
    for i in range(1, n + 1):
        v = val[i]
        F1[i] = v * P1[i - 1] % M1
        F2[i] = v * P2[i - 1] % M2
        B1[i] = v * P1[n - i] % M1
        B2[i] = v * P2[n - i] % M2
    # O(n) Fenwick build (propagate to parents).
    for i in range(1, n + 1):
        j = i + (i & -i)
        if j <= n:
            F1[j] = (F1[j] + F1[i]) % M1
            F2[j] = (F2[j] + F2[i]) % M2
            B1[j] = (B1[j] + B1[i]) % M1
            B2[j] = (B2[j] + B2[i]) % M2

    out = []
    ap = out.append

    for _ in range(m):
        t = data[pos]
        if t == b'2':
            a = int(data[pos + 1])
            b = int(data[pos + 2])
            pos += 3
            # prefix at b
            x = b
            f1 = f2 = g1 = g2 = 0
            while x > 0:
                f1 += F1[x]; f2 += F2[x]; g1 += B1[x]; g2 += B2[x]
                x -= x & -x
            # prefix at a-1
            x = a - 1
            pf1 = pf2 = pg1 = pg2 = 0
            while x > 0:
                pf1 += F1[x]; pf2 += F2[x]; pg1 += B1[x]; pg2 += B2[x]
                x -= x & -x
            Fs1 = (f1 - pf1) % M1
            Fs2 = (f2 - pf2) % M2
            Gs1 = (g1 - pg1) % M1
            Gs2 = (g2 - pg2) % M2
            if (Fs1 * P1[n - b] % M1 == Gs1 * P1[a - 1] % M1 and
                    Fs2 * P2[n - b] % M2 == Gs2 * P2[a - 1] % M2):
                ap('YES')
            else:
                ap('NO')
        else:
            k = int(data[pos + 1])
            nv = data[pos + 2][0] - 96
            pos += 3
            ov = val[k]
            if nv != ov:
                d = nv - ov
                val[k] = nv
                d1f = d * P1[k - 1] % M1
                d2f = d * P2[k - 1] % M2
                d1b = d * P1[n - k] % M1
                d2b = d * P2[n - k] % M2
                i = k
                while i <= n:
                    F1[i] = (F1[i] + d1f) % M1
                    F2[i] = (F2[i] + d2f) % M2
                    B1[i] = (B1[i] + d1b) % M1
                    B2[i] = (B2[i] + d2b) % M2
                    i += i & -i

    sys.stdout.write('\n'.join(out))
    if out:
        sys.stdout.write('\n')


main()
