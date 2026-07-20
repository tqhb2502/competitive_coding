# Restaurant Customers - CSES 1619
# https://cses.fi/problemset/task/1619
# Sweep line: sort arrivals and departures, merge with a two-pointer,
# track the peak number of simultaneous customers.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])

    arrivals = [0] * n
    departures = [0] * n
    idx = 1
    for i in range(n):
        arrivals[i] = int(data[idx])
        departures[i] = int(data[idx + 1])
        idx += 2

    arrivals.sort()
    departures.sort()

    best = 0
    cur = 0
    i = 0  # pointer into arrivals
    j = 0  # pointer into departures
    # Merge the two sorted event lists. Times are all distinct, so ties
    # cannot happen; the comparison direction does not matter.
    while i < n:
        if arrivals[i] < departures[j]:
            cur += 1
            if cur > best:
                best = cur
            i += 1
        else:
            cur -= 1
            j += 1

    sys.stdout.write(str(best) + "\n")


if __name__ == "__main__":
    main()
