import sys


def main():
    # Đọc số nguyên dương n từ đầu vào
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Mô phỏng trực tiếp dãy Collatz, gom lần lượt các giá trị vào res
    # (Python hỗ trợ số nguyên lớn vô hạn nên không lo tràn số)
    res = []
    while n != 1:
        res.append(n)
        if n % 2 == 0:
            n //= 2         # n chẵn: chia đôi
        else:
            n = 3 * n + 1   # n lẻ: nhân ba cộng một
    res.append(1)           # Luôn kết thúc bằng giá trị 1

    # In một lần bằng fast I/O để tránh chậm khi dãy dài
    sys.stdout.write(" ".join(map(str, res)) + "\n")


if __name__ == "__main__":
    main()
