# Sum of Two Values - https://cses.fi/problemset/task/1640
# Duyệt một lượt, dùng hash map lưu {giá trị -> vị trí 1-based} đã gặp.
# Với mỗi a[i], nếu (x - a[i]) đã có trong map thì in ra cặp vị trí.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    seen = {}
    out = "IMPOSSIBLE"
    # data[2 .. 2+n-1] là các phần tử của mảng
    for i in range(n):
        a = int(data[2 + i])
        need = x - a
        if need in seen:
            out = str(seen[need]) + " " + str(i + 1)
            break
        if a not in seen:
            seen[a] = i + 1
    sys.stdout.write(out + "\n")


main()
