import sys


def main():
    # Đọc toàn bộ input một lần rồi tách thành các token cho nhanh
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])

    # Hash map ánh xạ: giá trị đã gặp -> vị trí (1-based) của phần tử đứng trước
    seen = {}
    out = "IMPOSSIBLE"

    # Duyệt các phần tử mảng: data[2 .. 2+n-1]
    for i in range(n):
        a = int(data[2 + i])
        need = x - a
        # Nếu phần bổ sung đã gặp trước đó thì tìm được cặp vị trí phân biệt
        if need in seen:
            out = str(seen[need]) + " " + str(i + 1)
            break
        # Ghi lại vị trí đầu tiên gặp giá trị này (không ghi đè nếu đã có)
        if a not in seen:
            seen[a] = i + 1

    sys.stdout.write(out + "\n")


main()
