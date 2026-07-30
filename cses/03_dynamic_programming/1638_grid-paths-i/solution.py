import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    grid = data[1:1 + n]

    MOD = 1000000007

    # dp[j] = số đường đi đến ô (i, j) của hàng hiện tại (mảng cuộn 1 chiều).
    dp = [0] * n

    for i in range(n):
        row = grid[i]
        # left giữ dp[j-1] vừa tính của hàng này; đầu mỗi hàng chưa có ô trái nên = 0.
        left = 0
        for j in range(n):
            # row là bytes; ký tự '*' có mã byte 42.
            if row[j] == 42:
                # Ô bẫy: không đứng được, cũng chặn đường đi qua nó.
                dp[j] = 0
                left = 0
            elif i == 0 and j == 0:
                # Cơ sở: ô xuất phát.
                dp[j] = 1
                left = 1
            else:
                # dp[j] đang là giá trị hàng trên; left là ô bên trái cùng hàng.
                v = dp[j] + left
                if v >= MOD:  # trừ nhanh thay cho phép chia lấy dư
                    v -= MOD
                dp[j] = v
                left = v

    # Đáp án: số đường đi đến ô dưới-phải.
    sys.stdout.write(str(dp[n - 1]) + "\n")


main()
