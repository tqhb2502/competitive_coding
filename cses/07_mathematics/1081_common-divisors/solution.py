# Common Divisors - https://cses.fi/problemset/task/1081
# Đáp án = ước d lớn nhất sao cho có ít nhất 2 phần tử của mảng chia hết cho d.
# Đếm số bội của mỗi d bằng harmonic sieve: sum(cnt[d::d]) ở tốc độ C.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    M = 0
    ints = []
    for i in range(1, n + 1):
        v = int(data[i])
        ints.append(v)
        if v > M:
            M = v

    cnt = [0] * (M + 1)
    for v in ints:
        cnt[v] += 1

    # Duyệt d từ lớn xuống nhỏ; d đầu tiên có >= 2 bội là đáp án.
    for d in range(M, 0, -1):
        if sum(cnt[d::d]) >= 2:
            sys.stdout.write(str(d))
            return

    # n >= 2 nên luôn tìm thấy ở trên; phòng hờ.
    sys.stdout.write("1")


main()
