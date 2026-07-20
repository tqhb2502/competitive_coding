# Minimal Grid Path - CSES 3359
# https://cses.fi/problemset/task/3359
#
# Đi từ góc trái-trên (0,0) đến góc phải-dưới (n-1,n-1), mỗi bước sang phải
# hoặc xuống dưới, tìm xâu nhỏ nhất theo thứ tự từ điển (độ dài 2n-1).
#
# Ý tưởng: duyệt theo từng đường chéo phụ (anti-diagonal) r+c = d.
# Giữ "frontier" = tập các ô trên đường chéo d mà prefix của nó là nhỏ nhất.
# Chuyển sang đường chéo d+1: sinh các ô ứng viên (đi phải/xuống từ frontier),
# chọn ký tự nhỏ nhất, chỉ giữ lại các ô có ký tự đó.
#
# Toàn bộ các phép trên được làm theo lô (bulk) ở mức C: biểu diễn frontier
# bằng chuỗi byte 0/1 (một byte / một hàng), dịch xuống bằng phép >> 8 trên
# số nguyên lớn, và dùng bytes.translate / min để tìm và lọc ký tự min.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    lines = data[1:1 + n]
    big = b''.join(lines)  # lưới theo hàng (row-major), độ dài n*n

    out = sys.stdout
    if n == 1:
        out.write(big[:1].decode() + "\n")
        return

    step = n - 1
    total = 2 * n - 1

    # frontier: bytes 0/1 trên các hàng 0..n-1; bắt đầu tại hàng 0 (ô (0,0))
    tmp = bytearray(n)
    tmp[0] = 1
    flag = bytes(tmp)

    res = bytearray()
    res.append(big[0])  # grid[0][0]

    T0 = bytes([255, 0] + [0] * 254)  # 0 -> 255 (không phải ứng viên), 1 -> 0

    for d in range(1, total):
        lo = d - (n - 1)
        if lo < 0:
            lo = 0
        hi = d
        if hi > n - 1:
            hi = n - 1
        w = hi - lo + 1

        # ứng viên = frontier (đi phải, giữ hàng r) | dịch xuống (đi xuống, hàng r-1 -> r)
        F = int.from_bytes(flag, 'big')          # byte hàng 0 là MSB
        cand = (F | (F >> 8)).to_bytes(n, 'big')  # >> 8: hàng r nhận flag[r-1]
        candwin = cand[lo:hi + 1]                 # cửa sổ hàng hợp lệ [lo, hi]

        # ký tự trên đường chéo d cho các hàng lo..hi: chỉ số = start + k*step
        start = d + lo * step
        diag = big[start: start + (w - 1) * step + 1: step]

        # đặt các ô không phải ứng viên thành 255 (lớn hơn mọi chữ cái)
        noncand = candwin.translate(T0)
        masked = (int.from_bytes(diag, 'big') |
                  int.from_bytes(noncand, 'big')).to_bytes(w, 'big')
        mn = min(masked)
        res.append(mn)

        # frontier mới: các vị trí có ký tự == mn
        tb = bytearray(256)
        tb[mn] = 1
        newwin = masked.translate(bytes(tb))
        flag = b'\x00' * lo + newwin + b'\x00' * (n - 1 - hi)

    out.write(res.decode() + "\n")


main()
