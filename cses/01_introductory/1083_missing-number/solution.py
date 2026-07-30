import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Tổng đầy đủ S = 1 + 2 + ... + n
    total = n * (n + 1) // 2

    # Cộng dồn n-1 số được cho để có tổng T
    s = 0
    for i in range(1, n):
        s += int(data[i])

    # Số bị thiếu = S - T
    sys.stdout.write(str(total - s) + "\n")


if __name__ == "__main__":
    main()
