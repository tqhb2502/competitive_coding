# Playlist - https://cses.fi/problemset/task/1141
# Two-pointer (sliding window): longest subarray with all distinct ids.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        print(0)
        return
    n = int(data[0])
    a = data[1:1 + n]

    last = {}          # id -> chỉ số xuất hiện gần nhất
    left = 0           # biên trái của cửa sổ hiện tại
    best = 0
    for right in range(n):
        x = a[right]
        prev = last.get(x)
        if prev is not None and prev >= left:
            left = prev + 1
        last[x] = right
        length = right - left + 1
        if length > best:
            best = length

    sys.stdout.write(str(best) + "\n")


main()
