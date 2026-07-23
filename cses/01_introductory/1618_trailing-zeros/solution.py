import sys


def main():
    # Đọc n từ input
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Công thức Legendre: đếm số mũ của thừa số nguyên tố 5 trong n!,
    # cũng chính là số chữ số 0 ở cuối của n!.
    # Mỗi vòng cộng dồn số bội của 5, 25, 125, ... trong [1..n].
    res = 0
    p = 5
    while p <= n:
        res += n // p
        p *= 5

    sys.stdout.write(str(res) + "\n")


if __name__ == "__main__":
    main()
