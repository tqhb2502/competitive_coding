import sys
from bisect import bisect_right


def main():
    # Đọc toàn bộ input một lần để tăng tốc; data giữ toàn bộ token trong bộ nhớ
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # tops: giá trị đỉnh của các tháp, luôn giữ sắp xếp tăng dần (patience sorting)
    tops = []
    for i in range(1, n + 1):
        k = int(data[i])
        # Tìm tháp có đỉnh nhỏ nhất nhưng lớn hơn THỰC SỰ k (upper bound)
        j = bisect_right(tops, k)
        if j == len(tops):
            # Không có tháp phù hợp -> tạo tháp mới; k >= mọi đỉnh nên append giữ thứ tự
            tops.append(k)
        else:
            # Đặt khối lên tháp đó; thay đỉnh cũ bằng k vẫn giữ tops tăng dần
            tops[j] = k

    # Đáp số: số tháp tối thiểu chính là số phần tử của tops
    sys.stdout.write(str(len(tops)) + "\n")


main()
