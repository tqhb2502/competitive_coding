import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Đồ thị De Bruijn: đỉnh là xâu bit độ dài n-1 (2^(n-1) đỉnh).
    # mask giữ lại n-1 bit cuối; n=1 -> mask = 0 (một đỉnh rỗng, hai self-loop).
    mask = (1 << (n - 1)) - 1
    num_nodes = 1 << (n - 1)
    ptr = [0] * num_nodes            # cạnh kế tiếp chưa dùng của mỗi đỉnh (bit 0 rồi bit 1)

    # Hierholzer lặp bằng stack tường minh (tránh đệ quy sâu).
    start = 0
    stack_node = [start]             # ngăn xếp các đỉnh đang duyệt
    stack_bit = [-1]                 # bit của cạnh đã đi vào đỉnh (-1: đỉnh xuất phát)
    path = []                        # bit các cạnh theo thứ tự kết thúc (ngược của Euler)

    while stack_node:
        u = stack_node[-1]
        if ptr[u] < 2:
            # Còn cạnh chưa dùng: thêm bit b rồi đi tới đỉnh kề.
            b = ptr[u]
            ptr[u] += 1
            v = ((u << 1) | b) & mask
            stack_node.append(v)
            stack_bit.append(b)
        else:
            # Hết cạnh: rút đỉnh khỏi stack và ghi lại bit của cạnh đi vào nó.
            stack_node.pop()
            bit = stack_bit.pop()
            if bit != -1:
                path.append(bit)

    # Đảo lại để có đúng thứ tự Eulerian circuit.
    path.reverse()

    # Kết quả: tiền tố n-1 bit '0' của đỉnh xuất phát nối với dãy bit các cạnh.
    prefix = '0' * (n - 1)
    body = ''.join('01'[b] for b in path)
    sys.stdout.write(prefix + body + '\n')


main()
