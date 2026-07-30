import sys


def main():
    data = sys.stdin.buffer.read().split()  # đọc nhanh toàn bộ input
    if not data:
        return
    n = int(data[0])

    # cnt[r] = số prefix đã gặp có số dư r khi chia cho n.
    cnt = [0] * n
    cnt[0] = 1  # prefix rỗng P[0] = 0

    prefix = 0
    ans = 0
    for i in range(1, n + 1):
        prefix += int(data[i])  # cộng dồn prefix sum
        # Toán tử % của Python đã trả về số dư không âm khi n dương.
        r = prefix % n
        # Mỗi prefix trước đó cùng số dư tạo thành một subarray hợp lệ.
        ans += cnt[r]
        cnt[r] += 1

    sys.stdout.write(str(ans) + "\n")


main()
