# System of Linear Equations - https://cses.fi/problemset/task/3154
#
# Solve n linear equations in m variables modulo p = 1e9+7 (a prime field GF(p)).
# We run Gaussian elimination to row-echelon form, then back-substitute.
#
# Speed trick for pure CPython: pack each row into ONE big Python integer, giving
# every value a fixed-width W-bit "slot" (W = 72 bits = 9 bytes). A whole row
# operation  row_i <- row_i + mult * pivot_row  (mod p per slot) then becomes a
# single big-integer multiply+add executed in C.
#
# We keep the still-active rows "aligned": slot 0 always holds the coefficient of
# the column currently being processed. Reading a coefficient is then just
# `row & MASK` (O(1)), and after eliminating a column we do `>> W` to drop it and
# realign to the next column.
#
# Overflow / carry safety: a pivot row is fully reduced (every slot < p < 2^30)
# before use, so each elimination adds mult*pivot_slot < p*p < 2^60 to a target
# slot. A given slot is accumulated over at most (#pivots) <= 500 < 2^9 steps, so
# it stays below 500*2^60 < 2^69 < 2^72 = 2^W. Hence no slot ever overflows into
# its neighbour (the packing stays valid) and `>> W` removes a finished column
# cleanly. Reduced pivot rows read back their exact residues.
#
# After echelon form we back-substitute (free variables = 0) to get a particular
# solution, then VERIFY it against the original equations: if it satisfies all of
# them we print it, otherwise the system is inconsistent and we print -1.

import sys
from operator import mul


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    MOD = 10 ** 9 + 7
    W = 72
    SB = 9                     # bytes per slot
    MASK = (1 << W) - 1
    ncols = m + 1              # m coefficients + rhs
    rowbytes = ncols * SB

    # ---- read input, keep originals for verification, and pack rows ----
    A = []                     # A[i] = list of m coefficients (mod p)
    B = [0] * n                # rhs (mod p)
    rows = [0] * n             # packed big integers (initially aligned to column 0)
    for i in range(n):
        buf = bytearray(rowbytes)
        off = 0
        coeffs = [0] * m
        for c in range(m):
            v = int(data[pos]) % MOD; pos += 1
            coeffs[c] = v
            buf[off:off + 4] = v.to_bytes(4, 'little')   # v < 2^30 fits in 4 bytes
            off += SB
        b = int(data[pos]) % MOD; pos += 1
        buf[off:off + 4] = b.to_bytes(4, 'little')
        A.append(coeffs)
        B[i] = b
        rows[i] = int.from_bytes(buf, 'little')

    # ---- forward elimination to row-echelon form (rows[r..n-1] kept aligned) ----
    pivcol = [0] * n           # pivcol[pos] = column that pivot row at pos handles
    pivot_tail = [0] * n       # reduced pivot row, aligned to its own pivot column
    r = 0
    for col in range(m):
        ncur = ncols - col     # number of slots in the current tail
        sel = -1
        for i in range(r, n):
            if (rows[i] & MASK) % MOD:
                sel = i
                break
        if sel == -1:
            # free column: just drop it from every active row
            for i in range(r, n):
                rows[i] >>= W
            continue
        rows[r], rows[sel] = rows[sel], rows[r]

        # fully reduce the pivot row so every slot is < p
        prb = rows[r].to_bytes(ncur * SB, 'little')
        out = bytearray(ncur * SB)
        off = 0
        for _ in range(ncur):
            val = int.from_bytes(prb[off:off + SB], 'little') % MOD
            out[off:off + 4] = val.to_bytes(4, 'little')
            off += SB
        pr = int.from_bytes(out, 'little')
        pivot_tail[r] = pr
        pivcol[r] = col

        pc = pr & MASK                     # pivot coefficient, < p, nonzero
        inv_pc = pow(pc, MOD - 2, MOD)

        # eliminate this column from active rows below, then realign them
        for i in range(r + 1, n):
            c = (rows[i] & MASK) % MOD
            if c:
                rows[i] = (rows[i] + (MOD - c * inv_pc % MOD) * pr) >> W
            else:
                rows[i] >>= W

        r += 1
        if r == n:
            break
    rank = r

    # ---- back-substitution (free variables = 0) ----
    x = [0] * m
    for pp in range(rank - 1, -1, -1):
        col = pivcol[pp]
        ncur = ncols - col
        tb = pivot_tail[pp].to_bytes(ncur * SB, 'little')
        # column c (>= col) -> slot (c - col); rhs -> slot (ncur - 1)
        s = 0
        for c in range(col + 1, m):
            xc = x[c]
            if xc:
                o = (c - col) * SB
                coeff = int.from_bytes(tb[o:o + SB], 'little') % MOD
                if coeff:
                    s += coeff * xc
        ob = (ncur - 1) * SB
        b = int.from_bytes(tb[ob:ob + SB], 'little') % MOD
        pc = int.from_bytes(tb[0:SB], 'little') % MOD
        x[col] = (b - s) % MOD * pow(pc, MOD - 2, MOD) % MOD

    # ---- verify against the original system ----
    ok = True
    for i in range(n):
        if sum(map(mul, A[i], x)) % MOD != B[i]:
            ok = False
            break

    if ok:
        sys.stdout.write(' '.join(map(str, x)))
        sys.stdout.write('\n')
    else:
        sys.stdout.write('-1\n')


main()
