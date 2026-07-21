# Distinct Values Queries II — CSES 3356
# https://cses.fi/problemset/task/3356
#
# For every position i keep prev[i] = the previous position (< i) that currently
# holds the same value as position i (0 if none). A range [a, b] contains only
# distinct values  <=>  max(prev[i] for i in [a, b]) < a.
#
# Range-max of prev is kept in an iterative (bottom-up) segment tree with point
# assignment (Fenwick cannot do range-max under decreasing point updates).
#
# To recompute prev under point updates we need, for the value at a position,
# its predecessor / successor position sharing the same value.  Every position
# holds exactly one value at a time, so the multiset of (value, position) pairs
# always has exactly n active members.  Compress every (value, position) pair
# that can ever appear (initial array + all updates) into indices ordered by
# (value, position); a pair's neighbours with the SAME value are exactly its
# global neighbours in this order.  A Fenwick/BIT over these compressed indices
# storing present/absent flags supports:
#   - point add (activate / deactivate a pair)
#   - prev_present / next_present via prefix-count + BIT "select" (binary lift).
# All operations are O(log) so the whole thing is O((n + q) log).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    x = [0] * (n + 1)
    for i in range(1, n + 1):
        x[i] = int(data[pos]); pos += 1

    queries = []
    for _ in range(q):
        t = int(data[pos]); pos += 1
        if t == 1:
            k = int(data[pos]); u = int(data[pos + 1]); pos += 2
            queries.append((1, k, u))
        else:
            a = int(data[pos]); b = int(data[pos + 1]); pos += 2
            queries.append((2, a, b))

    # --- compress values (initial + all update targets) ---
    vals = set()
    for i in range(1, n + 1):
        vals.add(x[i])
    for t, a, b in queries:
        if t == 1:
            vals.add(b)          # b == u for update queries
    value_rank = {v: r for r, v in enumerate(sorted(vals))}

    # --- compress every (value, position) pair that can ever be active ---
    N1 = n + 1                   # encode key = value_rank * N1 + position
    cur_enc = [0] * (n + 1)
    encs = set()
    for i in range(1, n + 1):
        e = value_rank[x[i]] * N1 + i
        cur_enc[i] = e
        encs.add(e)
    for t, k, u in queries:
        if t == 1:
            encs.add(value_rank[u] * N1 + k)

    senc = sorted(encs)          # sorted by (value_rank, position)
    M = len(senc)
    key2idx = {}
    k_pos = [0] * (M + 1)        # position stored at a compressed index
    k_vid = [0] * (M + 1)        # value_rank stored at a compressed index
    for i, e in enumerate(senc):
        c = i + 1
        key2idx[e] = c
        k_pos[c] = e % N1
        k_vid[c] = e // N1

    cur_idx = [0] * (n + 1)      # compressed index of the active pair per position
    for i in range(1, n + 1):
        cur_idx[i] = key2idx[cur_enc[i]]

    # --- Fenwick/BIT over compressed indices: present flags + select ---
    bit = [0] * (M + 1)

    def bit_add(i, d, bit=bit, M=M):
        while i <= M:
            bit[i] += d
            i += i & (-i)

    def bit_sum(i, bit=bit):
        s = 0
        while i > 0:
            s += bit[i]
            i -= i & (-i)
        return s

    LOGM = M.bit_length()

    def bit_select(rank, bit=bit, M=M, LOGM=LOGM):
        # index of the rank-th (1-based) present element
        p = 0
        r = rank
        j = LOGM
        while j >= 0:
            nxt = p + (1 << j)
            if nxt <= M and bit[nxt] < r:
                p = nxt
                r -= bit[nxt]
            j -= 1
        return p + 1

    for i in range(1, n + 1):
        bit_add(cur_idx[i], 1)

    def in_val_pred(ix, vid):
        # position of the same-value predecessor of compressed index ix, else 0
        r = bit_sum(ix - 1)
        if r == 0:
            return 0
        j = bit_select(r)
        return k_pos[j] if k_vid[j] == vid else 0

    def in_val_succ(ix, vid, tot):
        # position of the same-value successor of compressed index ix, else 0
        r = bit_sum(ix)
        if r >= tot:
            return 0
        j = bit_select(r + 1)
        return k_pos[j] if k_vid[j] == vid else 0

    # --- initial prev[] ---
    prev_arr = [0] * (n + 1)
    last = {}
    for i in range(1, n + 1):
        v = x[i]
        prev_arr[i] = last.get(v, 0)
        last[v] = i

    # --- iterative segment tree for range max of prev[] ---
    size = 1
    while size < n:
        size <<= 1
    seg = [0] * (2 * size)
    for i in range(n):
        seg[size + i] = prev_arr[i + 1]
    for i in range(size - 1, 0, -1):
        a2 = seg[2 * i]; b2 = seg[2 * i + 1]
        seg[i] = a2 if a2 > b2 else b2

    def seg_update(p, val, seg=seg, size=size):
        i = size + p - 1
        seg[i] = val
        i >>= 1
        while i:
            a2 = seg[2 * i]; b2 = seg[2 * i + 1]
            seg[i] = a2 if a2 > b2 else b2
            i >>= 1

    def seg_query(l, r, seg=seg, size=size):
        res = 0
        l0 = size + l - 1
        r0 = size + r
        while l0 < r0:
            if l0 & 1:
                if seg[l0] > res:
                    res = seg[l0]
                l0 += 1
            if r0 & 1:
                r0 -= 1
                if seg[r0] > res:
                    res = seg[r0]
            l0 >>= 1
            r0 >>= 1
        return res

    out = []
    YES = b"YES"
    NO = b"NO"
    for t, a, b in queries:
        if t == 2:
            out.append(YES if seg_query(a, b) < a else NO)
        else:
            k = a
            u = b
            ov = cur_idx[k]
            nv = key2idx[value_rank[u] * N1 + k]
            if nv == ov:
                continue
            # remove position k from its current value group
            vid_o = k_vid[ov]
            bit_add(ov, -1)
            p_o = in_val_pred(ov, vid_o)
            s_o = in_val_succ(ov, vid_o, n - 1)
            if s_o:
                seg_update(s_o, p_o)
            # insert position k into the new value group
            bit_add(nv, 1)
            vid_n = k_vid[nv]
            p_n = in_val_pred(nv, vid_n)
            s_n = in_val_succ(nv, vid_n, n)
            seg_update(k, p_n)
            if s_n:
                seg_update(s_n, k)
            cur_idx[k] = nv

    sys.stdout.buffer.write(b"\n".join(out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
