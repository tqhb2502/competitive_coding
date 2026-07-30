import sys


def main():
    # Đọc toàn bộ input một lần và tách thành các token chuỗi byte cho nhanh.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    # Lấy đúng n token sau token đầu tiên và đếm số chuỗi phân biệt bằng set.
    # Không cần ép kiểu int vì mỗi số có duy nhất một biểu diễn chuỗi thập phân.
    distinct = len(set(data[1:1 + n]))
    sys.stdout.write(str(distinct) + "\n")


main()
