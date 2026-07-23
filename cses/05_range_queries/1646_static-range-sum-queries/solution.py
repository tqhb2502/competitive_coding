import sys
from itertools import accumulate


def main():
    # Đọc toàn bộ input một lần cho nhanh
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    x = data[idx:idx + n]
    idx += n
    # Dựng mảng tổng tiền tố với phần tử đầu bằng 0:
    # prefix[0] = 0, prefix[i] = x[1] + ... + x[i]
    prefix = list(accumulate((int(v) for v in x), initial=0))

    out = []
    out_append = out.append
    pref = prefix
    # Mỗi truy vấn (a, b): tổng đoạn [a, b] = prefix[b] - prefix[a-1], tốn O(1)
    for _ in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        out_append(str(pref[b] - pref[a - 1]))

    # Ghi kết quả một lần
    sys.stdout.buffer.write(("\n".join(out) + "\n").encode())


if __name__ == "__main__":
    main()
