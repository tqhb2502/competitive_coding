import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Greedy: mỗi bước trừ n đi chữ số lớn nhất của nó (tối ưu, trùng với DP).
    steps = 0
    while n > 0:
        # Tìm chữ số lớn nhất trong biểu diễn thập phân của n.
        mx = 0
        x = n
        while x:
            d = x % 10
            if d > mx:
                mx = d
            x //= 10
        n -= mx
        steps += 1

    sys.stdout.write(str(steps) + "\n")


if __name__ == "__main__":
    main()
