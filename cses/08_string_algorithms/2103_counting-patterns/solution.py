# Counting Patterns - CSES 2103
# https://cses.fi/problemset/task/2103
#
# Suffix automaton (SAM) của chuỗi s. Kích thước endpos của mỗi state = số lần
# xuất hiện của các substring thuộc state đó. Với mỗi pattern, đi theo các ký tự
# trong SAM: nếu đi hết thì đáp án = cnt[state]; nếu tắc thì đáp án = 0.
# Thuật toán tất định (không hashing) nên chống anti-hash test của CSES.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    k = int(data[1])
    patterns = data[2:2 + k]

    n = len(s)

    # Suffix automaton (mảng song song cho tốc độ).
    sa_len = [0]
    sa_link = [-1]
    sa_next = [{}]
    cnt = [0]           # cnt[state]: khởi tạo 1 cho state gốc, 0 cho clone
    last = 0

    len_ap = sa_len.append
    link_ap = sa_link.append
    next_ap = sa_next.append
    cnt_ap = cnt.append

    for ch in s:
        c = ch - 97  # 'a' -> 0
        cur = len(sa_len)
        len_ap(sa_len[last] + 1)
        link_ap(-1)
        next_ap({})
        cnt_ap(1)
        p = last
        while p != -1 and c not in sa_next[p]:
            sa_next[p][c] = cur
            p = sa_link[p]
        if p == -1:
            sa_link[cur] = 0
        else:
            q = sa_next[p][c]
            if sa_len[p] + 1 == sa_len[q]:
                sa_link[cur] = q
            else:
                clone = len(sa_len)
                len_ap(sa_len[p] + 1)
                link_ap(sa_link[q])
                next_ap(dict(sa_next[q]))
                cnt_ap(0)  # clone không tính vào endpos trực tiếp
                while p != -1 and sa_next[p].get(c) == q:
                    sa_next[p][c] = clone
                    p = sa_link[p]
                sa_link[q] = clone
                sa_link[cur] = clone
        last = cur

    total = len(sa_len)

    # Counting sort theo len giảm dần để cộng dồn endpos theo suffix link.
    bucket = [0] * (n + 2)
    for v in range(total):
        bucket[sa_len[v]] += 1
    for i in range(1, n + 2):
        bucket[i] += bucket[i - 1]
    order = [0] * total
    for v in range(total):
        L = sa_len[v]
        bucket[L] -= 1
        order[bucket[L]] = v
    # order: tăng dần theo len -> duyệt ngược để xử lý len giảm dần.
    for i in range(total - 1, -1, -1):
        v = order[i]
        pl = sa_link[v]
        if pl != -1:
            cnt[pl] += cnt[v]

    # Trả lời truy vấn.
    out = []
    out_ap = out.append
    for pat in patterns:
        cur = 0
        ok = True
        for ch in pat:
            nx = sa_next[cur].get(ch - 97)
            if nx is None:
                ok = False
                break
            cur = nx
        out_ap(cnt[cur] if ok else 0)

    sys.stdout.write('\n'.join(map(str, out)))
    if out:
        sys.stdout.write('\n')


main()
