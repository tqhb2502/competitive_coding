import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    w = [int(data[2 + i]) for i in range(n)]

    # Mã hoá cặp (rides, weight_chuyến_cuối) thành một số: state = rides * BIG + weight.
    # BIG = x + 1 vì weight luôn <= x, nên lấy min trên số này chính là so sánh
    # lexicographic (ưu tiên rides nhỏ, rồi tới weight của chuyến cuối).
    BIG = x + 1
    size = 1 << n
    full = size - 1

    dp = [0] * size
    dp[0] = BIG                             # Base case: 1 chuyến mở, chưa chở ai (rides=1, weight=0).

    for mask in range(1, size):
        best = 1 << 62
        m = mask
        # Thử chọn từng người p là người được thêm cuối cùng vào mask.
        while m:
            low = m & (-m)
            p = low.bit_length() - 1
            prev = dp[mask ^ low]
            wp = w[p]
            wt = prev % BIG                 # Weight của chuyến cuối ở state prev.
            if wt + wp <= x:                # Còn chỗ -> nhét p vào chuyến đang mở.
                cand = prev + wp
            else:                           # Chuyến cuối đã đầy -> mở chuyến mới cho p.
                cand = prev - wt + BIG + wp
            if cand < best:
                best = cand
            m ^= low
        dp[mask] = best

    # Đáp án là phần rides của state đầy đủ.
    sys.stdout.write(str(dp[full] // BIG) + "\n")


main()
