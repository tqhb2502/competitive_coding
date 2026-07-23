import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # moves: tổng số bước cần thực hiện
    # prev: giá trị phần tử liền trước sau khi đã chỉnh sửa
    # Python dùng số nguyên lớn tùy ý nên không lo tràn số
    moves = 0
    prev = 0
    # Duyệt mảng từ trái sang phải theo tư tưởng greedy (tham lam)
    for i in range(1, n + 1):
        x = int(data[i])
        if x < prev:
            # Phần tử nhỏ hơn prev: tăng nó lên bằng prev, tốn (prev - x) bước
            moves += prev - x
        else:
            # Phần tử đã >= prev: không cần chỉnh, cập nhật prev = x
            prev = x

    sys.stdout.write(str(moves) + "\n")


if __name__ == "__main__":
    main()
