# Concert Tickets - https://cses.fi/problemset/task/1091
# Fenwick tree (BIT) lưu số lượng vé còn lại + binary lifting (find_kth).
# Với mỗi khách: tìm vé giá cao nhất không vượt quá ngân sách, rồi xóa vé đó.

import sys
from bisect import bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    h = data[idx:idx + n]
    idx += n
    t = data[idx:idx + m]
    idx += m

    h = [int(x) for x in h]
    t = [int(x) for x in t]

    # Coordinate compression của các giá vé.
    unique = sorted(set(h))
    K = len(unique)
    rank = {v: i + 1 for i, v in enumerate(unique)}  # 1-based

    # Fenwick tree lưu số lượng vé còn lại tại mỗi chỉ số nén.
    tree = [0] * (K + 1)

    def update(pos, delta):
        while pos <= K:
            tree[pos] += delta
            pos += pos & (-pos)

    def prefix(pos):
        s = 0
        while pos > 0:
            s += tree[pos]
            pos -= pos & (-pos)
        return s

    # find_kth: chỉ số nhỏ nhất có prefix sum >= k (binary lifting).
    LOG = K.bit_length()

    def find_kth(k):
        pos = 0
        for i in range(LOG, -1, -1):
            nxt = pos + (1 << i)
            if nxt <= K and tree[nxt] < k:
                pos = nxt
                k -= tree[nxt]
        return pos + 1

    for v in h:
        update(rank[v], 1)

    out = []
    for budget in t:
        pos_idx = bisect_right(unique, budget)  # số lượng giá <= budget
        if pos_idx == 0:
            out.append("-1")
            continue
        cnt = prefix(pos_idx)  # số vé còn lại có giá <= budget
        if cnt == 0:
            out.append("-1")
        else:
            j = find_kth(cnt)  # vị trí vé giá cao nhất còn lại <= budget
            out.append(str(unique[j - 1]))
            update(j, -1)

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
