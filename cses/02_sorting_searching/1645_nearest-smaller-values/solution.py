import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc I/O
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = list(map(int, data[1:1 + n]))

    res = [0] * n          # res[i] = đáp án cho vị trí i (chỉ số bắt đầu từ 1)
    stack = []             # stack các chỉ số (0-based) có giá trị tăng nghiêm ngặt

    for i in range(n):
        x = a[i]
        # Pop các chỉ số có giá trị >= giá trị hiện tại: chúng không thể là
        # đáp án cho i cũng như cho bất kỳ vị trí nào bên phải i
        while stack and a[stack[-1]] >= x:
            stack.pop()
        if stack:
            # Đỉnh stack là chỉ số gần nhất bên trái nhỏ hơn x (đổi sang 1-indexed)
            res[i] = stack[-1] + 1
        # Nếu stack rỗng, giữ nguyên res[i] = 0
        stack.append(i)

    sys.stdout.write(' '.join(map(str, res)))
    sys.stdout.write('\n')


main()
