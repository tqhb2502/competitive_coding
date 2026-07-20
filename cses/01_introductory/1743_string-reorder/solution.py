# String Reorder - CSES 1743
# https://cses.fi/problemset/task/1743
import sys


def main():
    data = sys.stdin.buffer.read().split()
    w = sys.stdout.buffer.write
    if not data:
        w(b"\n")
        return
    s = data[0]
    n = len(s)

    count = [0] * 26
    for ch in s:
        count[ch - 65] += 1

    curmax = max(count)
    # Feasibility: arrangeable iff max frequency <= ceil(n/2) == (n+1)//2.
    if 2 * curmax > n + 1:
        w(b"-1\n")
        return

    # freq[k] = number of letters whose remaining count is exactly k.
    # Lets us keep the current maximum count (curmax) in O(1) per step.
    freq = [0] * (curmax + 2)
    for i in range(26):
        freq[count[i]] += 1

    # active = sorted list of letter indices that still have count > 0.
    active = [i for i in range(26) if count[i] > 0]

    # maxletter = some letter whose count == curmax (validated when needed).
    maxletter = 0
    for i in range(26):
        if count[i] == curmax:
            maxletter = i
            break

    res = bytearray(n)
    prev = -1          # previously placed letter index (-1 = none)
    m = n              # remaining characters to place

    for pos in range(n):
        if 2 * curmax > m:
            # "Tight" state (2*curmax == m+1, m odd): the dominant letter is
            # unique and MUST be placed here, otherwise it will not fit.
            if count[maxletter] != curmax:
                for i in active:
                    if count[i] == curmax:
                        maxletter = i
                        break
            c = maxletter
        else:
            # "Loose" state: any letter different from prev keeps the rest
            # completable, so take the smallest available one != prev.
            c = active[0]
            if c == prev:
                c = active[1]

        res[pos] = 65 + c

        v = count[c]
        count[c] = v - 1
        freq[v] -= 1
        freq[v - 1] += 1
        if v == curmax and freq[curmax] == 0:
            curmax -= 1
        if v == 1:
            active.remove(c)
        prev = c
        m -= 1

    w(bytes(res))
    w(b"\n")


main()
