import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1
    out = []
    for _ in range(t):
        n = int(data[idx]); idx += 1
        # Người thứ hai thắng khi và chỉ khi MỌI đống đều chẵn.
        all_even = True
        end = idx + n
        # Duyệt n giá trị; nếu tồn tại một đống lẻ thì người thứ nhất thắng.
        while idx < end:
            if int(data[idx]) & 1:
                all_even = False
                idx = end  # Đã đủ kết luận, bỏ qua phần còn lại của test case này.
                break
            idx += 1
        out.append("second" if all_even else "first")
    sys.stdout.write("\n".join(out) + "\n")


main()
