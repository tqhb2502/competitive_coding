import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh (I/O nhanh trên CSES).
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    xs = [0] * n
    ys = [0] * n
    pos = 1
    for i in range(n):
        xs[i] = int(data[pos])
        ys[i] = int(data[pos + 1])
        pos += 2

    contains = bytearray(n)   # đoạn i có chứa đoạn khác?
    contained = bytearray(n)  # đoạn i có bị đoạn khác chứa?

    # (A) "bị chứa": sắp x tăng, y giảm (nhờ dấu -ys) rồi giữ max_y các đoạn đã xét.
    # Nếu y hiện tại <= max_y thì có đoạn trước bao trọn đoạn này.
    a = sorted((xs[i], -ys[i], i) for i in range(n))
    max_y = -1
    for _x, ny, i in a:
        y = -ny
        if y <= max_y:
            contained[i] = 1
        else:
            max_y = y

    # (B) "chứa đoạn khác": sắp x giảm (nhờ dấu -xs), y tăng rồi giữ min_y các đoạn đã xét.
    # Nếu y hiện tại >= min_y thì đoạn này bao trọn một đoạn trước.
    b = sorted((-xs[i], ys[i], i) for i in range(n))
    min_y = 1 << 62
    for _nx, y, i in b:
        if y >= min_y:
            contains[i] = 1
        else:
            min_y = y

    # Dòng 1: cờ "chứa"; dòng 2: cờ "bị chứa".
    out1 = ' '.join('1' if v else '0' for v in contains)
    out2 = ' '.join('1' if v else '0' for v in contained)
    sys.stdout.write(out1 + '\n' + out2 + '\n')


main()
