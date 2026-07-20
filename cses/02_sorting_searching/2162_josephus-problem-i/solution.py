# Josephus Problem I
# https://cses.fi/problemset/task/2162
#
# Mô phỏng vòng tròn bằng deque: mỗi bước "bỏ qua" 1 đứa (đưa xuống cuối hàng)
# rồi "loại" đứa tiếp theo (in ra). Độ phức tạp O(n) thời gian, O(n) bộ nhớ.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    q = deque(range(1, n + 1))
    res = []
    while q:
        # skip: đưa phần tử đầu xuống cuối hàng đợi
        q.append(q.popleft())
        # remove: loại phần tử đầu tiếp theo
        res.append(q.popleft())

    sys.stdout.write(' '.join(map(str, res)) + '\n')


main()
