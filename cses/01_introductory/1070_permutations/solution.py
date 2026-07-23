import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Các trường hợp đặc biệt: n = 1 luôn hợp lệ; n = 2 và n = 3 vô nghiệm
    if n == 1:
        sys.stdout.write("1\n")
        return
    if n == 2 or n == 3:
        sys.stdout.write("NO SOLUTION\n")
        return

    # Với n >= 4: in các số chẵn trước, rồi tới các số lẻ.
    # Trong mỗi nhóm hai số liên tiếp hơn kém nhau 2, và điểm nối (4 với 1)
    # có hiệu 3 nên không có cặp kề nhau nào hiệu bằng 1.
    result = list(range(2, n + 1, 2)) + list(range(1, n + 1, 2))
    sys.stdout.write(" ".join(map(str, result)))
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
