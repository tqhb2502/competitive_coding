import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    arr = [int(v) for v in data[2:2 + n]]

    # feasible(limit): dùng greedy kiểm tra có chia được mảng thành <= k đoạn liên
    # tiếp, mỗi đoạn có tổng <= limit hay không (nhồi tối đa vào mỗi đoạn).
    def feasible(limit):
        parts = 1
        cur = 0
        for v in arr:
            # Vượt limit thì đóng đoạn cũ, mở đoạn mới bắt đầu bằng phần tử này
            if cur + v > limit:
                parts += 1
                cur = v
                if parts > k:
                    return False
            else:
                cur += v
        return parts <= k

    lo = max(arr)          # mỗi phần tử đơn lẻ phải nằm vừa trong một đoạn
    hi = sum(arr)          # khi k == 1 thì cả mảng là một đoạn duy nhất
    # Binary search trên đáp án: tìm limit khả thi nhỏ nhất
    while lo < hi:
        mid = (lo + hi) // 2
        if feasible(mid):
            hi = mid
        else:
            lo = mid + 1

    sys.stdout.write(str(lo) + "\n")


main()
