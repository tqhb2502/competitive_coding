import sys


def main():
    # Đọc toàn bộ dữ liệu một lần bằng fast I/O
    data = sys.stdin.buffer.read().split()
    if not data:
        print(0)
        return
    n = int(data[0])
    a = data[1:1 + n]  # giữ nguyên dạng bytes để so sánh, khỏi ép kiểu int

    last = {}   # id -> chỉ số xuất hiện gần nhất
    left = 0    # biên trái của cửa sổ trượt hiện tại
    best = 0
    # Duyệt con trỏ phải; mở rộng cửa sổ [left, right] gồm các id khác nhau
    for right in range(n):
        x = a[right]
        # Nếu id lặp lại và bản cũ còn nằm trong cửa sổ thì nhảy biên trái
        prev = last.get(x)
        if prev is not None and prev >= left:
            left = prev + 1
        last[x] = right
        # Cập nhật độ dài lớn nhất của dãy hợp lệ kết thúc tại right
        length = right - left + 1
        if length > best:
            best = length

    sys.stdout.write(str(best) + "\n")


main()
