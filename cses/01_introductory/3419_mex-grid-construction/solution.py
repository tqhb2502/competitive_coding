import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Giá trị mex tại ô (i, j) theo 0-indexed chính là i XOR j.
    # Sinh từng hàng bằng join, không cần lưu toàn bộ lưới.
    out = []
    for i in range(n):
        out.append(" ".join(str(i ^ j) for j in range(n)))

    sys.stdout.write("\n".join(out))
    if n > 0:
        sys.stdout.write("\n")


main()
