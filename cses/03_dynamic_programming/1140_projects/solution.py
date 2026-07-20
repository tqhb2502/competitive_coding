# Projects - CSES 1140
# https://cses.fi/problemset/task/1140
# Weighted interval scheduling: sắp xếp theo ngày kết thúc, DP + tìm kiếm nhị phân.

import sys
from bisect import bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    projects = []
    for _ in range(n):
        a = int(data[idx]); b = int(data[idx + 1]); p = int(data[idx + 2])
        idx += 3
        projects.append((b, a, p))  # sort by end day b

    projects.sort()

    ends = [b for (b, a, p) in projects]

    dp = [0] * (n + 1)
    for i in range(1, n + 1):
        b, a, p = projects[i - 1]
        # số dự án có b < a  (dự án tương thích): cần a_next >= b_prev + 1
        j = bisect_left(ends, a)
        take = dp[j] + p
        skip = dp[i - 1]
        dp[i] = take if take > skip else skip

    sys.stdout.write(str(dp[n]) + "\n")


if __name__ == "__main__":
    main()
