# Money Sums - CSES 1745
# https://cses.fi/problemset/task/1745
#
# Subset-sum (0/1 knapsack): tìm tất cả các tổng tập con khác nhau.
# Dùng số nguyên lớn của Python làm bitset: bit thứ i bật <=> tổng i tạo được.
# Với mỗi đồng xu c: dp |= dp << c  -> cập nhật toàn bộ tổng cùng lúc, rất nhanh.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    coins = data[1:1 + n]

    dp = 1  # bit 0 bật: tổng 0 tạo được (tập rỗng)
    for tok in coins:
        c = int(tok)
        dp |= dp << c

    # Các bit bật (trừ bit 0) là các tổng cần in.
    bits = bin(dp)[2:][::-1]  # bits[i] là bit thứ i
    sums = [str(i) for i in range(1, len(bits)) if bits[i] == '1']

    out = sys.stdout
    out.write(str(len(sums)))
    out.write("\n")
    out.write(" ".join(sums))
    out.write("\n")


if __name__ == "__main__":
    main()
