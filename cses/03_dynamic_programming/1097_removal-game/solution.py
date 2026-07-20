# Removal Game - CSES 1097
# https://cses.fi/problemset/task/1097
#
# DP trên đoạn con (zero-sum game / minimax):
#   dp[i][j] = hiệu điểm lớn nhất (người tới lượt TRỪ đối thủ) trên đoạn a[i..j].
#   dp[i][j] = max(a[i] - dp[i+1][j], a[j] - dp[i][j-1]), base dp[i][i] = a[i].
#   Đáp án người 1 = (tổng + dp[0][n-1]) // 2.
# Tối ưu bộ nhớ O(n) bằng cách lặp theo độ dài đoạn với mảng 1 chiều.
# Thuần Python chuẩn (chỉ dùng sys), không dùng thư viện ngoài.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = [int(x) for x in data[1:1 + n]]

    total = sum(a)

    if n == 1:
        # Người 1 buộc phải lấy phần tử duy nhất.
        sys.stdout.write(str(a[0]) + "\n")
        return

    # dp[i] giữ dp cho đoạn bắt đầu tại i với độ dài hiện tại.
    # Khởi tạo: độ dài 1 => dp[i][i] = a[i].
    dp = a[:]

    # Lặp theo độ dài đoạn L từ 2 đến n.
    # Với độ dài L: i chạy 0..n-L (m = n-L+1 đoạn hợp lệ).
    # dp_mới[i] = max(a[i] - dp_cũ[i+1], a[i+L-1] - dp_cũ[i])
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
