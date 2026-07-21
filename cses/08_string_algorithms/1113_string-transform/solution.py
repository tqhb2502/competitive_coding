# String Transform (CSES 1113) - inverse Burrows-Wheeler transform (BWT)
# https://cses.fi/problemset/task/1113
#
# Bài toán: cho chuỗi đã biến đổi L (cột cuối của ma trận các rotation đã sắp xếp
# của s + '#'), khôi phục chuỗi gốc s. Đây chính là inverse BWT với sentinel '#'.
import sys


def main():
    data = sys.stdin.buffer.read()
    L = data.strip()              # transformed string = last column, as bytes
    m = len(L)
    if m == 0:
        return

    # Đếm số lần xuất hiện từng ký tự. Bảng chữ cái: '#'(35) và 'a'..'z'(97..122).
    counts = [0] * 128
    counts[35] = L.count(b'#')
    for c in range(97, 123):
        counts[c] = L.count(bytes([c]))

    # C[c] = số ký tự nhỏ hơn c  -> vị trí bắt đầu của c trong cột đầu F.
    # Thứ tự byte trùng với thứ tự lexicographic (35 = '#' < 97 = 'a').
    C = [0] * 128
    total = 0
    for c in range(128):
        C[c] = total
        total += counts[c]

    # LF mapping: LF[i] = C[L[i]] + (rank của L[i] trong các ký tự bằng nó trước i).
    cnt = C[:]                    # con trỏ ghi hiện tại cho mỗi ký tự
    lf = [0] * m
    for i, c in enumerate(L):
        v = cnt[c]
        lf[i] = v
        cnt[c] = v + 1

    # Đi theo LF bắt đầu từ hàng 0 (hàng bắt đầu bằng '#').
    # Các ký tự L[cur] được ghi ngược từ cuối -> tạo ra T = '#' + s.
    out = bytearray(m)
    cur = 0
    idx = m - 1
    for _ in range(m):
        out[idx] = L[cur]
        idx -= 1
        cur = lf[cur]

    # out[0] == '#'; chuỗi gốc là phần còn lại.
    sys.stdout.buffer.write(out[1:])
    sys.stdout.buffer.write(b"\n")


main()
