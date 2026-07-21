# Repeating Substring
# https://cses.fi/problemset/task/2106
#
# Tìm substring dài nhất xuất hiện ít nhất hai lần trong xâu S.
# Dùng suffix array (prefix doubling O(n log n)) + LCP array (Kasai).
# Đáp án = giá trị LCP lớn nhất giữa hai suffix kề nhau trong suffix array;
# nếu bằng 0 thì không có substring lặp lại -> in ra -1.

import sys
from itertools import accumulate


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("-1\n")
        return

    s = data[0]                      # làm việc trực tiếp trên bytes cho nhanh
    n = len(s)
    if n <= 1:
        sys.stdout.write("-1\n")
        return

    # --- Nén tọa độ các ký tự ban đầu về 0..d-1 (d = số ký tự phân biệt) ---
    order = sorted(set(s))
    cmap = {c: i for i, c in enumerate(order)}
    rank = [cmap[c] for c in s]

    # --- Suffix array bằng prefix doubling ---
    # Khóa kết hợp: key[i] = rank[i]*base + (rank[i+k]+1), base = n+1 > mọi giá trị phụ.
    base = n + 1
    sa = list(range(n))
    k = 1
    while True:
        rank_shift = rank[k:] + [-1] * k
        keys = [a * base + b + 1 for a, b in zip(rank, rank_shift)]
        sa = sorted(range(n), key=keys.__getitem__)

        sk = [keys[i] for i in sa]
        diff = [0] + [1 if sk[i] != sk[i - 1] else 0 for i in range(1, n)]
        rnk_sorted = list(accumulate(diff))       # rank mới theo thứ tự trong sa

        newrank = [0] * n
        for p, rv in zip(sa, rnk_sorted):
            newrank[p] = rv
        rank = newrank

        if rnk_sorted[-1] == n - 1:               # tất cả rank phân biệt -> sa đã đúng
            break
        k <<= 1

    # --- LCP array bằng thuật toán Kasai, tìm LCP kề nhau lớn nhất ---
    lcp_max = 0
    lcp_pos = 0
    h = 0
    for i in range(n):
        r = rank[i]
        if r > 0:
            j = sa[r - 1]
            while i + h < n and j + h < n and s[i + h] == s[j + h]:
                h += 1
            if h > lcp_max:
                lcp_max = h
                lcp_pos = i
            if h:
                h -= 1
        else:
            h = 0

    if lcp_max == 0:
        sys.stdout.write("-1\n")
    else:
        sys.stdout.buffer.write(s[lcp_pos:lcp_pos + lcp_max] + b"\n")


solve()
