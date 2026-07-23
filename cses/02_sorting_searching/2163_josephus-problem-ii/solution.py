import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])  # số đứa bị bỏ qua mỗi lượt

    # Order-statistics segment tree đếm số phần tử còn tồn tại trên [1..n]
    # Kích thước lá: lũy thừa 2 nhỏ nhất >= n
    size = 1
    while size < n:
        size <<= 1
    tree = [0] * (2 * size)
    for i in range(n):
        tree[size + i] = 1                   # mỗi đứa ban đầu còn tồn tại -> lá = 1
    for i in range(size - 1, 0, -1):
        tree[i] = tree[2 * i] + tree[2 * i + 1]

    res = [0] * n
    pos = 0                                  # điểm bắt đầu đếm (0-based) trong các phần tử còn lại
    remaining = n
    for out in range(n):
        idx = (pos + k) % remaining          # chỉ số 0-based của đứa trẻ bị loại
        target = idx + 1                     # tìm phần tử thứ target (1-based) còn tồn tại

        # Đi xuống một lần: vừa tìm phần tử thứ target vừa xóa nó dọc đường
        node = 1
        tree[1] -= 1
        while node < size:
            left = node + node
            lc = tree[left]
            if lc >= target:
                node = left                  # đủ phần tử bên trái -> rẽ trái
            else:
                target -= lc                 # bỏ qua nhánh trái -> rẽ phải
                node = left + 1
            tree[node] -= 1

        res[out] = node - size + 1           # số hiệu đứa trẻ (1-based)
        pos = idx                            # đứa ngay sau chiếm đúng chỉ số idx trong mảng mới
        remaining -= 1

    sys.stdout.write(' '.join(map(str, res)) + '\n')


main()
