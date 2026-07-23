import sys


def main():
    # Đọc toàn bộ dữ liệu một lần rồi tách số để tăng tốc (tránh TLE)
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    x = int(data[idx]); idx += 1

    ans = 0
    cur = 0  # tổng của cửa sổ [left, right]
    left = 0
    # Two-pointer: mở rộng right, cộng a[right] vào cur
    for right in range(n):
        cur += int(data[idx + right])
        # Thu hẹp cửa sổ từ bên trái khi tổng vượt quá x
        while cur > x:
            cur -= int(data[idx + left])
            left += 1
        # Cửa sổ hiện tại có tổng đúng bằng x => đếm thêm một subarray
        if cur == x:
            ans += 1

    sys.stdout.write(str(ans) + "\n")


main()
