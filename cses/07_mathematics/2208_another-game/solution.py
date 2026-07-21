# Another Game - CSES 2208
# https://cses.fi/problemset/task/2208
#
# Luật: mỗi lượt chọn một số (>=1) đống không rỗng, lấy 1 xu từ mỗi đống đã chọn.
# Người lấy viên xu cuối cùng thắng.
# Kết quả: người THỨ HAI thắng <=> tất cả các đống đều CHẴN; ngược lại THỨ NHẤT thắng.
# (Chỉ phụ thuộc tính chẵn/lẻ của từng đống, không phụ thuộc độ lớn.)

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1
    out = []
    for _ in range(t):
        n = int(data[idx]); idx += 1
        all_even = True
        # duyệt n giá trị, nếu tồn tại một đống lẻ => first thắng
        end = idx + n
        while idx < end:
            if int(data[idx]) & 1:
                all_even = False
                idx = end  # đã đủ kết luận, bỏ qua phần còn lại của test case này
                break
            idx += 1
        out.append("second" if all_even else "first")
    sys.stdout.write("\n".join(out) + "\n")


main()
