import sys
from collections import deque


def main():
    # Đọc toàn bộ input một lần rồi tách thành các token để tăng tốc I/O.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Đưa tất cả n đứa trẻ vào deque theo thứ tự 1..n để mô phỏng vòng tròn.
    q = deque(range(1, n + 1))
    res = []
    while q:
        # Bỏ qua (skip): chuyển phần tử đầu xuống cuối hàng đợi.
        q.append(q.popleft())
        # Loại (remove): phần tử đầu tiếp theo bị loại, ghi vào kết quả.
        res.append(q.popleft())

    # Gộp toàn bộ kết quả thành một chuỗi rồi ghi ra một lần để tăng tốc I/O.
    sys.stdout.write(' '.join(map(str, res)) + '\n')


main()
