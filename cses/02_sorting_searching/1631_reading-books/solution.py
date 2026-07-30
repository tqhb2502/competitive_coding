import sys


def main():
    # Nạp TOÀN BỘ input một lần thành danh sách token (bộ nhớ O(n), không stream).
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Duyệt qua từng quyển sách để tính đồng thời:
    #   total = S = tổng thời gian tất cả các sách
    #   mx    = M = thời gian của quyển dài nhất
    total = 0
    mx = 0
    for i in range(1, n + 1):
        t = int(data[i])
        total += t
        if t > mx:
            mx = t

    # Đáp án = max(2*M, S): nếu quyển dài nhất quá lớn (2*M > S) thì bị nghẽn
    # bởi 2*M, ngược lại đủ chỗ xếp lịch song song nên bằng S.
    ans = max(2 * mx, total)
    sys.stdout.write(str(ans) + "\n")


main()
