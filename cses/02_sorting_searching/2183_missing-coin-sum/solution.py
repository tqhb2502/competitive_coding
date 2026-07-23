import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    coins = list(map(int, data[1:1 + n]))

    # Sắp xếp các đồng xu tăng dần để áp dụng greedy.
    coins.sort()

    # reach = tổng nhỏ nhất hiện chưa tạo được; bất biến: mọi tổng trong
    # [1, reach - 1] đều tạo được bằng các đồng đã xét.
    reach = 1
    for x in coins:
        if x <= reach:
            # Hai đoạn nối liền nhau, mở rộng phạm vi tạo được.
            reach += x
        else:
            # Đồng > reach nên có khoảng trống -> reach là đáp án nhỏ nhất bị thiếu.
            break

    sys.stdout.write(str(reach) + "\n")


if __name__ == "__main__":
    main()
