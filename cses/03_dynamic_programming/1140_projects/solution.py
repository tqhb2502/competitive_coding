# Projects - CSES 1140
# https://cses.fi/problemset/task/1140
# Weighted interval scheduling: sắp xếp theo ngày kết thúc, DP + tìm kiếm nhị phân.

import sys
from bisect import bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Chuyển toàn bộ số sang int một lần (nhanh hơn vòng lặp Python).
    nums = list(map(int, data))
    a_list = nums[1::3]
    b_list = nums[2::3]
    p_list = nums[3::3]

    # Sắp xếp theo ngày kết thúc b tăng dần (tuple bắt đầu bằng b).
    projects = sorted(zip(b_list, a_list, p_list))
    ends = [t[0] for t in projects]  # mảng các b đã sắp xếp

    dp = [0] * (n + 1)
    bl = bisect_left
    prev = 0  # prev = dp[i], lợi nhuận tối đa với i dự án đầu tiên
    for i in range(n):
        b, a, p = projects[i]
        # số dự án có b < a (dự án tương thích): cần a_next >= b_prev + 1
        j = bl(ends, a, 0, i + 1)
        take = dp[j] + p
        if take > prev:
            prev = take
        dp[i + 1] = prev

    sys.stdout.write(str(prev) + "\n")


if __name__ == "__main__":
    main()
