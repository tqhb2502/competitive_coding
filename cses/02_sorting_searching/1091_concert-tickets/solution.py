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

    # Nén tọa độ: các mức giá vé phân biệt, sắp tăng dần, gán chỉ số 1-based.
    unique = sorted(set(h))
    K = len(unique)
    rank = {v: i + 1 for i, v in enumerate(unique)}

    # Fenwick tree (BIT) lưu số lượng vé còn lại tại mỗi chỉ số nén.
    tree = [0] * (K + 1)

    def update(pos, delta):
        while pos <= K:
            tree[pos] += delta
            pos += pos & (-pos)

    # Tổng tiền tố: số vé còn lại có chỉ số giá <= pos.
    def prefix(pos):
        s = 0
        while pos > 0:
            s += tree[pos]
            pos -= pos & (-pos)
        return s

    # find_kth (binary lifting): chỉ số nhỏ nhất có prefix sum >= k.
    LOG = K.bit_length()

    def find_kth(k):
        pos = 0
        for i in range(LOG, -1, -1):
            nxt = pos + (1 << i)
            if nxt <= K and tree[nxt] < k:
                pos = nxt
                k -= tree[nxt]
        return pos + 1

    # Ban đầu đưa toàn bộ vé vào BIT.
    for v in h:
        update(rank[v], 1)

    out = []
    for budget in t:
        # pos_idx: số mức giá <= ngân sách (mọi chỉ số 1..pos_idx).
        pos_idx = bisect_right(unique, budget)
        if pos_idx == 0:
            out.append("-1")
            continue
        # cnt: số vé còn lại có giá <= ngân sách.
        cnt = prefix(pos_idx)
        if cnt == 0:
            out.append("-1")  # Không còn vé phù hợp.
        else:
            # Vé thứ hạng cnt là vé giá cao nhất còn lại <= ngân sách; bán rồi xóa.
            j = find_kth(cnt)
            out.append(str(unique[j - 1]))
            update(j, -1)

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
