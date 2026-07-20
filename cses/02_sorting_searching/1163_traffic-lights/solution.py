# Traffic Lights - CSES 1163
# https://cses.fi/problemset/task/1163
#
# Offline / reverse approach: instead of adding lights (which SPLITS a gap and
# can decrease the max), we start from the full configuration and REMOVE lights
# in reverse order of addition. Each removal MERGES two adjacent gaps into a
# larger one, so the maximum gap is monotonically non-decreasing and we only
# need to track a single running maximum. A doubly-linked list gives O(1)
# neighbor lookup and O(1) deletion.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    x = int(data[0])
    n = int(data[1])
    p = [int(data[2 + i]) for i in range(n)]

    # Sorted coordinates: boundaries 0 and x plus all light positions.
    coords = sorted(p)
    coords = [0] + coords + [x]
    m = len(coords)  # = n + 2

    # Map each light position to its index in coords (0 and x are the boundaries).
    pos_to_idx = {}
    for i in range(m):
        pos_to_idx[coords[i]] = i

    # Doubly-linked list over indices of coords.
    left = list(range(-1, m - 1))   # left[i]  = i - 1
    right = list(range(1, m + 1))   # right[i] = i + 1
    # right[m-1] = m is out of range but never used (last node never removed).

    # Full configuration: max distance between consecutive coords = answer A[n].
    current_max = 0
    for i in range(1, m):
        d = coords[i] - coords[i - 1]
        if d > current_max:
            current_max = d

    ans = [0] * n
    ans[n - 1] = current_max  # state with all n lights

    # Remove lights p_n, p_{n-1}, ..., p_2. Removing p_k yields state S_{k-1}.
    for k in range(n, 1, -1):
        node = pos_to_idx[p[k - 1]]
        L = left[node]
        R = right[node]
        # Unlink node.
        right[L] = R
        left[R] = L
        new_gap = coords[R] - coords[L]
        if new_gap > current_max:
            current_max = new_gap
        ans[k - 2] = current_max  # state S_{k-1}

    sys.stdout.write(" ".join(map(str, ans)))
    sys.stdout.write("\n")


main()
