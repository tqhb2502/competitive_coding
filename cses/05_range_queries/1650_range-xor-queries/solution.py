import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # Dựng mảng tiền tố XOR: prefix[i] = x_1 XOR ... XOR x_i, prefix[0] = 0
    prefix = [0] * (n + 1)
    acc = 0
    for i in range(1, n + 1):
        acc ^= int(data[idx]); idx += 1
        prefix[i] = acc

    # Trả lời mỗi truy vấn trong O(1): XOR đoạn [a, b] = prefix[b] XOR prefix[a-1]
    out = []
    out_append = out.append
    for _ in range(q):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        out_append(str(prefix[b] ^ prefix[a - 1]))

    # Xuất kết quả một lần
    sys.stdout.buffer.write(("\n".join(out) + "\n").encode())


if __name__ == "__main__":
    main()
