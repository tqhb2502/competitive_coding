import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    prices = data[2:2 + n]
    pages = data[2 + n:2 + 2 * n]

    # dp[j] = số trang lớn nhất với tổng giá KHÔNG VƯỢT QUÁ j (0 <= j <= x).
    # Base case: chưa xét cuốn nào nên mọi ngân sách cho 0 trang.
    dp = [0] * (x + 1)
    top = x + 1

    # Xét lần lượt từng cuốn sách (0/1 knapsack).
    for i in range(n):
        p = int(prices[i])
        if p > x:
            continue                 # cuốn này không bao giờ mua được
        v = int(pages[i])

        # Cập nhật dp[j] = max(dp[j], dp[j - p] + v) cho mọi j từ p đến x.
        # seg_new = dp[p:x+1]   -> dp[j] hiện tại ("không mua cuốn i").
        # seg_old = dp[0:x+1-p] -> dp[j - p] trước khi xét cuốn i ("mua cuốn i").
        # Đọc bằng slice tạo bản sao độc lập trước khi ghi đè nên không có xung
        # đột thứ tự đọc/ghi; tương đương vòng lặp 0/1 knapsack duyệt j giảm dần.
        # dp luôn không giảm theo j nên kết quả duy trì tính đơn điệu.
        seg_new = dp[p:top]
        seg_old = dp[0:top - p]
        dp[p:top] = [a + v if a + v > b else b
                     for a, b in zip(seg_old, seg_new)]

    # Đáp án: dp[x] là số trang lớn nhất mua được với ngân sách x.
    sys.stdout.write(str(dp[x]) + "\n")


main()
