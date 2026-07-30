import sys


def main():
    # Đọc toàn bộ input một lần rồi lấy n
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    total = 1 << n   # tổng số xâu cần in là 2^n
    out = []
    for i in range(total):
        # Công thức Gray code chuẩn: gray(i) = i XOR (i >> 1)
        g = i ^ (i >> 1)
        # Định dạng thành xâu nhị phân đúng n bit (thêm số 0 ở bên trái)
        out.append(format(g, '0{}b'.format(n)))

    # Gom toàn bộ output rồi ghi một lần (fast I/O)
    sys.stdout.write("\n".join(out) + "\n")


main()
