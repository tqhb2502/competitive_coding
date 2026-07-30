import sys


def main():
    # Đọc toàn bộ input; n, m rồi n hàng ký tự (mỗi hàng là bytes chỉ gồm A,B,C,D)
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    rows = data[2:2 + n]

    colors = b'ABCD'
    out = []
    prev_row = None  # hàng trước đã tô (bytearray các mã ASCII)

    # Duyệt row-major, tô tham lam từng ô
    for i in range(n):
        orig = rows[i]
        cur = bytearray(m)
        for j in range(m):
            # Ba ràng buộc: khác màu gốc, khác ô trên, khác ô trái
            forbid_o = orig[j]
            up = prev_row[j] if prev_row is not None else 0
            left = cur[j - 1] if j > 0 else 0
            # Chọn màu đầu tiên hợp lệ (luôn tồn tại vì có 4 màu, tối đa 3 bị cấm)
            for c in colors:
                if c != forbid_o and c != up and c != left:
                    cur[j] = c
                    break
        out.append(cur.decode())
        prev_row = cur

    sys.stdout.write('\n'.join(out))
    sys.stdout.write('\n')


main()
