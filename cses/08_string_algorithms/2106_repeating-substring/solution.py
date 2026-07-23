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
        # Xâu rỗng hoặc một ký tự không thể có substring lặp lại
        sys.stdout.write("-1\n")
        return

    # --- Nén tọa độ các ký tự ban đầu về 0..d-1 (d = số ký tự phân biệt) ---
    order = sorted(set(s))
    cmap = {c: i for i, c in enumerate(order)}
    rank = [cmap[c] for c in s]

    # --- Xây dựng suffix array bằng prefix doubling ---
    # Mỗi bước gấp đôi độ dài tiền tố so sánh; khóa kết hợp gộp cặp (rank[i], rank[i+k])
    # thành một số nguyên: key[i] = rank[i]*base + (rank[i+k]+1) với base = n+1 > mọi giá trị phụ.
    base = n + 1
    sa = list(range(n))
    k = 1
    while True:
        rank_shift = rank[k:] + [-1] * k
        keys = [a * base + b + 1 for a, b in zip(rank, rank_shift)]
        sa = sorted(range(n), key=keys.__getitem__)

        # Gán lại rank mới: tăng 1 mỗi khi khóa đổi so với hậu tố liền trước trong sa
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
    # Biến h giảm tối đa 1 mỗi bước nên tổng chi phí amortized O(n).
    lcp_max = 0
    lcp_pos = 0
    h = 0
    for i in range(n):
        r = rank[i]
        if r > 0:
            j = sa[r - 1]             # hậu tố kề trước i trong thứ tự từ điển
            while i + h < n and j + h < n and s[i + h] == s[j + h]:
                h += 1
            if h > lcp_max:
                lcp_max = h
                lcp_pos = i
            if h:
                h -= 1
        else:
            h = 0

    # max LCP = 0 nghĩa là không có substring nào lặp lại
    if lcp_max == 0:
        sys.stdout.write("-1\n")
    else:
        sys.stdout.buffer.write(s[lcp_pos:lcp_pos + lcp_max] + b"\n")


solve()
