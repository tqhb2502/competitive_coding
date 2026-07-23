import sys


def main():
    # Đọc toàn bộ input một lần (fast I/O) để tránh TLE.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    p = [int(v) for v in data[2:2 + n]]

    # Sắp xếp cân nặng tăng dần để dùng two-pointer.
    p.sort()

    # left: đứa nhẹ nhất còn lại; right: đứa nặng nhất còn lại.
    left = 0
    right = n - 1
    ans = 0

    # Mỗi bước xử lý đứa nặng nhất còn lại, tốn đúng 1 gondola.
    while left <= right:
        # Nếu ghép được đứa nhẹ nhất vào chung thì cho nó lên cùng.
        if p[left] + p[right] <= x:
            left += 1
        right -= 1
        ans += 1

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
