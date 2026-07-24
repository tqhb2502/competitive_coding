import sys
from bisect import bisect_left


def solve():
    data = list(map(int, sys.stdin.buffer.read().split()))
    if not data:
        return

    n = data[0]
    books = []

    idx = 1
    for _ in range(n):
        a = data[idx]
        b = data[idx + 1]
        idx += 2

        if a >= b:
            L, S = a, b
        else:
            L, S = b, a

        # Dùng số âm để sort tăng tương đương với sort giảm theo L, rồi giảm theo S.
        books.append((-L, -S))

    books.sort()

    tails = []

    for _, neg_s in books:
        s = -neg_s

        # LIS tăng nghiêm ngặt dùng bisect_left.
        pos = bisect_left(tails, s)

        if pos == len(tails):
            tails.append(s)
        else:
            tails[pos] = s

    print(len(tails))


if __name__ == "__main__":
    solve()
