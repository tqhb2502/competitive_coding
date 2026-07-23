import sys


def main():
    # Đọc toàn bộ dữ liệu vào một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])

    MOD = 1000000007

    # dp[i] = số cách tạo tổng đúng bằng i; điểm neo dp[0] = 1 (không tung lần nào).
    dp = [0] * (n + 1)
    dp[0] = 1

    # Cửa sổ trượt: window = dp[i-1] + ... + dp[i-6], duy trì O(1) mỗi bước
    # thay cho việc cộng lại đủ 6 số hạng của công thức truy hồi.
    window = 0
    for i in range(1, n + 1):
        window += dp[i - 1]          # thêm phần tử mới dp[i-1]
        if i - 7 >= 0:
            window -= dp[i - 7]      # bỏ phần tử dp[i-7] khi cửa sổ quá 6 số hạng
        window %= MOD
        dp[i] = window

    # Đáp án là dp[n].
    sys.stdout.write(str(dp[n]) + "\n")


main()
