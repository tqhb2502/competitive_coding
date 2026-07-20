# De Bruijn Sequence - CSES 1692
# https://cses.fi/problemset/task/1692
#
# Xây xâu bit ngắn nhất chứa tất cả 2^n xâu con độ dài n.
# Độ dài tối thiểu là (n-1) + 2^n.
#
# Mô hình đồ thị De Bruijn:
#   - Mỗi đỉnh là một xâu bit độ dài n-1 (có 2^(n-1) đỉnh).
#   - Từ đỉnh u thêm một bit b (0 hoặc 1) tạo cạnh biểu diễn xâu con u+b
#     (độ dài n), cạnh đi tới đỉnh (n-1) bit cuối của u+b = ((u<<1)|b) & mask.
#   - Mỗi đỉnh có in-degree = out-degree = 2 nên tồn tại Eulerian circuit.
# Chạy Hierholzer lấy Eulerian circuit, ghi lại bit của từng cạnh; kết quả là
# tiền tố (n-1) bit của đỉnh xuất phát nối với dãy bit của các cạnh.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    mask = (1 << (n - 1)) - 1        # n=1 -> mask = 0 (một đỉnh rỗng, hai self-loop)
    num_nodes = 1 << (n - 1)
    ptr = [0] * num_nodes            # con trỏ cạnh kế tiếp chưa dùng của mỗi đỉnh (0/1/2)

    start = 0
    stack_node = [start]
    stack_bit = [-1]                 # -1 đánh dấu cạnh vào không tồn tại
    path = []

    # Hierholzer lặp (tránh đệ quy sâu).
    while stack_node:
        u = stack_node[-1]
        if ptr[u] < 2:
            b = ptr[u]
            ptr[u] += 1
            v = ((u << 1) | b) & mask
            stack_node.append(v)
            stack_bit.append(b)
        else:
            stack_node.pop()
            bit = stack_bit.pop()
            if bit != -1:
                path.append(bit)

    path.reverse()

    prefix = '0' * (n - 1)
    body = ''.join('01'[b] for b in path)
    sys.stdout.write(prefix + body + '\n')


main()
