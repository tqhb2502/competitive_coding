import sys


def main():
    # Đọc toàn bộ dữ liệu; M là giá trị lớn nhất xuất hiện trong mảng.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    M = 0
    ints = []
    for i in range(1, n + 1):
        v = int(data[i])
        ints.append(v)
        if v > M:
            M = v

    # cnt[v] = số phần tử của mảng có giá trị đúng bằng v.
    cnt = [0] * (M + 1)
    for v in ints:
        cnt[v] += 1

    # Harmonic sieve: duyệt ước d từ lớn xuống nhỏ, số phần tử chia hết cho d là
    # sum(cnt[d::d]) (cộng cnt tại d, 2d, 3d, ...) tính ở tốc độ C.
    # d đầu tiên có >= 2 bội chính là gcd lớn nhất của một cặp -> đáp án.
    for d in range(M, 0, -1):
        if sum(cnt[d::d]) >= 2:
            sys.stdout.write(str(d))
            return

    # Vì n >= 2 nên luôn tìm thấy ở trên; nhánh này chỉ để phòng hờ.
    sys.stdout.write("1")


main()
