import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])

    # Đọc thời điểm đến và rời đi của từng khách hàng
    arrivals = [0] * n
    departures = [0] * n
    idx = 1
    for i in range(n):
        arrivals[i] = int(data[idx])
        departures[i] = int(data[idx + 1])
        idx += 2

    # Sắp xếp riêng mảng thời điểm đến và mảng thời điểm rời đi
    arrivals.sort()
    departures.sort()

    best = 0  # đỉnh (giá trị lớn nhất) của cur
    cur = 0   # số khách đang có mặt
    i = 0     # con trỏ trên mảng thời điểm đến
    j = 0     # con trỏ trên mảng thời điểm rời đi

    # Quét (sweep) bằng hai con trỏ để hợp nhất hai chuỗi sự kiện theo thời gian
    while i < n:
        if arrivals[i] < departures[j]:
            # Sự kiện đến: thêm một khách và cập nhật đỉnh
            cur += 1
            if cur > best:
                best = cur
            i += 1
        else:
            # Sự kiện rời đi: bớt một khách
            cur -= 1
            j += 1

    sys.stdout.write(str(best) + "\n")


if __name__ == "__main__":
    main()
