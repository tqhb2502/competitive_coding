import sys


def main():
    # Đọc nhanh toàn bộ dữ liệu vào.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = [int(x) for x in data[1:1 + n]]

    total = sum(a)

    if n == 1:
        # Chỉ có 1 phần tử: người 1 buộc phải lấy nó.
        sys.stdout.write(str(a[0]) + "\n")
        return

    # dp[i] = hiệu điểm tối ưu (người tới lượt trừ đối thủ) của đoạn bắt đầu tại i
    # với độ dài đang xét. Khởi tạo với độ dài 1: dp[i][i] = a[i].
    dp = a[:]

    # Quy hoạch động theo độ dài đoạn L tăng dần (m = n-L+1 đoạn hợp lệ):
    #   dp_mới[i] = max(a[i] - dp_cũ[i+1], a[i+L-1] - dp_cũ[i]).
    # Dùng zip trên các slice để đẩy vòng lặp xuống mức C của CPython.
    for L in range(2, n + 1):
        m = n - L + 1
        dp[0:m] = [
            h if h >= t else t
            for h, t in (
                (ah - dn, at - dc)
                for ah, dn, at, dc in zip(
                    a[0:m], dp[1:m + 1], a[L - 1:L - 1 + m], dp[0:m]
                )
            )
        ]

    diff = dp[0]  # dp[0][n-1] = điểm người 1 - điểm người 2
    ans = (total + diff) // 2
    sys.stdout.write(str(ans) + "\n")


main()
