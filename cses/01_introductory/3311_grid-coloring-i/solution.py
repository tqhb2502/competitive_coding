# Grid Coloring I - https://cses.fi/problemset/task/3311
# Đổi màu mỗi ô để: khác ô trên, khác ô trái, và khác chính nó.
# Tối đa 3 màu bị cấm trong số 4 màu => luôn còn ít nhất 1 màu hợp lệ.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    rows = data[2:2 + n]  # mỗi phần tử là bytes độ dài m (chỉ gồm A,B,C,D)

    colors = b'ABCD'
    out = []
    prev_row = None  # hàng trước đã tô (bytearray các mã ASCII)

    for i in range(n):
        orig = rows[i]
        cur = bytearray(m)
        for j in range(m):
            forbid_o = orig[j]                    # khác màu gốc
            up = prev_row[j] if prev_row is not None else 0
            left = cur[j - 1] if j > 0 else 0     # ô trái vừa tô
            for c in colors:
                if c != forbid_o and c != up and c != left:
                    cur[j] = c
                    break
        out.append(cur.decode())
        prev_row = cur

    sys.stdout.write('\n'.join(out))
    sys.stdout.write('\n')


main()
