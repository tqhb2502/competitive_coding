# Subarray Sum Queries II
# https://cses.fi/problemset/task/3226
#
# Range maximum-subarray-sum queries (empty subarray with sum 0 allowed).
# Iterative (bottom-up) segment tree. Each node stores 4 values:
#   sum  = total of the segment
#   pre  = max prefix sum  (>= 0, empty segment allowed)
#   suf  = max suffix sum  (>= 0)
#   best = max subarray sum(>= 0)
# Merge of two nodes (non-commutative):
#   sum  = L.sum + R.sum
#   pre  = max(L.pre, L.sum + R.pre)
#   suf  = max(R.suf, R.sum + L.suf)
#   best = max(L.best, R.best, L.suf + R.pre)

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    size = n
    SUM = [0] * (2 * size)
    PRE = [0] * (2 * size)
    SUF = [0] * (2 * size)
    BEST = [0] * (2 * size)

    # Leaves
    for i in range(n):
        x = int(data[pos]); pos += 1
        p = x if x > 0 else 0
        j = size + i
        SUM[j] = x
        PRE[j] = p
        SUF[j] = p
        BEST[j] = p

    # Build internal nodes
    for i in range(size - 1, 0, -1):
        l = 2 * i
        r = l + 1
        ls = SUM[l]; rs = SUM[r]
        lp = PRE[l]; rp = PRE[r]
        lsuf = SUF[l]; rsuf = SUF[r]
        lb = BEST[l]; rb = BEST[r]
        SUM[i] = ls + rs
        t = ls + rp
        PRE[i] = lp if lp > t else t
        t = rs + lsuf
        SUF[i] = rsuf if rsuf > t else t
        cross = lsuf + rp
        m = lb if lb > rb else rb
        BEST[i] = m if m > cross else cross

    sum_ = SUM; pre_ = PRE; suf_ = SUF; best_ = BEST  # local binds (hot)

    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        l = size + a - 1
        r = size + b          # half-open right bound
        # left accumulator (merge from the left) and right accumulator
        Lsum = Lpre = Lsuf = Lbest = 0
        Rsum = Rpre = Rsuf = Rbest = 0
        while l < r:
            if l & 1:
                nsum = sum_[l]; npre = pre_[l]; nsuf = suf_[l]; nbest = best_[l]
                # merge(L_acc, node)
                t = Lsum + npre
                cpre = Lpre if Lpre > t else t
                t = nsum + Lsuf
                csuf = nsuf if nsuf > t else t
                cross = Lsuf + npre
                m = Lbest if Lbest > nbest else nbest
                Lbest = m if m > cross else cross
                Lsum = Lsum + nsum
                Lpre = cpre
                Lsuf = csuf
                l += 1
            if r & 1:
                r -= 1
                nsum = sum_[r]; npre = pre_[r]; nsuf = suf_[r]; nbest = best_[r]
                # merge(node, R_acc)
                t = nsum + Rpre
                cpre = npre if npre > t else t
                t = Rsum + nsuf
                csuf = Rsuf if Rsuf > t else t
                cross = nsuf + Rpre
                m = nbest if nbest > Rbest else Rbest
                Rbest = m if m > cross else cross
                Rsum = nsum + Rsum
                Rpre = cpre
                Rsuf = csuf
            l >>= 1
            r >>= 1
        # final merge(L_acc, R_acc) -> only best is needed
        cross = Lsuf + Rpre
        m = Lbest if Lbest > Rbest else Rbest
        ap(m if m > cross else cross)

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
