import sys


def main():
    # Đọc toàn bộ dữ liệu một lần cho nhanh (fast I/O)
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1  # số ứng viên
    m = int(data[idx]); idx += 1  # số căn hộ
    k = int(data[idx]); idx += 1  # sai số cho phép (tolerance)

    a = data[idx:idx + n]
    idx += n
    b = data[idx:idx + m]
    idx += m

    # Sắp xếp cả hai mảng tăng dần để chạy greedy bằng two-pointer
    a = sorted(map(int, a))
    b = sorted(map(int, b))

    i = 0  # con trỏ ứng viên
    j = 0  # con trỏ căn hộ
    count = 0
    # Two-pointer: ghép ứng viên nhỏ nhất chưa ghép với căn hộ vừa vặn nhỏ nhất
    while i < n and j < m:
        if b[j] < a[i] - k:
            # Căn hộ quá nhỏ cho mọi ứng viên còn lại -> bỏ căn hộ này
            j += 1
        elif b[j] > a[i] + k:
            # Căn hộ quá lớn -> ứng viên này không thể nhận căn hộ nào nữa
            i += 1
        else:
            # Nằm trong đoạn chấp nhận -> ghép một cặp, chuyển cả hai con trỏ
            count += 1
            i += 1
            j += 1

    sys.stdout.write(str(count) + "\n")


main()
