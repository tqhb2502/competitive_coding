# Towers - https://cses.fi/problemset/task/1073
# Greedy + binary search (patience sorting): duy trì tops của các tháp theo thứ tự tăng dần.
import sys
from bisect import bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    tops = []  # danh sách các giá trị đỉnh tháp, luôn giữ sắp xếp tăng dần
    for i in range(1, n + 1):
        k = int(data[i])
        # tìm tháp có đỉnh nhỏ nhất nhưng STRICTLY lớn hơn k (upper bound)
        j = bisect_right(tops, k)
        if j == len(tops):
            # không có tháp nào đạt được -> tạo tháp mới (k >= mọi đỉnh -> append giữ thứ tự)
            tops.append(k)
        else:
            # đặt cube lên tháp đó; thay đỉnh cũ bằng k vẫn giữ tops sắp xếp
            tops[j] = k
    sys.stdout.write(str(len(tops)) + "\n")


main()
