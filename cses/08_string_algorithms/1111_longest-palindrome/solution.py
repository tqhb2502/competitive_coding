# Longest Palindrome - CSES 1111
# https://cses.fi/problemset/task/1111
# Manacher's algorithm, O(n). Deterministic (no hashing).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # bytes, chars a-z (97..122)
    n = len(s)
    if n == 0:
        return

    # Transformed array with '#' separators (value 0) and two distinct guards.
    # Full layout:  [guard1] # c0 # c1 # ... # c(n-1) # [guard2]
    # '#' string length m = 2n+1; full length m+2. Char c_i sits at full index 2i+2.
    m = 2 * n + 1
    t = bytearray(m + 2)          # separators are already 0
    t[0] = 1                      # left guard (unique)
    t[m + 1] = 2                  # right guard (unique)
    t[2:2 * n + 1:2] = s          # place original chars at even positions

    P = [0] * (m + 2)             # radius per center (full-array indexing)

    center = 0
    right = 0
    best_len = 0
    best_center = 1

    tl = t  # local aliases for speed
    Pl = P
    for i in range(1, m + 1):
        if i < right:
            mir = 2 * center - i
            r = right - i
            pm = Pl[mir]
            p = pm if pm < r else r
        else:
            p = 0
        # expand around i (guards guarantee termination without bounds checks)
        while tl[i + p + 1] == tl[i - p - 1]:
            p += 1
        Pl[i] = p
        if i + p > right:
            center = i
            right = i + p
        if p > best_len:
            best_len = p
            best_center = i

    start = (best_center - 1 - best_len) // 2
    sys.stdout.buffer.write(s[start:start + best_len] + b"\n")


main()
