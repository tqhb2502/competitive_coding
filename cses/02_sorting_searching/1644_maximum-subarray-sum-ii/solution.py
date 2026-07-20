# Maximum Subarray Sum II - https://cses.fi/problemset/task/1644
# Prefix sum + sliding window minimum bằng monotonic deque.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    a = int(data[idx]); idx += 1
    b = int(data[idx]); idx += 1

    # prefix[i] = tổng của a[0..i-1]; sum(l+1..r) = prefix[r] - prefix[l]
    prefix = [0] * (n + 1)
    s = 0
    for i in range(1, n + 1):
        s += int(data[idx]); idx += 1
        prefix[i] = s

    # Với mỗi r, chọn l trong [r-b, r-a] sao cho prefix[l] nhỏ nhất.
    dq = deque()  # các chỉ số l với giá trị prefix tăng dần
    ans = -(10 ** 18)
    for r in range(a, n + 1):
        l = r - a  # chỉ số l mới trở thành hợp lệ khi r tăng
        while dq and prefix[dq[-1]] >= prefix[l]:
            dq.pop()
        dq.append(l)
        # bỏ các l quá cũ (< r - b) => độ dài vượt quá b
        while dq[0] < r - b:
            dq.popleft()
        cur = prefix[r] - prefix[dq[0]]
        if cur > ans:
            ans = cur

    sys.stdout.write(str(ans) + "\n")


main()
