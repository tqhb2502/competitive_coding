# All Manhattan Distances - https://cses.fi/problemset/task/3411
#
# Tổng khoảng cách Manhattan trên mọi cặp điểm:
#   sum |x_i - x_j| + sum |y_i - y_j|  (tách độc lập theo từng chiều).
# Với mỗi chiều: sort rồi dùng prefix sum. Sau khi sort tăng dần, đóng góp của
# a[k] là a[k]*k - (tổng a[0..k-1]). Cộng dồn qua mọi k => tổng theo cặp.
# Toàn bộ dùng số nguyên (big int của Python) => chính xác tuyệt đối, không tràn.

import sys


def sum_pairwise_abs(values):
    # Tổng |a_i - a_j| trên mọi cặp, dùng sort + prefix sum. Exact integer.
    values.sort()
    total = 0
    prefix = 0
    for k, v in enumerate(values):
        total += v * k - prefix
        prefix += v
    return total


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    n = int(data[0])
    xs = [0] * n
    ys = [0] * n
    idx = 1
    for i in range(n):
        xs[i] = int(data[idx])
        ys[i] = int(data[idx + 1])
        idx += 2
    answer = sum_pairwise_abs(xs) + sum_pairwise_abs(ys)
    sys.stdout.write(str(answer) + "\n")


if __name__ == "__main__":
    main()
