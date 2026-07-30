import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    p = list(map(int, data[1:1 + n]))

    # Sắp xếp rồi lấy median: điểm làm tổng khoảng cách đạt cực tiểu.
    p.sort()
    median = p[n // 2]

    # Cộng dồn |x - median| để ra tổng chi phí nhỏ nhất.
    total = 0
    for x in p:
        total += x - median if x >= median else median - x

    sys.stdout.write(str(total) + "\n")


if __name__ == "__main__":
    main()
