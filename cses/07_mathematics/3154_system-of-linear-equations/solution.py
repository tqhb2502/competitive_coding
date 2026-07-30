import sys
from operator import mul


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    MOD = 10 ** 9 + 7
    # Mỗi giá trị chiếm một "slot" rộng W = 72 bit (9 byte) trong big integer.
    W = 72
    SB = 9                     # số byte mỗi slot
    MASK = (1 << W) - 1
    ncols = m + 1              # m hệ số + vế phải
    rowbytes = ncols * SB

    # ---- Đọc input, giữ bản gốc để kiểm chứng, và đóng gói (pack) từng hàng ----
    # Mỗi hàng gồm m hệ số + vế phải được nén thành MỘT số nguyên lớn, ban đầu
    # căn chỉnh (aligned) sao cho slot 0 ứng với cột 0.
    A = []                     # A[i] = danh sách m hệ số (mod p)
    B = [0] * n                # vế phải (mod p)
    rows = [0] * n             # các hàng đã đóng gói (aligned về cột 0)
    for i in range(n):
        buf = bytearray(rowbytes)
        off = 0
        coeffs = [0] * m
        for c in range(m):
            v = int(data[pos]) % MOD; pos += 1
            coeffs[c] = v
            buf[off:off + 4] = v.to_bytes(4, 'little')   # v < 2^30 vừa 4 byte
            off += SB
        b = int(data[pos]) % MOD; pos += 1
        buf[off:off + 4] = b.to_bytes(4, 'little')
        A.append(coeffs)
        B[i] = b
        rows[i] = int.from_bytes(buf, 'little')

    # ---- Khử tiến đưa về dạng bậc thang (rows[r..n-1] luôn được căn chỉnh) ----
    pivcol = [0] * n           # pivcol[pos] = cột mà hàng pivot tại pos xử lý
    pivot_tail = [0] * n       # hàng pivot đã reduce, aligned về cột pivot của nó
    r = 0
    for col in range(m):
        ncur = ncols - col     # số slot còn lại trong phần đuôi hiện tại
        # Tìm hàng chưa dùng có hệ số tại cột này khác 0 (mod p) làm pivot.
        sel = -1
        for i in range(r, n):
            if (rows[i] & MASK) % MOD:
                sel = i
                break
        if sel == -1:
            # Cột tự do (free column): chỉ cần bỏ nó khỏi mọi hàng đang hoạt động.
            for i in range(r, n):
                rows[i] >>= W
            continue
        rows[r], rows[sel] = rows[sel], rows[r]

        # Reduce đầy đủ hàng pivot để mọi slot đều < p (an toàn tràn slot).
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

        pc = pr & MASK                     # hệ số pivot, < p, khác 0
        inv_pc = pow(pc, MOD - 2, MOD)     # nghịch đảo modular (Fermat)

        # Khử cột này khỏi các hàng hoạt động phía dưới, rồi căn chỉnh lại (>> W).
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

    # ---- Thế ngược (back-substitution), gán biến tự do = 0 ----
    x = [0] * m
    for pp in range(rank - 1, -1, -1):
        col = pivcol[pp]
        ncur = ncols - col
        tb = pivot_tail[pp].to_bytes(ncur * SB, 'little')
        # cột c (>= col) -> slot (c - col); vế phải -> slot cuối (ncur - 1).
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

    # ---- Kiểm chứng nghiệm dựng ra trên HỆ GỐC ----
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
