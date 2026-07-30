import sys


def main():
    # Đọc toàn bộ input một lần bằng buffer để I/O nhanh (tránh input())
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])

    count = {0: 1}  # prefix sum -> số lần xuất hiện; khởi tạo P[0] = 0
    prefix = 0
    ans = 0

    idx = 2
    for i in range(n):
        prefix += int(data[idx + i])  # cập nhật prefix sum P[r]
        # Subarray kết thúc tại đây có tổng x cần prefix trước đó bằng prefix - x
        ans += count.get(prefix - x, 0)
        count[prefix] = count.get(prefix, 0) + 1  # ghi nhận prefix sum hiện tại

    sys.stdout.write(str(ans) + "\n")


main()
