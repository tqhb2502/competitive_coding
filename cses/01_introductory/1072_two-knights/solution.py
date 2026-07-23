import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    out = []
    # Với mỗi bàn cờ k x k (k từ 1 đến n) áp dụng công thức đếm bù trừ.
    for k in range(1, n + 1):
        # Tổng số cách chọn 2 ô bất kỳ: C(k*k, 2) = k*k*(k*k-1)//2.
        total = k * k * (k * k - 1) // 2
        # Số cặp Mã tấn công nhau: 4 * (k-1) * (k-2).
        attacking = 4 * (k - 1) * (k - 2)
        # Kết quả = tổng số cặp - số cặp tấn công nhau.
        out.append(str(total - attacking))

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
