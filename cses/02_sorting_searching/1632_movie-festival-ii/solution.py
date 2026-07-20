# Movie Festival II - CSES 1632
# https://cses.fi/problemset/task/1632
#
# Greedy: sort movies by ending time. Maintain a multiset of the k members'
# current last-end times (initially all 0). For each movie (a, b) in order of
# increasing b, assign it to the member whose last-end time is the LARGEST value
# that is still <= a (leaving members with smaller end times free for movies that
# start earlier). We implement the ordered multiset with a Fenwick (BIT) tree
# over coordinate-compressed end values, using a "find k-th occupied" query.

import sys
from bisect import bisect_right, bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); k = int(data[pos + 1]); pos += 2

    a_list = [0] * n
    b_list = [0] * n
    coords = [0]  # value 0 is the initial end time of every member
    for i in range(n):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        a_list[i] = a
        b_list[i] = b
        coords.append(b)

    # Process movies in order of increasing ending time.
    order = sorted(range(n), key=lambda i: b_list[i])

    # Coordinate compression over all multiset-possible values ({0} U {b_i}).
    V = sorted(set(coords))
    m = len(V)

    tree = [0] * (m + 1)

    # Initialize: k members all at value 0, which is V[0] -> Fenwick index 1.
    i = 1
    while i <= m:
        tree[i] += k
        i += i & (-i)

    # Largest power of two with (1 << LOG) <= m, for the find-k-th binary lifting.
    LOG = 0
    while (1 << (LOG + 1)) <= m:
        LOG += 1

    count = 0
    for oi in order:
        a = a_list[oi]
        b = b_list[oi]

        # p = number of compressed values <= a; these are Fenwick indices 1..p.
        p = bisect_right(V, a)

        # s = how many members currently have end time <= a (occupied slots in 1..p).
        s = 0
        i = p
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        if s == 0:
            continue  # no free member with last-end <= a

        # find_kth(s): smallest index whose prefix sum >= s. Because exactly s
        # occupied slots lie in [1..p], this is the LARGEST occupied index <= p,
        # i.e. the member with the greatest end time that is still <= a.
        idx = 0
        rem = s
        pw = LOG
        while pw >= 0:
            nxt = idx + (1 << pw)
            if nxt <= m and tree[nxt] < rem:
                idx = nxt
                rem -= tree[nxt]
            pw -= 1
        idx += 1

        # Remove that member's old end time.
        i = idx
        while i <= m:
            tree[i] -= 1
            i += i & (-i)

        # Insert the new end time b for that member.
        bidx = bisect_left(V, b) + 1
        i = bidx
        while i <= m:
            tree[i] += 1
            i += i & (-i)

        count += 1

    sys.stdout.write(str(count) + "\n")


main()
