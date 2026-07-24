import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Giá trị mex tại ô (i, j) theo 0-indexed chính là i XOR j.
    # Tạo chuỗi từng hàng bằng join rồi gom mọi hàng vào danh sách out,
    # cuối cùng ghi ra một lần (không dựng mảng lưới 2 chiều).
    out = []
    for i in range(n):
        out.append(" ".join(str(i ^ j) for j in range(n)))

    sys.stdout.write("\n".join(out))
    # Thêm ký tự xuống dòng kết thúc, bỏ qua khi n = 0 (không có hàng nào).
    if n > 0:
        sys.stdout.write("\n")


main()
