# Repetitions - CSES 1069
# https://cses.fi/problemset/task/1069
# Tìm độ dài đoạn các ký tự giống nhau liên tiếp dài nhất.

import sys
from itertools import groupby


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    s = data[0]
    # groupby chạy ở tầng C, nhanh hơn vòng lặp Python thuần túy.
    best = max(sum(1 for _ in g) for _, g in groupby(s))
    sys.stdout.write(str(best) + "\n")


if __name__ == "__main__":
    main()
