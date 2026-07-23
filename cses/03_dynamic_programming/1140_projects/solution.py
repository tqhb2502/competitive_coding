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
    ends = [t[0] for t in projects]  # mảng các ngày kết thúc đã sắp xếp

    # dp[i] = tổng tiền thưởng lớn nhất khi chỉ xét i dự án đầu tiên.
    dp = [0] * (n + 1)
    bl = bisect_left
    prev = 0  # prev = dp[i], tiền thưởng tối đa với i dự án đầu tiên
    for i in range(n):
        b, a, p = projects[i]
        # Số dự án có ngày kết thúc < a (dự án tương thích): cần a_sau >= b + 1.
        j = bl(ends, a, 0, i + 1)
        # Chọn phương án tốt hơn: nhận dự án i hay bỏ qua nó.
        take = dp[j] + p
        if take > prev:
            prev = take
        dp[i + 1] = prev

    sys.stdout.write(str(prev) + "\n")


if __name__ == "__main__":
    main()
