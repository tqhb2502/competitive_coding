import sys


# Tìm chữ số nằm ở vị trí thứ k (đếm từ 1) trong chuỗi 123456789101112...
def solve(k):
    # d: số chữ số của nhóm hiện tại
    # count: số lượng số có d chữ số = 9 * 10^(d-1)
    # start: số nhỏ nhất có d chữ số = 10^(d-1)
    d = 1
    count = 9
    start = 1
    # Trừ dần tổng số chữ số của từng nhóm cho đến khi k rơi vào nhóm hiện tại
    while k > d * count:
        k -= d * count
        d += 1
        count *= 10
        start *= 10
    # k là vị trí 1-indexed trong khối các số có d chữ số:
    # xác định số chứa chữ số và chỉ số chữ số bên trong số đó
    number = start + (k - 1) // d
    digit_index = (k - 1) % d
    return str(number)[digit_index]


def main():
    data = sys.stdin.buffer.read().split()
    q = int(data[0])
    out = []
    for i in range(1, q + 1):
        k = int(data[i])
        out.append(solve(k))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
