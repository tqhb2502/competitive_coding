import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Subarray phải khác rỗng nên khởi tạo bằng phần tử đầu tiên.
    cur = int(data[1])  # tổng subarray tốt nhất kết thúc tại phần tử hiện tại
    ans = cur           # đáp án tốt nhất từng thấy

    # Kadane: nếu cur âm thì bỏ đoạn trước và bắt đầu lại, ngược lại nối thêm x.
    for i in range(2, n + 1):
        x = int(data[i])
        cur = x if cur < 0 else cur + x  # cur = max(x, cur + x)
        if cur > ans:
            ans = cur

    sys.stdout.write(str(ans) + "\n")


main()
