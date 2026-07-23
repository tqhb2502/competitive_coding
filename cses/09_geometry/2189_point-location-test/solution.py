import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc I/O.
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    idx = 1
    out = []
    # Xử lý từng truy vấn độc lập.
    for _ in range(t):
        x1 = int(data[idx]); y1 = int(data[idx + 1])
        x2 = int(data[idx + 2]); y2 = int(data[idx + 3])
        x3 = int(data[idx + 4]); y3 = int(data[idx + 5])
        idx += 6
        # cross product (tích có hướng) = (p2 - p1) x (p3 - p1); big int nên
        # số học chính xác, không tràn số.
        cross = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
        # Dấu của cross cho biết vị trí của p3 so với hướng nhìn từ p1 sang p2.
        if cross > 0:
            out.append("LEFT")   # quay ngược chiều kim đồng hồ -> bên trái
        elif cross < 0:
            out.append("RIGHT")  # quay theo chiều kim đồng hồ -> bên phải
        else:
            out.append("TOUCH")  # thẳng hàng -> nằm trên đường thẳng
    sys.stdout.write("\n".join(out) + "\n")


main()
