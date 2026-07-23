import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = data[1:1 + n]

    # pos[v] = vị trí (index) của giá trị v trong mảng
    pos = [0] * (n + 1)
    for i in range(n):
        pos[int(a[i])] = i

    # Mỗi lần pos[v] < pos[v-1] là một "đứt gãy" cần thêm một vòng mới;
    # số vòng = 1 + số đứt gãy
    rounds = 1
    for v in range(2, n + 1):
        if pos[v] < pos[v - 1]:
            rounds += 1

    sys.stdout.write(str(rounds) + "\n")


main()
