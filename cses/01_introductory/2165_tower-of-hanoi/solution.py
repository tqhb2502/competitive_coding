import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    moves = []

    # Đệ quy mô phỏng bằng stack tường minh để tránh giới hạn đệ quy của Python
    # (thực ra n <= 16 nên đệ quy thường cũng đủ an toàn).
    # Mỗi frame: (m, src, dst, aux, stage) với stage đánh dấu giai đoạn đang xử lý.
    stack = [(n, 1, 3, 2, 0)]
    while stack:
        m, src, dst, aux, stage = stack.pop()
        if m == 0:
            continue
        if stage == 0:
            # Đẩy phần tiếp theo (stage 1) rồi tới bài con: dọn m-1 đĩa sang cọc phụ.
            stack.append((m, src, dst, aux, 1))
            stack.append((m - 1, src, aux, dst, 0))
        elif stage == 1:
            # Chuyển đĩa lớn nhất từ nguồn sang đích, rồi đưa m-1 đĩa nhỏ về đích.
            moves.append((src, dst))
            stack.append((m - 1, aux, dst, src, 0))

    # In số bước rồi từng bước "a b".
    out = [str(len(moves))]
    for a, b in moves:
        out.append(str(a) + " " + str(b))
    sys.stdout.write("\n".join(out) + "\n")


main()
