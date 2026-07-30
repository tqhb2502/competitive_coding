import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    a = int(data[idx]); idx += 1  # độ dài nhỏ nhất của subarray
    b = int(data[idx]); idx += 1  # độ dài lớn nhất của subarray

    # prefix[i] = tổng arr[0..i-1]; tổng subarray (l+1..r) = prefix[r] - prefix[l]
    prefix = [0] * (n + 1)
    s = 0
    for i in range(1, n + 1):
        s += int(data[idx]); idx += 1
        prefix[i] = s

    # Với mỗi r, chọn l trong cửa sổ [r-b, r-a] sao cho prefix[l] nhỏ nhất
    dq = deque()  # monotonic deque: các chỉ số l theo prefix tăng dần
    ans = -(10 ** 18)  # khởi tạo rất nhỏ để xử lý trường hợp mọi tổng đều âm
    for r in range(a, n + 1):
        # Chỉ số l = r - a vừa trở nên hợp lệ, đẩy vào deque giữ tính đơn điệu
        l = r - a
        while dq and prefix[dq[-1]] >= prefix[l]:
            dq.pop()
        dq.append(l)
        # Loại các l quá cũ (< r - b) vì độ dài vượt quá b
        while dq[0] < r - b:
            dq.popleft()
        # Đầu deque là l có prefix nhỏ nhất trong cửa sổ hợp lệ
        cur = prefix[r] - prefix[dq[0]]
        if cur > ans:
            ans = cur

    sys.stdout.write(str(ans) + "\n")


main()
