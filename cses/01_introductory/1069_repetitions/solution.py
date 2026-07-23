import sys
from itertools import groupby


def main():
    # Đọc nhanh toàn bộ dữ liệu; chuỗi DNA là token đầu tiên.
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    s = data[0]

    # groupby gộp các ký tự giống nhau liên tiếp thành từng đoạn (chạy ở tầng C).
    # Độ dài đoạn dài nhất chính là đáp án.
    best = max(sum(1 for _ in g) for _, g in groupby(s))
    sys.stdout.write(str(best) + "\n")


if __name__ == "__main__":
    main()
