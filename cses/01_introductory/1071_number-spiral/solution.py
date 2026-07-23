import sys


def main():
    # Đọc toàn bộ dữ liệu một lần cho nhanh
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    idx = 1
    for _ in range(t):
        y = int(data[idx])
        x = int(data[idx + 1])
        idx += 2

        # Lớp chữ "L" chứa ô (y, x) chính là max(y, x)
        m = y if y > x else x

        # Hướng tăng của lớp đảo chiều tùy theo lớp chẵn hay lẻ
        if m % 2 == 0:
            if x == m:
                # Cột phải: giá trị tăng khi y tăng
                ans = (m - 1) * (m - 1) + y
            else:  # y == m: hàng dưới, giá trị giảm khi x tăng
                ans = m * m - x + 1
        else:
            if y == m:
                # Hàng dưới: giá trị tăng khi x tăng
                ans = (m - 1) * (m - 1) + x
            else:  # x == m: cột phải, giá trị giảm khi y tăng
                ans = m * m - y + 1

        out.append(ans)

    # In tất cả kết quả một lần
    sys.stdout.write("\n".join(map(str, out)) + "\n")


if __name__ == "__main__":
    main()
