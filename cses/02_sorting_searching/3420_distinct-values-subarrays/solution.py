import sys


def main():
    # Đọc toàn bộ input một lần bằng fast I/O.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = data[1:1 + n]  # giữ dạng bytes để so sánh nhanh, không cần ép sang int

    last = {}    # giá trị -> vị trí xuất hiện gần nhất
    left = 0     # con trỏ trái nhỏ nhất cho cửa sổ phân biệt
    total = 0
    # Two-pointer: duyệt con trỏ phải right, duy trì left để [left, right] phân biệt.
    for right in range(n):
        v = a[right]
        p = last.get(v)
        # Nếu v đã xuất hiện trong cửa sổ hiện tại, dời left qua vị trí đó.
        if p is not None and p >= left:
            left = p + 1
        # Cộng số subarray kết thúc tại right và bắt đầu trong [left, right].
        total += right - left + 1
        last[v] = right

    sys.stdout.write(str(total) + "\n")


main()
